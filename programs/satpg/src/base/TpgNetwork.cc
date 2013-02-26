
/// @file TpgNetwork.cc
/// @brief TpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgPrimitive.h"
#include "TpgFault.h"
#include "ym_networks/TgNetwork.h"
#include "ym_networks/TgNode.h"
#include "ym_networks/TgBlifReader.h"
#include "ym_networks/TgIscas89Reader.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// 全ての論理ノードを kTgGateCplx タイプとして登録する時に true にする．
// TpgPrimitive 関係のコードのデバッグ用
const bool FORCE_TO_CPLX_LOGIC = false;

inline
TpgNode**
alloc_nodearray(Alloc& alloc,
		ymuint n)
{
  void* p = alloc.get_memory(sizeof(TpgNode*) * n);
  return new (p) TpgNode*[n];
}


// immediate dominator リストをマージする．
TpgNode*
merge(TpgNode* node1,
      TpgNode* node2)
{
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( node1 == NULL || node2 == NULL ) {
      return NULL;
    }
    ymuint id1 = node1->id();
    ymuint id2 = node2->id();
    if ( id1 < id2 ) {
      node1 = node1->imm_dom();
    }
    else if ( id1 > id2 ) {
      node2 = node2->imm_dom();
    }
  }
}

// pair の最初の要素で比較を行なうファンクター
// こういうのは lambda 表記があると簡潔にすむ．
struct Lt
{
  bool
  operator()(const pair<ymuint, ymuint>& left,
	     const pair<ymuint, ymuint>& right)
  {
    return left.first < right.first;
  }

};

// 論理式から必要なプリミティブ数を数える．
ymuint
primitive_count(const LogExpr& expr)
{
  if ( expr.is_posiliteral() ) {
    // 肯定のリテラルは入力プリミティブ1つ
    // 同じ入力番号のリテラルがあったとしても共有しない．
    return 1;
  }
  if ( expr.is_negaliteral() ) {
    // 否定のリテラルはNOT入力プリミティブ1つ
    // 同じ入力番号のリテラルがあったとしても共有しない．
    return 1;
  }

  // 演算子のプリミティブが1つ
  ymuint n = 1;

  // あとは子供の論理式に対するプリミティブ数を足し合わせる．
  ymuint nc = expr.child_num();
  for (ymuint i = 0; i < nc; ++ i) {
    LogExpr expr1 = expr.child(i);
    n += primitive_count(expr1);
  }

  return n;
}

// ゲートタイプから代表故障を求める．
void
get_rep_faults(TpgNode* node,
	       TpgFault* f0,
	       TpgFault* f1,
	       TpgFault*& rep0,
	       TpgFault*& rep1)
{
  if ( node->is_input() ) {
    // どうでもいい
    return;
  }
  if ( node->is_output() ) {
    // バッファと同じ
    rep0 = f0;
    rep1 = f1;
    return;
  }

  tTgGateType type = node->gate_type();
  switch ( type ) {
  case kTgGateBuff:
    rep0 = f0;
    rep1 = f1;
    break;

  case kTgGateNot:
    rep0 = f1;
    rep1 = f0;
    break;

  case kTgGateAnd:
    rep0 = f0;
    rep1 = NULL;
    break;

  case kTgGateNand:
    rep0 = f1;
    rep1 = NULL;
    break;

  case kTgGateOr:
    rep0 = NULL;
    rep1 = f1;
    break;

  case kTgGateNor:
    rep0 = NULL;
    rep1 = f0;
    break;

  case kTgGateXor:
    rep0 = NULL;
    rep1 = NULL;
    break;

  case kTgGateCplx:
    // 面倒くさいので
    rep0 = NULL;
    rep1 = NULL;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief blif ファイルを読み込んでインスタンスを作る．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @note エラーが起こったら NULL を返す．
TpgNetwork*
TpgNetwork::read_blif(const string& filename,
		      const CellLibrary* cell_library)
{
  TgNetwork tgnetwork;
  TgBlifReader read;

  bool stat = read(filename, tgnetwork, cell_library);
  if ( !stat ) {
    return NULL;
  }

  TpgNetwork* network = new TpgNetwork(tgnetwork);

  return network;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @note エラーが起こったら NULL を返す．
TpgNetwork*
TpgNetwork::read_iscas89(const string& filename)
{
  TgNetwork tgnetwork;
  TgIscas89Reader read;

  bool stat = read(filename, tgnetwork);
  if ( !stat ) {
    return NULL;
  }

  TpgNetwork* network = new TpgNetwork(tgnetwork);

  return network;
}

// @brief コンストラクタ
// @param[in] tgnetwork もとのネットワーク
TpgNetwork::TpgNetwork(const TgNetwork& tgnetwork) :
  mAlloc(4096)
{
  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////

  mNodeNum = tgnetwork.node_num();
  mInputNum = tgnetwork.input_num1();
  mOutputNum = tgnetwork.output_num1();
  mFFNum = tgnetwork.ff_num();

  {
    void* p = mAlloc.get_memory(sizeof(TpgNode) * mNodeNum);
    mNodeArray = new (p) TpgNode[mNodeNum];
  }

  mNodeMap = alloc_nodearray(mAlloc, mNodeNum);
  mInputArray = alloc_nodearray(mAlloc, input_num2());
  mOutputArray = alloc_nodearray(mAlloc, output_num2());
  mOutputArray2 = alloc_nodearray(mAlloc, output_num2());

  mActNodeNum = 0;
  mActNodeArray = alloc_nodearray(mAlloc, mNodeNum);

  {
    void* p = mAlloc.get_memory(sizeof(bool) * mNodeNum);
    mTmpMark = new (p) bool[mNodeNum];
  }
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mTmpMark[i] = false;
  }

  ymuint id = 0;

  //////////////////////////////////////////////////////////////////////
  // 外部入力を作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npi = input_num2();
  for (ymuint i = 0; i < npi; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.input(i);
    TpgNode* node = make_node(id, tgnode);
    mInputArray[i] = node;
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  ymuint nl = tgnetwork.logic_num();
  for (ymuint i = 0; i < nl; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.sorted_logic(i);
    TpgNode* node = make_node(id, tgnode);
    if ( tgnode->is_cplx_logic() ) {
      // 論理式をとってくる．
      ymuint fid = tgnode->func_id();
      LogExpr expr = tgnetwork.get_lexp(fid);

      // プリミティブ数を数え，必要なメモリ領域を確保する．
      ymuint np = primitive_count(expr);
      node->mPrimitiveNum = np;
      void* p = mAlloc.get_memory(sizeof(TpgPrimitive) * np);
      node->mPrimitiveList = new (p) TpgPrimitive[np];

      // expr の内容を表すプリミティブの木を作る．
      // 結果は node->mPrimitiveList に直接書き込まれる．
      ymuint subid = 0;
      make_primitive(expr, tgnode, node->mPrimitiveList, subid);

      // 念のため計算どおりのプリミティブ数か確かめる．
      assert_cond( subid == np, __FILE__, __LINE__);
    }
    else if ( FORCE_TO_CPLX_LOGIC ) {
      // デバッグ用のコード
      // 組み込み型のゲートも TpgPrimitive を用いて表す．
      ymuint ni = tgnode->fanin_num();
      tTgGateType gate_type = tgnode->gate_type();

      // プリミティブ数はファンイン数＋1
      ymuint np = ni + 1;
      node->mPrimitiveNum = np;
      void* p = mAlloc.get_memory(sizeof(TpgPrimitive) * np);
      node->mPrimitiveList = new (p) TpgPrimitive[np];

      // 入力プリミティブの設定
      for (ymuint i = 0; i < ni; ++ i) {
	TpgPrimitive* prim = &node->mPrimitiveList[i];
	set_input_primitive(prim, i);
      }

      // 演算子(AND/OR/XOR)プリミティブの設定
      TpgPrimitive* prim = &node->mPrimitiveList[ni];
      set_logic_primitive(prim, gate_type, ni);
      for (ymuint i = 0; i < ni; ++ i) {
	prim->mFanins[i] = &node->mPrimitiveList[i];
      }

      // cplx_logic マークにつけ直す．
      node->mTypeId = 3U | (kTgGateCplx << 2);
    }
  }


  //////////////////////////////////////////////////////////////////////
  // 外部出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num2();
  for (ymuint i = 0; i < npo; ++ i, ++ id) {
    const TgNode* tgnode = tgnetwork.output(i);
    TpgNode* node = make_node(id, tgnode);
    mOutputArray[i] = node;
  }

  assert_cond( id == mNodeNum, __FILE__, __LINE__);


  //////////////////////////////////////////////////////////////////////
  // ファンアウトの情報をコピーする．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    const TgNode* tgnode = tgnetwork.node(i);
    ymuint nfo = tgnode->fanout_num();
    TpgNode* node = mNodeMap[tgnode->gid()];
    for (ymuint i = 0; i < nfo; ++ i) {
      const TgNode* otgnode = tgnode->fanout(i);
      TpgNode* onode = mNodeMap[otgnode->gid()];
      node->mFanouts[i] = onode;
    }
  }


  //////////////////////////////////////////////////////////////////////
  // 故障リストを作る．
  //////////////////////////////////////////////////////////////////////
  mFaultNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = &mNodeArray[i];
    mFaultNum += node->fanin_num() * 2 + 2;
  }
  {
    void* p = mAlloc.get_memory(sizeof(TpgFault) * mFaultNum);
    mFaultChunk = new (p) TpgFault[mFaultNum];
  }
  ymuint fid = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    // 代表故障を等価故障のなかでもっとも出力よりの故障と定めるので
    // 出力側からのトポロジカル順に処理する．
    TpgNode* node = &mNodeArray[mNodeNum - i - 1];
    TpgFault* rep0 = NULL;
    TpgFault* rep1 = NULL;
    if ( node->fanout_num() == 1 ) {
      TpgNode* onode = node->fanout(0);
      // ちょっとかっこ悪い探し方
      ymuint ni = onode->fanin_num();
      ymuint ipos = ni;
      for (ymuint j = 0; j < ni; ++ j) {
	if ( onode->fanin(j) == node ) {
	  ipos = j;
	}
      }
      assert_cond( ipos < ni, __FILE__, __LINE__);
      rep0 = onode->input_fault(0, ipos);
      rep1 = onode->input_fault(1, ipos);
    }
    TpgFault* f0 = new_fault(node, true, 0, 0, rep0, fid);
    TpgFault* f1 = new_fault(node, true, 0, 1, rep1, fid);
    node->mOutputFault[0] = f0;
    node->mOutputFault[1] = f1;

    // ノードタイプから代表故障(r0, r1)をもとめる．
    get_rep_faults(node, f0, f1, rep0, rep1);

    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      node->mInputFault[j * 2 + 0] = new_fault(node, false, j, 0, rep0, fid);
      node->mInputFault[j * 2 + 1] = new_fault(node, false, j, 1, rep1, fid);
    }
  }
  assert_cond( fid == mFaultNum, __FILE__, __LINE__);

  // 代表故障のリストを作る．
  mRepFaultNum = 0;
  for (ymuint i = 0; i < mFaultNum; ++ i) {
    TpgFault* f = &mFaultChunk[i];
    if ( f->is_rep() ) {
      ++ mRepFaultNum;
    }
  }
  {
    void* p = mAlloc.get_memory(sizeof(TpgFault*) * mRepFaultNum);
    mRepFaultList = new (p) TpgFault*[mRepFaultNum];
  }
  ymuint pos = 0;
  for (ymuint i = 0; i < mFaultNum; ++ i) {
    TpgFault* f = &mFaultChunk[i];
    if ( f->is_rep() ) {
      mRepFaultList[pos] = f;
      ++ pos;
    }
  }
  assert_cond( pos == mRepFaultNum, __FILE__, __LINE__);

  // 全部アクティブにしておく．
  activate_all();

  // TFI のサイズの昇順に並べ方出力順を
  // mOutputArray2 に記録する．
  vector<pair<ymuint, ymuint> > tmp_list(output_num2());
  for (ymuint i = 0; i < output_num2(); ++ i) {
    TpgNode* onode = output(i);
    dfs_mark(onode);
    ymuint n = 0;
    for (ymuint j = 0; j < mNodeNum; ++ j) {
      if ( mTmpMark[j] ) {
	++ n;
      }
    }
    dfs_unmark(onode);
    tmp_list[i] = make_pair(n, i);
  }

  sort(tmp_list.begin(), tmp_list.end(), Lt());

  for (ymuint i = 0; i < output_num2(); ++ i) {
    mOutputArray2[i] = mOutputArray[tmp_list[i].second];
  }
}

// @brief 故障を生成する．
// @param[in] node 対象のノード
// @param[in] is_output 出力の故障の時 true にするフラグ
// @param[in] ipos 入力位置
// @param[in] val 故障値
// @param[in] rep_fault 代表故障
// @param[inout] fid 故障ID
// @note rep_fault が NULL の場合は自分自身が代表故障となる．
// @note この関数内で fid の値がインクリメントされる．
TpgFault*
TpgNetwork::new_fault(TpgNode* node,
		      bool is_output,
		      ymuint ipos,
		      int val,
		      TpgFault* rep_fault,
		      ymuint& fid)
{
  TpgFault* f = &mFaultChunk[fid];
  f->set(fid, node, is_output, ipos, val, rep_fault);
  ++ fid;
  return f;
}

// @brief デストラクタ
TpgNetwork::~TpgNetwork()
{
  // このオブジェクトが確保したメモリは mAlloc のデストラクタが
  // 開放してくれる．
}

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] po_pos 出力番号
void
TpgNetwork::activate_po(ymuint po_pos)
{
  TpgNode* onode = output2(po_pos);

  // pos 番めの出力から到達可能なノードにマークをつける．
  dfs_mark(onode);

  activate_sub();

  // マークを消す．
  dfs_unmark(onode);
}

// @brief 全てのノードをアクティブにする．
void
TpgNetwork::activate_all()
{
  // といっても PO に到達できないノードは除外する．

  // すべての PO から到達可能なノードにマークをつける．
  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    dfs_mark(output(i));
  }

  activate_sub();

  // マークを消す．
  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    dfs_unmark(output(i));
  }
}

// @brief ノードの TFI にマークをつける．
// @note 結果は mTmpMark[node->id()] に格納される．
void
TpgNetwork::dfs_mark(TpgNode* node)
{
  if ( mTmpMark[node->id()] ) {
    return;
  }

  mTmpMark[node->id()] = true;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    dfs_mark(node->fanin(i));
  }
}

// @brief ノードの TFI のマークを消す．
// @note 結果は mTmpMark[node->id()] に格納される．
void
TpgNetwork::dfs_unmark(TpgNode* node)
{
  if ( !mTmpMark[node->id()] ) {
    return;
  }

  mTmpMark[node->id()] = false;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    dfs_unmark(node->fanin(i));
  }
}

// @brief activate_po(), activate_all() の下請け関数
void
TpgNetwork::activate_sub()
{
  // マークにしたがってファンアウトなどの情報をセットする．
  mActNodeNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = &mNodeArray[i];
    if ( !mTmpMark[i] ) {
      node->clear_active();
      continue;
    }
    node->set_active();
    mActNodeArray[mActNodeNum] = node;
    ++ mActNodeNum;

    ymuint nfo = node->fanout_num();
    ymuint act_nfo = 0;
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* onode = node->fanout(i);
      if ( mTmpMark[onode->id()] ) {
	node->mActFanouts[act_nfo] = onode;
	++ act_nfo;
      }
    }
    node->mActFanoutNum = act_nfo;
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    TpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    if ( nfo == 0 ) {
      assert_cond( node->is_output(), __FILE__, __LINE__);
      node->mImmDom = NULL;
    }
    else {
      TpgNode* node0 = node->active_fanout(0);
      for (ymuint i = 1; i < nfo; ++ i) {
	node0 = merge(node0, node->active_fanout(i));
	if ( node0 == NULL ) {
	  break;
	}
      }
      node->mImmDom = node0;
    }
  }
}

// @brief ノードを得る．
TpgNode*
TpgNetwork::node(ymuint pos)
{
  // アドレス計算のために TpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief ノードを得る．
const TpgNode*
TpgNetwork::node(ymuint pos) const
{
  // アドレス計算のために TpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief 全故障数を返す．
ymuint
TpgNetwork::all_fault_num() const
{
  return mFaultNum;
}

// @brief 故障を取り出す．
// @param[in] id 故障番号 ( 0 <= id < all_fault_num() )
TpgFault*
TpgNetwork::fault(ymuint id)
{
  return &mFaultChunk[id];
}

// @brief 故障を取り出す．
// @param[in] id 故障番号 ( 0 <= id < all_fault_num() )
const TpgFault*
TpgNetwork::fault(ymuint id) const
{
  return &mFaultChunk[id];
}

// @brief 代表故障数を得る．
ymuint
TpgNetwork::rep_fault_num() const
{
  return mRepFaultNum;
}

// @brief 代表故障を取り出す．
// @param[in] pos 位置番号 ( 0 <= pos < rep_fault_num() )
TpgFault*
TpgNetwork::rep_fault(ymuint pos)
{
  assert_cond( pos < rep_fault_num(), __FILE__, __LINE__);
  return mRepFaultList[pos];
}

// @brief 代表故障を取り出す．
// @param[in] pos 位置番号 ( 0 <= pos < rep_fault_num() )
const TpgFault*
TpgNetwork::rep_fault(ymuint pos) const
{
  assert_cond( pos < rep_fault_num(), __FILE__, __LINE__);
  return mRepFaultList[pos];
}

// @brief TpgNode の内容を設定する．
// @param[in] id ID番号
// @param[in] tgnode もととなる TgNode
// @param[in] type ノードタイプ
// @param[in] sub_id サブID
TpgNode*
TpgNetwork::make_node(ymuint id,
		      const TgNode* tgnode)
{
  TpgNode* node = &mNodeArray[id];
  node->mId = id;

  const char* src_name = tgnode->name();
  if ( src_name != NULL ) {
    ymuint len = strlen(src_name) + 1;
    void* p = mAlloc.get_memory(sizeof(char) * len);
    char* dst_name = new (p) char[len];
    for (ymuint i = 0; i < len; ++ i) {
      dst_name[i] = src_name[i];
    }
    node->mName = dst_name;
  }
  else {
    node->mName = NULL;
  }

  mNodeMap[tgnode->gid()] = node;

  node->mCval = kB3X;
  node->mNval = kB3X;
  node->mMaVal = kB3X;

  if ( tgnode->is_input() ) {
    node->mTypeId = 1U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_output() ) {
    node->mTypeId = 2U | (tgnode->lid() << 2);
  }
  else if ( tgnode->is_logic() ) {
    node->mTypeId = 3U | (static_cast<ymuint>(tgnode->gate_type()) << 2);

    switch ( tgnode->gate_type() ) {
    case kTgGateAnd:
    case kTgGateNand:
      node->mCval = kB3False;
      node->mNval = kB3True;
      break;

    case kTgGateOr:
    case kTgGateNor:
      node->mCval = kB3True;
      node->mNval = kB3False;
      break;

    default:
      break;
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  node->mPrimitiveNum = 0;
  node->mPrimitiveList = NULL;

  ymuint ni = tgnode->fanin_num();
  node->mFaninNum = ni;
  if ( ni > 0 ) {
    node->mFanins = alloc_nodearray(mAlloc, ni);

    void* p = mAlloc.get_memory(sizeof(TpgFault*) * ni * 2);
    node->mInputFault = new (p) TpgFault*[ni * 2];
  }
  else {
    node->mFanins = NULL;
    node->mInputFault = NULL;
  }

  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* itgnode = tgnode->fanin(i);
    TpgNode* inode = mNodeMap[itgnode->gid()];
    node->mFanins[i] = inode;
  }

  ymuint no = tgnode->fanout_num();
  node->mFanoutNum = no;
  node->mFanouts = alloc_nodearray(mAlloc, no);
  node->mActFanoutNum = 0;
  node->mActFanouts = alloc_nodearray(mAlloc, no);

  for (ymuint i = 0; i < ni; ++ i) {
    node->mInputFault[i * 2 + 0] = NULL;
    node->mInputFault[i * 2 + 1] = NULL;
  }
  node->mOutputFault[0] = NULL;
  node->mOutputFault[1] = NULL;

  node->mMarks = 0U;

  return node;
}

// @brief 複雑な論理式に対応するプリミティブを作る．
// @param[in] expr 論理式
// @param[in] tgnode もととなる TgNode
// @param[in] primitive_list プリミティブを設定する領域
// @param[inout] id プリミティブID
// @note id は内部でインクリメントされる．
TpgPrimitive*
TpgNetwork::make_primitive(const LogExpr& expr,
			   const TgNode* tgnode,
			   TpgPrimitive* primitive_list,
			   ymuint& id)
{
  if ( expr.is_posiliteral() ) {
    // 肯定のリテラル
    ymuint ipos = expr.varid().val();
    TpgPrimitive* input = &primitive_list[id];
    ++ id;
    set_input_primitive(input, ipos);
    return input;
  }
  if ( expr.is_negaliteral() ) {
    // 否定のリテラル
    ymuint ipos = expr.varid().val();
    TpgPrimitive* input = &primitive_list[id];
    ++ id;
    set_not_input_primitive(input, ipos);
    return input;
  }

  ymuint nc = expr.child_num();
  vector<TpgPrimitive*> fanin(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    // 子供の論理式に対するプリミティブを作る．
    LogExpr expr1 = expr.child(i);
    TpgPrimitive* inode = make_primitive(expr1, tgnode, primitive_list, id);
    fanin[i] = inode;
  }

  TpgPrimitive* prim = &primitive_list[id];
  ++ id;

  tTgGateType gate_type;
  if ( expr.is_and() ) {
    gate_type = kTgGateAnd;
  }
  else if ( expr.is_or() ) {
    gate_type = kTgGateOr;
  }
  else if ( expr.is_xor() ) {
    gate_type = kTgGateXor;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  set_logic_primitive(prim, gate_type, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    prim->mFanins[i] = fanin[i];
  }
  return prim;
}

// @brief 入力プリミティブの設定を行なう．
// @param[in] prim プリミティブ
// @param[in] id 入力番号
void
TpgNetwork::set_input_primitive(TpgPrimitive* prim,
				ymuint id)
{
  prim->mTypeId = 0U | (id << 2);
  prim->mFaninNum = 0;
  prim->mFanins = NULL;
}

// @brief 否定付き入力プリミティブの設定を行なう．
// @param[in] prim プリミティブ
// @param[in] id 入力番号
void
TpgNetwork::set_not_input_primitive(TpgPrimitive* prim,
				    ymuint id)
{
  prim->mTypeId = 1U | (id << 2);
  prim->mFaninNum = 0;
  prim->mFanins = NULL;
}

// @brief 論理プリミティブの設定を行なう．
// @param[in] prim プリミティブ
// @param[in] gate_type ゲートタイプ
// @param[in] ni 入力数
void
TpgNetwork::set_logic_primitive(TpgPrimitive* prim,
				tTgGateType gate_type,
				ymuint ni)
{
  prim->mTypeId = 2U | (static_cast<ymuint32>(gate_type) << 2);
  prim->mFaninNum = ni;
  void* p = mAlloc.get_memory(sizeof(TpgPrimitive*) * ni);
  prim->mFanins = new (p) TpgPrimitive*[ni];
}


//////////////////////////////////////////////////////////////////////
// 必要割り当て関係のコード
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// node のファンアウトに mark1 をつける．
void
set_tfo_mark(TpgNode* node)
{
  if ( node->mark1() ) {
    return;
  }
  node->set_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* onode = node->active_fanout(i);
    set_tfo_mark(onode);
  }
}

// node のファンアウトに mark1 をつける．
void
clear_tfo_mark(TpgNode* node)
{
  if ( !node->mark1() ) {
    return;
  }
  node->clear_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* onode = node->active_fanout(i);
    clear_tfo_mark(onode);
  }
}

END_NONAMESPACE

// @brief f の検出に必要な割り当てを求める．
// @param[in] f 対象の故障
// @param[in] ma_list 割り当て結果を格納するリスト
// @return 矛盾が生じたら(fが冗長故障の場合) false を返す．
// @note TpgNetwork のメンバにはアクセスしないので static メンバになっている．
// @note ma_list の内容は TpgNode::id() * 2 + val (0 / 1)
bool
TpgNetwork::get_mandatory_assignment(TpgFault* f,
				     vector<ymuint32>& ma_list)
{
  TpgNode* fnode = f->node();
  TpgNode* fsrc = fnode;
  if ( f->is_input_fault() ) {
    fsrc = fnode->fanin(f->pos());
  }

  vector<TpgNode*> node_list;
  Bool3 gval;
  if ( f->val() == 1 ) {
    gval = kB3False;
  }
  else {
    gval = kB3True;
  }
  bool stat = fsrc->bwd_prop(NULL, gval, node_list);
  if ( !stat ) {
    goto untestable;
  }

  if ( f->is_input_fault() ) {
    Bool3 nval = fnode->nval();
    if ( nval != kB3X ) {
      ymuint ni = fnode->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	if ( i == f->pos() ) continue;
	TpgNode* inode = fnode->fanin(i);
	if ( !inode->bwd_prop(fnode, nval, node_list) ) {
	  goto untestable;
	}
      }
    }
  }
  if ( fnode->imm_dom() != NULL ) {
    set_tfo_mark(fnode);
    for (TpgNode* dom = fnode->imm_dom(); dom != NULL; dom = dom->imm_dom()) {
      Bool3 nval = dom->nval();
      if ( nval != kB3X ) {
	ymuint ni = dom->fanin_num();
	for (ymuint i = 0; i < ni; ++ i) {
	  TpgNode* inode = dom->fanin(i);
	  if ( inode->mark1() ) continue;
	  if ( !inode->bwd_prop(dom, nval, node_list) ) {
	    clear_tfo_mark(fnode);
	    goto untestable;
	  }
	}
      }
    }
    clear_tfo_mark(fnode);
  }

  for (vector<TpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    TpgNode* node = *p;
    assert_cond( node->ma_value() != kB3X, __FILE__, __LINE__);
    int val = node->ma_value() == kB3True ? 1 : 0;
    ma_list.push_back(node->id() * 2 + val);
    node->clear_ma_value();
  }
  sort(ma_list.begin(), ma_list.end());
  return true;

 untestable:
  for (vector<TpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_ma_value();
  }
  return false;
}

END_NAMESPACE_YM_SATPG

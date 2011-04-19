#ifndef LIBYM_BDN_BDNMGRIMPL_H
#define LIBYM_BDN_BDNMGRIMPL_H

/// @file libym_bdn/BdnMgrImpl.h
/// @brief BdnMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "ym_bdn/BdnMgr.h"
#include "ym_bdn/BdnNodeHandle.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnMgrImpl BdnMgrImpl.h BdnMgrImp.h
/// @brief BdnMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class BdnMgrImpl
{
public:

  /// @brief コンストラクタ
  /// @note 空のネットワークに初期化される．
  BdnMgrImpl();

  /// @brief デストラクタ
  ~BdnMgrImpl();


public:

  /// @brief 名前の取得
  string
  name() const;

  /// @brief ポート数の取得
  ymuint
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  const BdnPort*
  port(ymuint pos) const;

  /// @brief D-FF 数の取得
  ymuint
  dff_num() const;

  /// @brief D-FF のリストの取得
  const BdnDffList&
  dff_list() const;

  /// @brief ラッチ数の取得
  ymuint
  latch_num() const;

  /// @brief ラッチのリストの取得
  const BdnLatchList&
  latch_list() const;


public:

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  ymuint
  max_node_id() const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 入力ノードのリストを得る．
  const BdnNodeList&
  input_list() const;

  /// @brief 出力ノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力ノードのリストを得る．
  const BdnNodeList&
  output_list() const;

  /// @brief 入出力ノードのリストを得る．
  const BdnNodeList&
  inout_list() const;

  /// @brief 論理ノード数を得る．
  ymuint
  lnode_num() const;

  /// @brief 論理ノードのリストを得る．
  const BdnNodeList&
  lnode_list() const;

  /// @brief ソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  sort(vector<BdnNode*>& node_list) const;

  /// @brief 逆順でソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  rsort(vector<BdnNode*>& node_list) const;

  /// @brief 最大段数を求める．
  /// @note 段数とは入力ノードから出力ノードへ至る経路中の論理ノードの数
  ymuint
  level() const;


public:

  /// @brief 名前を設定する．
  /// @param[in] name 新しい名前
  void
  set_name(const string& name);

  /// @brief コピーコンストラクタと代入演算子用のサブルーティン
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const BdnMgr& src);

  /// @brief 空にする．
  void
  clear();

  /// @brief どこにもファンアウトしていないノードを削除する．
  void
  clean_up();

  /// @brief ポートを作る．
  /// @param[in] name 名前
  /// @param[in] bit_width ビット幅
  BdnPort*
  new_port(const string& name,
	   ymuint bit_width);

  /// @brief D-FF を作る．
  /// @param[in] name 名前
  /// @return 生成されたD-FFを返す．
  BdnDff*
  new_dff(const string& name = string());

  /// @brief ラッチを作る．
  /// @param[in] name 名前
  /// @return 生成されたラッチを返す．
  BdnLatch*
  new_latch(const string& name = string());

  /// @brief 外部入力を作る．
  /// @param[in] port ポート
  /// @param[in] bitpos ビット位置
  /// @return 作成したノードを返す．
  /// @note エラー条件は以下の通り
  ///  - bitpos が port のビット幅を越えている．
  ///  - port の bitpos にすでにノードがある．
  BdnNode*
  new_port_input(BdnPort* port,
		 ymuint bitpos);

  /// @brief 外部出力ノードを作る．
  /// @param[in] port ポート
  /// @param[in] bitpos ビット位置
  /// @return 作成したノードを返す．
  /// @note エラー条件は以下の通り
  ///  - bitpos が port のビット幅を越えている．
  ///  - port の bitpos にすでにノードがある．
  BdnNode*
  new_port_output(BdnPort* port,
		  ymuint bitpos);


public:

  /// @brief 論理ノードの内容を設定する．
  /// @param[in] node 設定するノード
  /// @param[in] fcode 機能コード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return ノード＋極性を返す．
  /// @note fcode の各ビットの意味は以下のとおり，
  ///  - 0bit: ファンイン0の反転属性
  ///  - 1bit: ファンイン1の反転属性
  ///  - 2bit: XOR/AND フラグ( 0: AND, 1: XOR)
  ///  - 3bit: 出力の反転属性
  /// @note すでに構造的に同じノードがあればそれを返す．
  /// @note なければ node に設定する．
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  set_logic(BdnNode* node,
	    ymuint fcode,
	    BdnNodeHandle inode1_handle,
	    BdnNodeHandle inode2_handle);

  /// @brief 論理ノードの内容を変更する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] new_handle 設定する新しいハンドル
  /// @note node のファンアウト先の情報を書き換える．
  void
  change_logic(BdnNode* node ,
	       BdnNodeHandle new_handle);

  /// @brief 出力ノードのファンインを変更する
  /// @param[in] node 変更対象の出力ノード
  /// @param[in] inode_handle ファンインのノード+極性
  void
  change_output_fanin(BdnNode* node,
		      BdnNodeHandle inode_handle);

  /// @brief ノード間の接続を変更する．
  /// @param[in] from 接続の入力側のノード
  /// @param[in] to 接続の出力側のノード
  /// @param[in] pos to のファンイン番号
  /// @note to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(BdnNode* from,
	  BdnNode* to,
	  ymuint pos);

  /// @brief D-FF を削除する．
  /// @param[in] dff 削除対象の D-FF
  void
  delete_dff(BdnDff* dff);

  /// @brief ラッチを削除する．
  /// @param[in] latch 削除対象のラッチ
  void
  delete_latch(BdnLatch* latch);

  /// @brief ノードを作成する．
  /// @return 作成されたノードを返す．
  BdnNode*
  alloc_node();

  /// @brief node を削除する．
  /// @param[in] node 削除対象のノード
  void
  delete_node(BdnNode* node);

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 確保するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // ネットワーク名
  string mName;

  // ID 番号をキーにしたポートの配列
  vector<BdnPort*> mPortArray;

  // D-FF の ID 番号を管理するためのオブジェクト
  ItvlMgr mDffItvlMgr;

  // ID 番号をキーにした DFF の配列
  vector<BdnDff*> mDffArray;

  // D-FFノードのリスト
  BdnDffList mDffList;

  // ラッチの ID番号を管理するためのオブジェクト
  ItvlMgr mLatchItvlMgr;

  // ID番号をキーにしたラッチの配列
  vector<BdnLatch*> mLatchArray;

  // ラッチのリスト
  BdnLatchList mLatchList;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mNodeItvlMgr;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<BdnNode*> mNodeArray;

  // 入力ノードのリスト
  BdnNodeList mInputList;

  // 出力ノードのリスト
  BdnNodeList mOutputList;

  // 論理ノードのリスト
  BdnNodeList mLnodeList;

  // 論理ノードのハッシュ表
  BdnNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // 最大レベル (最下位ビットは valid フラグ)
  mutable
  ymuint32 mLevel;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス BdnMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief 名前を得る．
inline
string
BdnMgrImpl::name() const
{
  return mName;
}

// @brief 名前を設定する．
inline
void
BdnMgrImpl::set_name(const string& name)
{
  mName = name;
}

// @brief ポート数の取得
inline
ymuint
BdnMgrImpl::port_num() const
{
  return mPortArray.size();
}

// @brief ポートの取得
// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
inline
const BdnPort*
BdnMgrImpl::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief D-FF 数の取得
inline
ymuint
BdnMgrImpl::dff_num() const
{
  return mDffList.size();
}

// @brief D-FF のリストの取得
inline
const BdnDffList&
BdnMgrImpl::dff_list() const
{
  return mDffList;
}

// @brief ラッチ数の取得
inline
ymuint
BdnMgrImpl::latch_num() const
{
  return mLatchList.size();
}

// @brief ラッチのリストの取得
inline
const BdnLatchList&
BdnMgrImpl::latch_list() const
{
  return mLatchList;
}

// ノード番号の最大値 + 1 を返す．
inline
ymuint
BdnMgrImpl::max_node_id() const
{
  return mNodeArray.size();
}

// @brief 入力ノード数の取得
// @return 入力ノード数を返す．
inline
ymuint
BdnMgrImpl::input_num() const
{
  return mInputList.size();
}

// @brief 入力ノードのリストを得る．
inline
const BdnNodeList&
BdnMgrImpl::input_list() const
{
  return mInputList;
}

// @brief 出力ノード数を得る．
inline
ymuint
BdnMgrImpl::output_num() const
{
  return mOutputList.size();
}

// @brief 出力ノードのリストを得る．
inline
const BdnNodeList&
BdnMgrImpl::output_list() const
{
  return mOutputList;
}

// 論理ノード数を得る．
inline
ymuint
BdnMgrImpl::lnode_num() const
{
  return mLnodeList.size();
}

// 論理ノードのリストを得る．
inline
const BdnNodeList&
BdnMgrImpl::lnode_list() const
{
  return mLnodeList;
}

END_NAMESPACE_YM_BDN

#endif // LIBYM_BDN_BDNMGRIMPL_H
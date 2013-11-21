#ifndef SATPGNODE_H
#define SATPGNODE_H

/// @file SatpgNode.h
/// @brief SatpgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "ym_networks/tgnet.h"
#include "ym_logic/VarId.h"
#include "ym_logic/Bool3.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class SatpgNode SatpgNode.h "SatpgNode.h"
/// @brief SATPG 用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class SatpgNode
{
  friend class SatpgNetwork;

private:

  /// @brief コンストラクタ
  SatpgNode();

  /// @brief デストラクタ
  ~SatpgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 外部入力タイプの時 true を返す．
  /// @note FF 出力もここに含まれる．
  bool
  is_input() const;

  /// @brief 外部入力タイプの時に入力番号を返す．
  ymuint
  input_id() const;

  /// @brief 外部出力タイプの時 true を返す．
  /// @note FF 入力もここに含まれる．
  bool
  is_output() const;

  /// @brief 外部出力タイプの時に出力番号を返す．
  ymuint
  output_id() const;

  /// @brief logic タイプの時 true を返す．
  bool
  is_logic() const;

  /// @brief ゲートタイプを得る．
  tTgGateType
  gate_type() const;

  /// @brief 組み込み型でない logic タイプ (cplx_logic タイプ)の時 true を返す．
  bool
  is_cplx_logic() const;

  /// @brief cplx_logic タイプのときにプリミティブ数を返す．
  ymuint
  primitive_num() const;

  /// @brief cplx_logic タイプのときにプリミティブを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < primitive_num() )
  DtpgPrimitive*
  primitive(ymuint pos) const;

  /// @brief ファンイン数を得る．
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  SatpgNode*
  fanin(ymuint pos) const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
  SatpgNode*
  fanout(ymuint pos) const;

  /// @brief アクティブなファンアウト数を得る．
  ymuint
  active_fanout_num() const;

  /// @brief アクティブなファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
  SatpgNode*
  active_fanout(ymuint pos) const;

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  DtpgFault*
  output_fault(int val);

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  DtpgFault*
  input_fault(int val,
	      ymuint pos);

  /// @brief アクティブの場合 true を返す．
  bool
  is_active() const;

  /// @brief 直近の dominator を得る．
  SatpgNode*
  imm_dom() const;


public:
  //////////////////////////////////////////////////////////////////////
  // SatEngine 用のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常回路用の変数番号をセットする．
  /// @param[in] gvar 正常値を表す変数番号
  void
  set_gvar(VarId gvar);

  /// @brief 故障回路用の変数番号をセットする．
  /// @param[in] fvar 故障値を表す変数番号
  /// @param[in] dvar 故障差(正常値 xor 故障値)を表す変数番号
  void
  set_fvar(VarId fvar,
	   VarId dvar);

  /// @brief 変数番号の割り当て情報をクリアする．
  void
  clear_var();

  /// @brief 正常値を表す変数番号を得る．
  VarId
  gvar() const;

  /// @brief 故障回路用の変数番号が割り当てられていたら true を返す．
  bool
  has_fvar() const;

  /// @brief 故障値を表す変数番号を得る．
  VarId
  fvar() const;

  /// @brief 故障差を表す変数番号を得る．
  VarId
  dvar() const;

  /// @brief mark1 を得る．
  bool
  mark1() const;

  /// @brief mark1 をつける．
  void
  set_mark1();

  /// @brief mark1 を消す．
  void
  clear_mark1();

  /// @brief mark2 を得る．
  bool
  mark2() const;

  /// @brief mark2 をつける．
  void
  set_mark2();

  /// @brief mark2 を消す．
  void
  clear_mark2();

  /// @brief mark3 を得る．
  bool
  mark3() const;

  /// @brief mark3 をつける．
  void
  set_mark3();

  /// @brief mark3 を消す．
  void
  clear_mark3();


public:
  //////////////////////////////////////////////////////////////////////
  // mandatory assignment 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後方含意で出力に0を割り当てる．
  /// @param[in] from_node 含意元のノード
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop(SatpgNode* from_node,
	   Bool3 val,
	   vector<SatpgNode*>& node_list);

  /// @brief ファンアウト先に0を伝播する．
  /// @param[in] from_node 含意元のノード
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanout_prop(SatpgNode* from_node,
	      Bool3 val,
	      vector<SatpgNode*>& node_list);

  /// @brief controling value を得る．
  /// @note ない場合は kB3X を返す．
  Bool3
  cval() const;

  /// @brief noncontroling valueを得る．
  /// @note ない場合は kB3X を返す．
  Bool3
  nval() const;

  /// @brief 値を得る．
  Bool3
  ma_value() const;

  /// @brief 値をクリアする．
  void
  clear_ma_value();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アクティブにする．
  void
  set_active();

  /// @brief アクティブフラグを消す．
  void
  clear_active();

  /// @brief 後方含意を行う．
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_imp(Bool3 val,
	  vector<SatpgNode*>& node_list);

  /// @brief 前方含意を行う．
  /// @param[in] from_node 含意元のノード
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fwd_imp(SatpgNode* from_node,
	  Bool3 val,
	  vector<SatpgNode*>& node_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // いくつかのデータをパックしたもの
  // - [0:1] ノードタイプ
  //   0: 未使用
  //   1: 外部入力
  //   2: 外部出力
  //   3: 論理ノード
  // - [2:31] 入力/出力ノードの場合の通し番号
  //          or ゲートタイプ
  ymuint32 mTypeId;

  // プリミティブ数
  ymuint32 mPrimitiveNum;

  // プリミティブのリスト
  // 入力からのトポロジカル順に格納する．
  DtpgPrimitive* mPrimitiveList;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  SatpgNode** mFanins;

  // ファンアウト数
  ymuint32 mFanoutNum;

  // ファンアウトの配列
  SatpgNode** mFanouts;

  // アクティブなファンアウト数
  ymuint32 mActFanoutNum;

  // アクティブなファンアウトの配列
  SatpgNode** mActFanouts;

  // 出力の故障
  DtpgFault* mOutputFault[2];

  // 入力の故障
  DtpgFault** mInputFault;

  // いくつかのマークを納めるビットベクタ
  ymuint32 mMarks;

  // 正常回路の変数番号
  VarId mGid;

  // 故障回路の変数番号
  VarId mFid;

  // 故障差の変数番号
  VarId mDid;

  // immediate dominator
  SatpgNode* mImmDom;

  // controling value
  Bool3 mCval;

  // noncontroling value
  Bool3 mNval;

  // mandatory assignments 用の値
  Bool3 mMaVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
SatpgNode::SatpgNode()
{
  // 実際の初期化は SatpgNetwork が行なう．
}

// @brief デストラクタ
inline
SatpgNode::~SatpgNode()
{
}

// @brief ID番号を得る．
inline
ymuint
SatpgNode::id() const
{
  return mId;
}

// @brief 外部入力タイプの時 true を返す．
// @note FF 出力もここに含まれる．
inline
bool
SatpgNode::is_input() const
{
  return (mTypeId & 3U) == 1U;
}

// @brief 外部入力タイプの時に入力番号を返す．
inline
ymuint
SatpgNode::input_id() const
{
  assert_cond( is_input(), __FILE__, __LINE__);
  return (mTypeId >> 2);
}

// @brief 外部出力タイプの時 true を返す．
// @note FF 入力もここに含まれる．
inline
bool
SatpgNode::is_output() const
{
  return (mTypeId & 3U) == 2U;
}

// @brief 外部出力タイプの時に出力番号を返す．
inline
ymuint
SatpgNode::output_id() const
{
  assert_cond( is_output(), __FILE__, __LINE__);
  return (mTypeId >> 2);
}

// @brief logic タイプの時 true を返す．
inline
bool
SatpgNode::is_logic() const
{
  return (mTypeId & 3U) == 3U;
}

// @brief ゲートタイプを得る．
inline
tTgGateType
SatpgNode::gate_type() const
{
  assert_cond( is_logic(), __FILE__, __LINE__);
  return static_cast<tTgGateType>((mTypeId >> 2) & 15U);
}

// @brief 組み込み型でない logic タイプの時 true を返す．
inline
bool
SatpgNode::is_cplx_logic() const
{
  return is_logic() && gate_type() == kTgGateCplx;
}

// @brief cplx_logic タイプのときにプリミティブ数を返す．
inline
ymuint
SatpgNode::primitive_num() const
{
  return mPrimitiveNum;
}

// @brief ファンイン数を得る．
inline
ymuint
SatpgNode::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
inline
SatpgNode*
SatpgNode::fanin(ymuint pos) const
{
  assert_cond( pos < mFaninNum, __FILE__, __LINE__);
  return mFanins[pos];
}

// @brief ファンアウト数を得る．
inline
ymuint
SatpgNode::fanout_num() const
{
  return mFanoutNum;
}

// @brief ファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
inline
SatpgNode*
SatpgNode::fanout(ymuint pos) const
{
  assert_cond( pos < mFanoutNum, __FILE__, __LINE__);
  return mFanouts[pos];
}

// @brief アクティブなファンアウト数を得る．
inline
ymuint
SatpgNode::active_fanout_num() const
{
  return mActFanoutNum;
}

// @brief アクティブなファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
inline
SatpgNode*
SatpgNode::active_fanout(ymuint pos) const
{
  assert_cond( pos < mActFanoutNum, __FILE__, __LINE__);
  return mActFanouts[pos];
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
inline
DtpgFault*
SatpgNode::output_fault(int val)
{
  return mOutputFault[val % 2];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
inline
DtpgFault*
SatpgNode::input_fault(int val,
		      ymuint pos)
{
  assert_cond( pos < mFaninNum, __FILE__, __LINE__);
  return mInputFault[pos * 2 + (val % 2)];
}

// @brief mark1 を得る．
inline
bool
SatpgNode::mark1() const
{
  return static_cast<bool>((mMarks >> 3) & 1U);
}

// @brief mark1 をつける．
inline
void
SatpgNode::set_mark1()
{
  mMarks |= 8U;
}

// @brief mark1 を消す．
inline
void
SatpgNode::clear_mark1()
{
  mMarks &= ~8U;
}

// @brief mark2 を得る．
inline
bool
SatpgNode::mark2() const
{
  return static_cast<bool>((mMarks >> 4) & 1U);
}

// @brief mark2 をつける．
inline
void
SatpgNode::set_mark2()
{
  mMarks |= 16U;
}

// @brief mark2 を消す．
inline
void
SatpgNode::clear_mark2()
{
  mMarks &= ~16U;
}

// @brief mark3 を得る．
inline
bool
SatpgNode::mark3() const
{
  return static_cast<bool>((mMarks >> 5) & 1U);
}

// @brief mark3 をつける．
inline
void
SatpgNode::set_mark3()
{
  mMarks |= 32U;
}

// @brief mark3 を消す．
inline
void
SatpgNode::clear_mark3()
{
  mMarks &= ~32U;
}

// @brief アクティブの場合 true を返す．
inline
bool
SatpgNode::is_active() const
{
  return static_cast<bool>(mMarks & 1U);
}

// @brief アクティブにする．
inline
void
SatpgNode::set_active()
{
  mMarks |= 1U;
}

// @brief アクティブフラグを消す．
inline
void
SatpgNode::clear_active()
{
  mMarks &= ~1U;
}

// @brief 正常回路用の変数番号をセットする．
// @param[in] gvar 正常値を表す変数番号
inline
void
SatpgNode::set_gvar(VarId gvar)
{
  mGid = gvar;
  mFid = gvar;
  mMarks |= 2U;
}

// @brief 故障回路用の変数番号をセットする．
// @param[in] fvar 故障値を表す変数番号
// @param[in] dvar 故障差(正常値 xor 故障値)を表す変数番号
inline
void
SatpgNode::set_fvar(VarId fvar,
		   VarId dvar)
{
  mFid = fvar;
  mDid = dvar;
  mMarks |= 4U;
}

// @brief 変数番号の割り当て情報をクリアする．
inline
void
SatpgNode::clear_var()
{
  mMarks &= ~6U;
}

// @brief 正常回路用の変数番号が割り当てられていたら true を返す．
inline
bool
SatpgNode::has_gvar() const
{
  return static_cast<bool>((mMarks >> 1) & 1U);
}


// @brief 正常値を表す変数番号を得る．
inline
VarId
SatpgNode::gvar() const
{
  return mGid;
}

// @brief 故障回路用の変数番号が割り当てられていたら true を返す．
inline
bool
SatpgNode::has_fvar() const
{
  return static_cast<bool>((mMarks >> 2) & 1U);
}

// @brief 故障値を表す変数番号を得る．
inline
VarId
SatpgNode::fvar() const
{
  return mFid;
}

// @brief 故障差を表す変数番号を得る．
inline
VarId
SatpgNode::dvar() const
{
  return mDid;
}

// @brief 直近の dominator を得る．
inline
SatpgNode*
SatpgNode::imm_dom() const
{
  return mImmDom;
}

// @brief controling value を得る．
// @note ない場合は kB3X を返す．
inline
Bool3
SatpgNode::cval() const
{
  return mCval;
}

// @brief noncontroling valueを得る．
// @note ない場合は kB3X を返す．
inline
Bool3
SatpgNode::nval() const
{
  return mNval;
}

// @brief 値を得る．
inline
Bool3
SatpgNode::ma_value() const
{
  return mMaVal;
}

// @brief 値をクリアする．
inline
void
SatpgNode::clear_ma_value()
{
  mMaVal = kB3X;
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // SATPGNODE_H

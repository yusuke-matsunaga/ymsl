﻿#ifndef MINPATIMPL_H
#define MINPATIMPL_H

/// @file MinPatImpl.h
/// @brief MinPatImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat.h"
#include "FaultInfo.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatImpl MinPatImpl.h "MinPatImpl.h"
/// @brief MinPat の実装クラス
//////////////////////////////////////////////////////////////////////
class MinPatImpl :
  public MinPat
{
public:

  /// @brief コンストラクタ
  MinPatImpl();

  /// @brief デストラクタ
  virtual
  ~MinPatImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストベクタの最小化を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  /// @param[out] tv_list テストベクタのリスト
  /// @param[out] stats 実行結果の情報を格納する変数
  virtual
  void
  run(TpgNetwork& network,
      TvMgr& tvmgr,
      FaultMgr& fmgr,
      Fsim& fsim2,
      vector<TestVector*>& tv_list,
      MinPatStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 支配故障を求める．
  void
  get_dom_faults(const vector<TpgFault*>& fault_list,
		 vector<TpgFault*>& dom_fault_list);

  void
  analyze_faults(const vector<TpgFault*> fault_list);

  /// @brief 故障間の衝突性を調べる．
  void
  analyze_conflict(const vector<TpgFault*>& fault_list,
		   vector<pair<ymuint, ymuint> >& edge_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最大ノード番号
  ymuint mMaxNodeId;

  // 最大故障番号
  ymuint mMaxFaultId;

  // ノードごとに関係する入力の番号のリストを収める配列
  vector<vector<ymuint> > mInputListArray;

  // 故障ごとの情報を収める配列
  vector<FaultInfo> mFaultInfoArray;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATIMPL_H

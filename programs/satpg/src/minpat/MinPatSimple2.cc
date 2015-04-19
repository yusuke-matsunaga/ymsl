﻿
/// @file MinPatSimple2.cc
/// @brief MinPatSimple2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatSimple2.h"
#include "TpgFault.h"
#include "FaultAnalyzer.h"
#include "DomChecker.h"
#include "ConflictChecker.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
// @param[in] group_dominance グループ支配を計算する．
MinPat*
new_MinPatSimple2(bool group_dominance)
{
  return new MinPatSimple2(group_dominance);
}

BEGIN_NONAMESPACE

struct FaultLt
{
  bool
  operator()(const pair<double, TpgFault*>& left,
	     const pair<double, TpgFault*>& right)
  {
    return left.first > right.first;
  }
};

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス MinPatSimple2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] group_dominance グループ支配を計算する．
MinPatSimple2::MinPatSimple2(bool group_dominance) :
  MinPatNaive(group_dominance)
{
}

// @brief デストラクタ
MinPatSimple2::~MinPatSimple2()
{
}

// @brief 初期化を行う．
// @param[in] fault_list 検出された故障のリスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
void
MinPatSimple2::init(const vector<TpgFault*>& fault_list,
		    TvMgr& tvmgr,
		    Fsim& fsim2)
{
  DomChecker checker(analyzer(), tvmgr, fsim2);

  vector<TpgFault*> dom_fault_list;
  checker.get_dom_faults(dom_method(), fault_list, dom_fault_list);

  ymuint nf = dom_fault_list.size();

  // 故障を衝突数の多い順に並べる．
  ConflictChecker checker2(analyzer(), tvmgr, fsim2);

  ymuint sample_num = 1000;
  vector<double> conf_prob_array;
  checker2.estimate_conflict(dom_fault_list, sample_num, conf_prob_array);
  vector<pair<double, TpgFault*> > tmp_list(nf);
  vector<TpgFault*> tmp_list2(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* f = dom_fault_list[i];
    double cnum = conf_prob_array[f->id()];
    tmp_list[i] = make_pair(cnum, f);
  }
  sort(tmp_list.begin(), tmp_list.end(), FaultLt());
  for (ymuint i = 0; i < nf; ++ i) {
    tmp_list2[i] = tmp_list[i].second;
  }
  set_fault_list(tmp_list2);

}

END_NAMESPACE_YM_SATPG

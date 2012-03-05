
/// @file SatImp.cc
/// @brief SatImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "SatImp.h"
#include "StrNode.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnXor.h"
#include "ImpInfo.h"
#include "ImpList.h"
#include "ImpMgr.h"
#include "StrImp.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"
#include "ym_logic/SatSolver.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

bool debug = true;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス SatImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SatImp::SatImp()
{
}

// @brief デストラクタ
SatImp::~SatImp()
{
}

BEGIN_NONAMESPACE
void
check(SatSolver& solver,
      ymuint src_id,
      ymuint src_val,
      ymuint src_bv,
      ymuint dst_id,
      ymuint dst_val,
      ymuint dst_bv)
{
  ymuint val0 = (src_val == 1) ? src_bv : ~src_bv;
  ymuint val1 = (dst_val == 1) ? dst_bv : ~dst_bv;
  if ( (val0 & ~val1) != 0UL ) {
    cerr << "Error(bitval): Node#" << src_id << ": " << src_val
	 << " -> Node#" << dst_id << ": " << dst_val << endl;
  }
  vector<Literal> tmp(2);
  vector<Bool3> model;
  Literal lit0(VarId(src_id), src_val ? kPolPosi : kPolNega);
  Literal lit1(VarId(dst_id), dst_val ? kPolPosi : kPolNega);
  tmp[0] = lit0;
  tmp[1] = ~lit1;
  if ( solver.solve(tmp, model) != kB3False ) {
    cerr << "Error(sat): Node#" << src_id << ": " << src_val
	 << " -> Node#" << dst_id << ": " << dst_val << endl;
  }
}

ymuint
count_list(const vector<list<ImpVal> >& cand_info)
{
  ymuint c = 0;
  for (vector<list<ImpVal> >::const_iterator p = cand_info.begin();
       p != cand_info.end(); ++ p) {
    const list<ImpVal>& imp_list = *p;
    c += imp_list.size();
  }
  return c;
}

void
mark_tfi(StrNode* node,
	 vector<bool>& mark,
	 vector<ymuint>& input_list)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  if ( node->is_input() ) {
    input_list.push_back(node->id());
  }
  else {
    mark_tfi(node->fanin0().src_node(), mark, input_list);
    mark_tfi(node->fanin1().src_node(), mark, input_list);
  }
}

bool
check_intersect(const vector<ymuint>& list1,
		const vector<ymuint>& list2)
{
  ymuint n1 = list1.size();
  ymuint n2 = list2.size();
  ymuint i1 = 0;
  ymuint i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    if ( list1[i1] < list2[i2] ) {
      ++ i1;
    }
    else if ( list1[i1] > list2[i2] ) {
      ++ i2;
    }
    else {
      return true;
    }
  }
  return false;
}

void
print_fanin(ostream& s,
	    const StrEdge& e)
{
  if ( e.src_inv() ) {
    s << "~";
  }
  s << e.src_node()->id();
}

END_NONAMESPACE


// @brief ネットワーク中の間接含意を求める．
// @param[in] network 対象のネットワーク
// @param[in] imp_info 間接含意のリスト
void
SatImp::learning(const BdnMgr& network,
		 const ImpInfo& d_imp,
		 ImpInfo& imp_info)
{
  ymuint n = network.max_node_id();

  imp_info.set_size(n);

  // BDN の情報を ImpMgr にコピーする．
  ImpMgr imp_mgr;
  imp_mgr.set(network);

  SatSolver solver;
  for (ymuint i = 0; i < n; ++ i) {
    VarId vid = solver.new_var();
    assert_cond( vid.val() == i, __FILE__, __LINE__);
  }

  // BDN から CNF を作る．
  vector<BdnNode*> node_list;
  network.sort(node_list);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    Literal lit(VarId(id), kPolPosi);

    const BdnNode* bnode0 = bnode->fanin0();
    bool inv0 = bnode->fanin0_inv();
    Literal lit0(VarId(bnode0->id()), inv0 ? kPolNega : kPolPosi);

    const BdnNode* bnode1 = bnode->fanin1();
    bool inv1 = bnode->fanin1_inv();
    Literal lit1(VarId(bnode1->id()), inv1 ? kPolNega : kPolPosi);

    if ( bnode->is_and() ) {
      solver.add_clause(lit0, ~lit);
      solver.add_clause(lit1, ~lit);
      solver.add_clause(~lit0, ~lit1, lit);
    }
    else if ( bnode->is_xor() ) {
      solver.add_clause(lit0, ~lit1, lit);
      solver.add_clause(~lit0, lit1, lit);
      solver.add_clause(~lit0, ~lit1, ~lit);
      solver.add_clause(lit0, lit1, ~lit);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }

  if ( debug ) {
    cerr << "CNF generated" << endl;
  }

#if 1
  imp_mgr.print_network(cout);
#endif

  StopWatch timer;
  timer.start();

  // 各ノードから到達可能な入力ノードのリストを求める．
  vector<vector<ymuint> > input_list_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node0 = imp_mgr.node(i);
    if ( node0 == NULL ) continue;

    vector<bool> tfi_mark(n, false);
    mark_tfi(node0, tfi_mark, input_list_array[i]);
    sort(input_list_array[i].begin(), input_list_array[i].end());
  }

  // 直接含意と対偶の含意をコピーしておく
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    StrNode* node0 = imp_mgr.node(src_id);
    if ( node0 == NULL ) continue;

    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      const ImpList& imp_list = d_imp.get(src_id, src_val);
      for (ImpList::iterator p = imp_list.begin(); p != imp_list.end(); ++ p) {
	const ImpCell& cell = *p;
	ymuint dst_id = cell.dst_id();
	ymuint dst_val = cell.dst_val();
	imp_info.put(src_id, src_val, dst_id, dst_val);
	if ( !d_imp.check(dst_id, dst_val ^ 1, src_id, src_val ^ 1) ) {
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	}
      }
    }
  }

  // シミュレーションでフィルタリングして残った候補を
  // SAT で調べる．
  imp_mgr.random_sim();
  vector<list<ImpVal> > cand_info(n * 2);
  vector<ymuint32> const_flag(n, 0U);
  for (ymuint i = 0; i < n; ++ i) {
    if ( debug ) {
      if ( (i % 100) == 0 ) {
	cerr << i << " / " << n << endl;
      }
    }
    StrNode* node0 = imp_mgr.node(i);
    if ( node0 == NULL ) continue;

    ymuint64 val0 = node0->bitval();

    if ( val0 != ~0UL ) {
      const_flag[i] |= 1UL;
    }
    if ( val0 != 0UL ) {
      const_flag[i] |= 2UL;
    }

    for (ymuint j = i + 1; j < n; ++ j) {
      StrNode* node1 = imp_mgr.node(j);
      if ( node1 == NULL ) continue;

      if ( !check_intersect(input_list_array[i], input_list_array[j]) ) {
	continue;
      }
      ymuint64 val1 = node1->bitval();
      // node0 が 0 の時に 0 となるノードを探す．
      if ( (~val0 & val1) == 0UL ) {
	if ( !imp_info.check(i, 0, j, 0) ) {
	  cand_info[i * 2 + 0].push_back(ImpVal(j, 0));
	}
      }
      // node0 が 0 の時に 1 となるノードを探す．
      else if ( (~val0 & ~val1) == 0UL ) {
	if ( !imp_info.check(i, 0, j, 1) ) {
	  cand_info[i * 2 + 0].push_back(ImpVal(j, 1));
	}
      }
      // node0 が 1 の時に 0 となるノードを探す．
      if ( (val0 & val1) == 0UL ) {
	if ( !imp_info.check(i, 1, j, 0) ) {
	  cand_info[i * 2 + 1].push_back(ImpVal(j, 0));
	}
      }
      // node0 が 1 の時に 1 となるノードを探す．
      else if ( (val0 & ~val1) == 0UL ) {
	if ( !imp_info.check(i, 1, j, 1) ) {
	  cand_info[i * 2 + 1].push_back(ImpVal(j, 1));
	}
      }
    }
  }

  ymuint prev_size = count_list(cand_info);
  if ( debug ) {
    cerr << "nsat0 = " << prev_size << endl;
  }
  ymuint count = 1;
  ymuint nochg = 0;
  for ( ; ; ) {
    imp_mgr.random_sim();

    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      StrNode* node0 = imp_mgr.node(src_id);
      if ( node0 == NULL ) continue;

      ymuint64 orig_val0 = node0->bitval();
      if ( orig_val0 != ~0UL ) {
	const_flag[src_id] |= 1U;
      }
      if ( orig_val0 != 0UL ) {
	const_flag[src_id] |= 2U;
      }

      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	ymuint64 val0 = orig_val0;
	if ( src_val == 0 ) {
	  val0 = ~val0;
	}
	list<ImpVal>& imp_list = cand_info[src_id * 2 + src_val];
	for (list<ImpVal>::iterator p = imp_list.begin();
	     p != imp_list.end(); ) {
	  const ImpVal& imp = *p;
	  ymuint dst_id = imp.id();
	  StrNode* node1 = imp_mgr.node(dst_id);
	  assert_cond( node1 != NULL, __FILE__, __LINE__);
	  ymuint dst_val = imp.val();
	  ymuint64 val1 = node1->bitval();
	  if ( dst_val == 0 ) {
	    val1 = ~val1;
	  }
	  if ( (val0 & ~val1) != 0UL ) {
	    list<ImpVal>::iterator q = p;
	    ++ p;
	    imp_list.erase(q);
	  }
	  else {
	    ++ p;
	  }
	}
      }
    }
    ymuint cur_size = count_list(cand_info);
    ymuint diff = prev_size - cur_size;
    prev_size = cur_size;

    if ( debug ) {
      if ( (count % 100) == 0 ) {
	cerr << "nsat" << count << " = " << cur_size << endl;
      }
    }
    ++ count;

    if ( diff < 10 ) {
      ++ nochg;
      if ( nochg >= 10 ) {
	break;
      }
    }
    else {
      nochg = 0;
    }
  }

  if ( debug ) {
    cerr << "random simulation end." << endl;
  }

  timer.stop();
  USTime pre_time = timer.time();

  timer.reset();
  timer.start();

  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node0 = imp_mgr.node(i);
    if ( node0 == NULL ) continue;

    if ( (const_flag[i] & 1U) == 0U ) {
      // 0 になったことがない．
      Literal lit(VarId(i), kPolNega);
      vector<Literal> tmp(1, lit);
      vector<Bool3> model;
      if ( solver.solve(tmp, model) == kB3False ) {
	// node0 は 1 固定
	solver.add_clause(~lit);
#if 0
	cout << "Node#" << i << " is const-1" << endl;
#endif
	imp_info.set_1(i);
      }
      else {
	const_flag[i] = 3U;
      }
    }
    else if ( (const_flag[i] & 2U) == 0U ) {
      // 1 になったことがない．
      Literal lit(VarId(i), kPolPosi);
      vector<Literal> tmp(1, lit);
      vector<Bool3> model;
      if ( solver.solve(tmp, model) == kB3False ) {
	// node0 は 0 固定
	solver.add_clause(~lit);
#if 0
	cout << "Node#" << i << " is const-0" << endl;
#endif
	imp_info.set_0(i);
      }
      else {
	const_flag[i] = 3U;
      }
    }
  }

  ymuint remain = count_list(cand_info);
  count = 1;
  ymuint nimp = 0;
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    if ( const_flag[src_id] != 3U ) continue;
    StrNode* node0 = imp_mgr.node(src_id);
    if ( node0 == NULL ) continue;
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      Literal lit0(VarId(src_id), src_val == 0 ? kPolNega : kPolPosi);
      const list<ImpVal>& imp_list = cand_info[src_id * 2 + src_val];
      for (list<ImpVal>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( debug ) {
	  cerr << "sat#" << count << " / " << remain << endl;
	}
	++ count;
	const ImpVal& imp = *p;
	ymuint dst_id = imp.id();
	ymuint dst_val = imp.val();
	if ( const_flag[dst_id] != 3U ) continue;
	Literal lit1(VarId(dst_id), dst_val == 0 ? kPolNega : kPolPosi);
	vector<Literal> tmp(2);
	vector<Bool3> model;
	tmp[0] = lit0;
	tmp[1] = ~lit1;
	if ( solver.solve(tmp, model) == kB3False ) {
	  imp_info.put(src_id, src_val, dst_id, dst_val);
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	  nimp += 2;
#if 1
	  solver.add_clause(~lit0, lit1);
#endif
	}
      }
    }
  }
  timer.stop();
  USTime sat_time = timer.time();

  cout << "SAT statistics" << endl
       << " " << nimp << " / " << remain << endl
       << "  simulation: " << pre_time << endl
       << "  SAT:        " << sat_time << endl;
}

END_NAMESPACE_YM_NETWORKS

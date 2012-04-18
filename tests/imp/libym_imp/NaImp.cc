
/// @file NaImp.cc
/// @brief NaImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NaImp.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpNode.h"
#include "ImpValList.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

#if defined(YM_DEBUG)
bool debug = false;
#else
bool debug = false;
#endif

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス NaImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NaImp::NaImp()
{
}

// @brief デストラクタ
NaImp::~NaImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] direct_imp 直接含意のリスト
// @param[in] imp_info 間接含意のリスト
void
NaImp::learning(ImpMgr& imp_mgr,
		ImpInfo& imp_info)
{
  cerr << "NaImp start" << endl;

  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  vector<ImpValList> imp_lists(n * 2);

  StopWatch timer;
  timer.start();

  // direct_imp の情報を imp_lists にコピーする．
  {
    vector<vector<ImpVal> > imp_lists_array(n * 2);
    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      ImpNode* node = imp_mgr.node(src_id);
      if ( node == NULL ) {
	continue;
      }
      // 自分自身を追加する．
      imp_lists_array[src_id * 2 + 0].push_back(ImpVal(src_id, 0));
      imp_lists_array[src_id * 2 + 1].push_back(ImpVal(src_id, 1));

      // node に 0 を割り当てる．
      vector<ImpVal> imp_list0;
      bool ok0 = imp_mgr.assert(node, 0, imp_list0);
      if ( !ok0 ) {
	imp_list0.clear();
	// 単一の割り当てで矛盾が起こった．
	// node は 1 固定
	imp_mgr.set_const(src_id, 1);
      }
      imp_mgr.backtrack();

      // node に 1 を割り当てる．
      vector<ImpVal> imp_list1;
      bool ok1 = imp_mgr.assert(node, 1, imp_list1);
      if ( !ok1 ) {
	imp_list1.clear();
	// 単一の割り当てで矛盾が起こった．
	// node は 0 固定
	imp_mgr.set_const(src_id, 0);
      }
      imp_mgr.backtrack();

      if ( !imp_mgr.is_const(src_id) ) {
	for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	  const vector<ImpVal>& imp_list = (src_val == 0) ? imp_list0 : imp_list1;
	  for (vector<ImpVal>::const_iterator p = imp_list.begin();
	       p != imp_list.end(); ++ p) {
	    ymuint dst_id = p->id();
	    if ( imp_mgr.is_const(dst_id) ) {
	      continue;
	    }
	    ymuint dst_val = p->val();
	    ymuint src_val1 = src_val ^ 1;
	    ymuint dst_val1 = dst_val ^ 1;
	    imp_lists_array[dst_id * 2 + dst_val].push_back(ImpVal(src_id, src_val));
	    imp_lists_array[src_id * 2 + src_val1].push_back(ImpVal(dst_id, dst_val1));
	  }
	}
      }
    }
    for (ymuint i = 0; i < n; ++ i) {
      for (ymuint val = 0; val < 2; ++ val) {
	vector<ImpVal>& imp_list = imp_lists_array[i * 2 + val];
	sort(imp_list.begin(), imp_list.end());
	vector<ImpVal>::iterator p = unique(imp_list.begin(), imp_list.end());
	imp_list.erase(p, imp_list.end());
	ImpValList& dst_list = imp_lists[i * 2 + val];
	dst_list.insert(imp_list);
	dst_list.set_change1();
	dst_list.set_change2();
      }
    }
  }

  if ( debug ) {
    for (ymuint i = 0; i < n; ++ i) {
      for (ymint val = 0; val < 2; ++ val) {
	ImpValList& imp_list = imp_lists[i * 2 + val];
	cout << "Node#" << i << ":" << val << endl;
	imp_list.print(cout);
      }
    }
  }
#if 1
  timer.stop();
  USTime time = timer.time();
  cerr << "phase0:" << time << endl;
  timer.reset();
  timer.start();
#endif

  // 論理ノードの割り当て情報を作る．
  vector<ImpNode*> node_list;
  imp_mgr.get_node_list(node_list);

  vector<ImpNode*> rnode_list;
  ymuint ni = imp_mgr.input_num();
  rnode_list.reserve(node_list.size() + ni);
  for (vector<ImpNode*>::reverse_iterator p = node_list.rbegin();
       p != node_list.rend(); ++ p) {
    ImpNode* node = *p;
    if ( !node->fanout_list().empty() ) {
      rnode_list.push_back(node);
    }
  }
  for (ymuint i = 0; i < imp_mgr.input_num(); ++ i) {
    rnode_list.push_back(imp_mgr.input_node(i));
  }

  bool smart = false;
  for (bool first = true; ; first = false) {
    ymuint delta = 0;
    for (vector<ImpNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      ImpNode* node = *p;
      ymuint id = node->id();
      if ( imp_mgr.is_const(id) ) {
	continue;
      }
      ymuint idx_0 = id * 2 + 0;
      ymuint idx_1 = id * 2 + 1;

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      ymuint id0 = node0->id();
      bool inv0 = e0.src_inv();
      ymuint idx0_0 = id0 * 2 + (inv0 ? 1 : 0);
      ymuint idx0_1 = idx0_0 ^ 1;

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      ymuint id1 = node1->id();
      bool inv1 = e1.src_inv();
      ymuint idx1_0 = id1 * 2 + (inv1 ? 1: 0);
      ymuint idx1_1 = idx1_0 ^ 1;

      // 出力が0になる条件は入力が0になる条件のユニオン
      {
	if ( debug ) {
	  cout << "Node#" << id << ":0" << endl;
	}
	const ImpValList& src1_list = imp_lists[idx0_0];
	const ImpValList& src2_list = imp_lists[idx1_0];
	ImpValList& dst_list = imp_lists[idx_0];
	dst_list.reset_change1();
	ymuint old_num = dst_list.num();
	if ( smart || src1_list.changed() ) {
	  dst_list.merge(src1_list);
	}
	if ( smart || src2_list.changed() ) {
	  dst_list.merge(src2_list);
	}
	ymuint delta1 = dst_list.num() - old_num;
	if ( delta1 > 0 ) {
	  dst_list.set_change1();
	}
	delta += delta1;
	if ( debug ) {
	  if ( delta1 > 0 ) {
	    cout << " Node#" << id << ":0 changed" << endl;
	  }
	  dst_list.print(cout);
	  cout << endl
	       << endl;
	}
      }
      // 出力が1になる条件は入力が1になる条件のインターセクション
      {
	if ( debug ) {
	  cout << "Node#" << id << ":1" << endl;
	}
	const ImpValList& src1_list = imp_lists[idx0_1];
	const ImpValList& src2_list = imp_lists[idx1_1];
	ImpValList& dst_list = imp_lists[idx_1];
	dst_list.reset_change1();
	ymuint old_num = dst_list.num();
	if ( smart || src1_list.changed() ||
	     src2_list.changed() ) {
	  dst_list.cap_merge(src1_list, src2_list);
	}
	ymuint delta1 = dst_list.num() - old_num;
	if ( delta1 > 0 ) {
	  dst_list.set_change1();
	}
	delta += delta1;
	if ( debug ) {
	  if ( delta1 > 0 ) {
	    cout << " Node#" << id << ":1 changed" << endl;
	  }
	  dst_list.print(cout);
	  cout << endl << endl;
	}
      }
    }
    cerr << "phase1: delta = " << delta << endl;

    for (vector<ImpNode*>::iterator p = rnode_list.begin();
	 p != rnode_list.end(); ++ p) {
      ImpNode* node = *p;
      ymuint id = node->id();
      if ( imp_mgr.is_const(id) ) {
	continue;
      }

      ymuint idx_0 = id * 2 + 0;
      ymuint idx_1 = id * 2 + 1;

      ImpValList& dst0_list = imp_lists[idx_0];
      ImpValList& dst1_list = imp_lists[idx_1];
      dst0_list.reset_change2();
      dst1_list.reset_change2();
      const vector<ImpEdge*>& fo_list = node->fanout_list();
      for (vector<ImpEdge*>::const_iterator p = fo_list.begin();
	   p != fo_list.end(); ++ p) {
	const ImpEdge* edge = *p;
	ImpNode* onode = edge->dst_node();
	ymuint oid = onode->id();
	ymuint opos = edge->dst_pos();
	bool inv = edge->src_inv();
	ymuint oidx_0 = oid * 2 + 0;
	ymuint oidx_1 = oid * 2 + 1;

	const ImpEdge& other_edge = (opos == 0) ? onode->fanin1() : onode->fanin0();
	ImpNode* snode = other_edge.src_node();
	ymuint sid = snode->id();
	bool sinv = other_edge.src_inv();
	ymuint sidx_1 = sid * 2 + (sinv ? 0 : 1);

      	// 出力の0の条件と他方のファンイン1の1の条件の共通部分が
	// 0の条件となる．
	{
	  if ( debug ) {
	    cout << "Node#" << id << ":" << (inv ? 1 : 0) << endl;
	  }
	  const ImpValList& src1_list = imp_lists[oidx_0];
	  const ImpValList& src2_list = imp_lists[sidx_1];
	  ImpValList& dst_list = inv ? dst1_list : dst0_list;
	  ymuint old_num = dst_list.num();
	  if ( smart || src1_list.changed() ||
	       src2_list.changed() ) {
	    dst_list.cap_merge(src1_list, src2_list);
	  }
	  ymuint delta1 = dst_list.num() - old_num;
	  if ( delta1 > 0 ) {
	    dst_list.set_change2();
	  }
	  delta += delta1;
	  if ( debug ) {
	    if ( delta1 > 0 ) {
	      cout << " Node#" << id << ":" << (inv ? 1 : 0) << " changed" << endl;
	    }
	    dst_list.print(cout);
	    cout << endl << endl;
	  }
	}

	// 出力の1の条件がファンイン0の1の条件となる．
	{
	  if ( debug ) {
	    cout << "Node#" << id << ":" << (inv ? 0 : 1) << endl;
	  }
	  const ImpValList& src_list = imp_lists[oidx_1];
	  ImpValList& dst_list = inv ? dst0_list : dst1_list;
	  ymuint old_num = dst_list.num();
	  if ( smart || src_list.changed() ) {
	    dst_list.merge(src_list);
	  }
	  ymuint delta1 = dst_list.num() - old_num;
	  if ( delta1 > 0 ) {
	    dst_list.set_change2();
	  }
	  delta += delta1;
	  if ( debug ) {
	    if ( delta1 > 0 ) {
	      cout << " Node#" << id << ":" << (inv ? 0 : 1) << " changed" << endl;
	    }
	    dst_list.print(cout);
	    cout << endl;
	  }
	}
      }
    }
    cerr << "phase2: delta = " << delta << endl;
    if ( delta == 0 ) {
      break;
    }
  }

  USTime time2 = timer.time();
  cerr << "phase3: " << time2 << endl;

  // imp_lists の情報から imp_info を作る．
  {
    vector<vector<ImpVal> > imp_list_array(n * 2);
    for (ymuint dst_id = 0; dst_id < n; ++ dst_id) {
      if ( imp_mgr.is_const(dst_id) ) {
	continue;
      }
      ImpNode* node = imp_mgr.node(dst_id);
      if ( node == NULL ) {
	continue;
      }
      for (ymuint dst_val = 0; dst_val < 2; ++ dst_val) {
	const ImpValList& imp_list = imp_lists[dst_id * 2 + dst_val];
	for (ImpValListIter p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  const ImpVal& val = *p;
	  ymuint src_id = val.id();
	  ymuint src_val = val.val();
	  if ( src_id == dst_id ) {
	    continue;
	  }
	  imp_list_array[src_id * 2 + src_val].push_back(ImpVal(dst_id, dst_val));
	  imp_list_array[dst_id * 2 + (dst_val ^ 1)].push_back(ImpVal(src_id, src_val ^ 1));
	}
      }
    }
    // imp_list_array の内容を imp_info にコピーする．
    imp_info.set(imp_list_array);
  }

  check_const(imp_mgr, imp_info);

  //verify(imp_mgr, imp_info);

  cerr << "NaImp end" << endl;
}

END_NAMESPACE_YM_NETWORKS

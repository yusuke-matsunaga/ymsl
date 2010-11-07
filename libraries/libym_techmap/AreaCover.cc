
/// @file libym_techmap/AreaCover.cc
/// @brief AreaCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover.h"
#include "PatMgr.h"
#include "PatMatcher.h"
#include "ym_techmap/CnGraph.h"
#include "ym_cell/Cell.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
// クラス AreaCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AreaCover::AreaCover()
{
}

// @brief デストラクタ
AreaCover::~AreaCover()
{
}

// @brief 面積最小化マッピングを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] pat_mgr パタングラフを管理するオブジェクト
// @param[out] mapnetwork マッピング結果
void
AreaCover::operator()(const SbjGraph& sbjgraph,
		      const PatMgr& patmgr,
		      CnGraph& mapnetwork)
{
  record_cuts(sbjgraph, patmgr);

}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] pat_mgr パタングラフを管理するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover::record_cuts(const SbjGraph& sbjgraph,
		       const PatMgr& patmgr)
{
  ymuint n = sbjgraph.max_node_id();
  mNodeInfoArray.resize(n * 2);
  ymuint max_input = patmgr.max_input();
  mWeight.resize(max_input);

  const FuncGroup& inv_func = patmgr.inv_func();

  // 入力のコストを設定
  const SbjNodeList& input_list = sbjgraph.input_list();
  for (SbjNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const SbjNode* node = *p;
    NodeInfo& p_info = node_info(node, false);
    p_info.mCost = 0.0;
    p_info.mCell = NULL;
    add_inv(node, true, inv_func);
  }
  const SbjNodeList& dff_list = sbjgraph.dff_list();
  for (SbjNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const SbjNode* node = *p;
    NodeInfo& p_info = node_info(node, false);
    p_info.mCost = 0.0;
    p_info.mCell = NULL;
    NodeInfo& n_info = node_info(node, true);
    n_info.mCost = 0.0;
    n_info.mCell = NULL;
  }

  // 論理ノードのコストを入力側から計算
  PatMatcher pat_match(patmgr);
  ymuint np = patmgr.pat_num();
  vector<const SbjNode*> snode_list;
  sbjgraph.sort(snode_list);
  for (vector<const SbjNode*>::const_iterator p = snode_list.begin();
       p != snode_list.end(); ++ p) {
    const SbjNode* node = *p;

    NodeInfo& p_info = node_info(node, false);
    NodeInfo& n_info = node_info(node, true);
    p_info.mCost = DBL_MAX;
    n_info.mCost = DBL_MAX;
    for (ymuint pat_id = 0; pat_id < np; ++ pat_id) {
      const PatGraph& pat = patmgr.pat(pat_id);
      ymuint ni = pat.input_num();
      if ( pat_match(node, pat) ) {
	ymuint rep_id = pat.rep_id();
	const RepFunc& rep = patmgr.rep(rep_id);
	ymuint nf = rep.func_num();
	for (ymuint f_pos = 0; f_pos < nf; ++ f_pos) {
	  ymuint func_id = rep.func_id(f_pos);
	  const FuncGroup& func = patmgr.func_group(func_id);
	  const NpnMap& npn_map = func.npn_map();
	  Match c_match(ni);
	  for (ymuint i = 0; i < ni; ++ i) {
	    tNpnImap imap = npn_map.imap(i);
	    const SbjNode* inode = pat_match.leaf_node(npnimap_pos(imap));
	    bool iinv = (npnimap_pol(imap) == kPolNega);
	    c_match.set_leaf(i, inode, iinv);
	  }
	  bool root_inv = pat.root_inv();
	  if ( npn_map.opol() == kPolNega ) {
	    root_inv = !root_inv;
	  }
	  for (ymuint i = 0; i < ni; ++ i) {
	    mWeight[i] = 0.0;
	  }
	  calc_weight(node, c_match, 1.0);
	  ymuint nc = func.cell_num();
	  for (ymuint c_pos = 0; c_pos < nc; ++ c_pos) {
	    const Cell* cell = func.cell(c_pos);
	    double cur_cost = cell->area().value();
	    for (ymuint i = 0; i < ni; ++ i) {
	      const SbjNode* leaf_node = c_match.leaf_node(i);
	      bool leaf_inv = c_match.leaf_inv(i);
	      NodeInfo& l_info = node_info(leaf_node, leaf_inv);
	      cur_cost += l_info.mCost * mWeight[i];
	    }
	    NodeInfo& c_info = root_inv ? n_info : p_info;
	    if ( c_info.mCost >= cur_cost ) {
	      c_info.mCost = cur_cost;
	      c_info.mMatch = c_match;
	      c_info.mCell = cell;
	    }
	  }
	}
      }
    }
    bool has_match = false;
    if ( p_info.mCell != NULL ) {
      has_match = true;
      add_inv(node, true, inv_func);
    }
    if ( n_info.mCell != NULL ) {
      has_match = true;
      add_inv(node, false, inv_func);
    }
    assert_cond( has_match, __FILE__, __LINE__);
  }
}

// @brief 逆極性の解にインバーターを付加した解を追加する．
// @param[in] node 対象のノード
// @param[in] inv 極性
// @param[in] inv_func インバータの関数グループ
void
AreaCover::add_inv(const SbjNode* node,
		   bool inv,
		   const FuncGroup& inv_func)
{
  NodeInfo& node_info = this->node_info(node, inv);
  NodeInfo& alt_info = this->node_info(node, !inv);

  if ( alt_info.mMatch.leaf_num() == 1 ) {
    // 逆極性の解が自分の解＋インバーターだった
    return;
  }
  ymuint nc = inv_func.cell_num();
  for (ymuint c_pos = 0; c_pos < nc; ++ c_pos) {
    const Cell* cell = inv_func.cell(c_pos);
    double cost = cell->area().value();
    cost += alt_info.mCost;
    if ( node_info.mCost > cost ) {
      node_info.mCost = cost;
      node_info.mMatch.resize(1);
      node_info.mMatch.set_leaf(0, node, !inv);
      node_info.mCell = cell;
    }
  }
}

// @brief node から各入力にいたる経路の重みを計算する．
void
AreaCover::calc_weight(const SbjNode* node,
		       const Match& match,
		       double cur_weight)
{
  for ( ; ; ) {
    for (ymuint i = 0; i < match.leaf_num(); ++ i) {
      if ( match.leaf_node(i) == node ) {
	// node は cut の葉だった．
	if  ( !node->pomark() ) {
	  mWeight[i] += cur_weight;
	}
	return;
      }
    }
    const SbjNode* inode0 = node->fanin(0);
    double cur_weight0 = cur_weight / inode0->n_fanout();
    calc_weight(inode0, match, cur_weight0);
    node = node->fanin(1);
    cur_weight /= node->n_fanout();
  }
}

END_NAMESPACE_YM_TECHMAP

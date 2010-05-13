#ifndef MAGUS_CELLMAP_WEIGHTDCOVER_H
#define MAGUS_CELLMAP_WEIGHTDCOVER_H

/// @file magus/cellmap/WeightDCover.h
/// @brief WeightDCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WeightDCover.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "DagDCover.h"
#include "ADCost.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

class SbjNode;

//////////////////////////////////////////////////////////////////////
/// @class WeightDCover WeightDCover.h "WeightDCover.h"
/// @brief depth/area optimal cover を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class WeightDCover :
  public DagDCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mode モード
  WeightDCover(int mode);

  /// @brief デストラクタ
  virtual
  ~WeightDCover();


public:

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder カットを保持するオブジェクト
  /// @param[in] slack 最小段数に対するスラック
  virtual
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      size_t slack,
	      MapRecord& maprec);


private:
  
  // node のカットを記録する．
  void
  record(SbjNode* node,
	 const CutHolder& cut_holder);

  // node のカットを選択する．
  void
  select(SbjNode* node,
	 MapRecord& maprec);
  
  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(SbjNode* node,
	      const Cut* cut,
	      double cur_weight);


private:

  struct NodeInfo
  {

    // コンストラクタ
    NodeInfo() :
      mMinDepth(0),
      mReqDepth(0)
    {
    }

    ADCostList<double> mCostList;
    int mMinDepth;
    int mReqDepth;
  };
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  int mMode;
  
  vector<NodeInfo> mNodeInfo;

  vector<double> mWeight;

  ADCostMgr<double> mCostMgr;

  vector<ADCostIterator<double> > mIcostLists;
  
};

END_NAMESPACE_MAGUS_CELLMAP

#endif // MAGUS_CELLMAP_WEIGHTDCOVER_H

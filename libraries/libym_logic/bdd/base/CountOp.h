#ifndef COUNTOP_H
#define COUNTOP_H

/// @file CountOp.h
/// @brief CountOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CountOp CountOp.h "CountOp.h"
/// @brief 節点数を数える演算を行うクラス
//////////////////////////////////////////////////////////////////////
class CountOp :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  CountOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~CountOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] edge_list 根の枝のリスト
  /// @return edge_list の根からたどれる BDD のノード数を返す．
  ymuint64
  apply(const vector<BddEdge>& edge_list);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief apply() の下請け関数
  void
  count_sub(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 計数に用いるカウンタ
  ymuint64 mNum;

};

END_NAMESPACE_YM_BDD

#endif // COUNTOP_H
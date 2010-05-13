#ifndef MAGUS_CELLMAP_DAGACOVER_H
#define MAGUS_CELLMAP_DAGACOVER_H

/// @file magus/cellmap/DagACover.h
/// @brief DagACover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagACover.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "cellmap.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

class SbjGraph;
class CutHolder;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class DagACover DagACover.h "DagACover.h"
/// @brief DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class DagACover
{
public:
  
  /// @brief デストラクタ
  virtual
  ~DagACover() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder カットを保持するオブジェクト
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  virtual
  int
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      MapRecord& maprec) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class DagACoverFactory DagACover.h "DagACover.h"
/// @brief DagACover (の派生クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class DagACoverFactory
{
public:

  /// @brief コンストラクタ
  DagACoverFactory();
  
  /// @brief デストラクタ
  ~DagACoverFactory();


public:

  /// @brief DagCover (の派生クラス)を生成する．
  DagACover*
  operator()(const string& args);

};

END_NAMESPACE_MAGUS_CELLMAP

#endif // MAGUS_CELLMAP_DAGACOVER_H

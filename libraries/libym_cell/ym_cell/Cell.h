#ifndef YM_CELL_CELL_H
#define YM_CELL_CELL_H

/// @file　ym_cell/Cell.h
/// @brief Cell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cell.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class Cell Cell.h <ym_cell/Cell.h>
/// @brief セル本体のクラス
//////////////////////////////////////////////////////////////////////
class Cell
{
public:

  /// @brief コンストラクタ．
  Cell() { }

  /// @brief デストラクタ
  virtual
  ~Cell() { }


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief 面積の取得
  virtual
  CellArea
  area() const = 0;

  /// @brief ピン数の取得
  virtual
  ymuint
  n_pins() const = 0;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_pins() )
  virtual
  const CellPin*
  pin(ymuint pos) const = 0;

  /// @brief 名前からピンの取得
  /// @param[in] name ピン名
  /// @return name という名前をピンを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellPin*
  pin(const string& name) const = 0;

  /// @brief バス数の取得
  virtual
  ymuint
  n_buses() const = 0;

  /// @brief バスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_buses() )
  virtual
  const CellBus*
  bus(ymuint pos) const = 0;

  /// @brief 名前からバスの取得
  /// @param[in] name バス名
  /// @return name という名前のバスを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellBus*
  bus(const string& name) const = 0;

  /// @brief バンドル数の取得
  virtual
  ymuint
  n_bundles() const = 0;

  /// @brief バンドルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_bundles() )
  virtual
  const CellBundle*
  bundle(ymuint pos) const = 0;

  /// @brief 名前からバンドルの取得
  virtual
  const CellBundle*
  bundle(const string& name) const = 0;

  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] opos 終了ピン番号
  /// @param[out] timing_list タイミング情報を納めるベクタ
  /// @return 条件に合致するタイミング情報の数を返す．
  virtual
  ymuint
  timing(ymuint ipos,
	 ymuint opos,
	 vector<const CellTiming*>& timing_list) const = 0;

  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] opos 終了ピン番号
  /// @param[in] timing_sense タイミング情報の摘要条件
  /// @param[in] timing_type タイミング情報の種類
  /// @param[out] timing_list タイミング情報を納めるベクタ
  /// @return 条件に合致するタイミング情報の数を返す．
  vitual
  ymuint
  timing(ymuint ipos,
	 ymuint opos,
	 tCellTimingSense timing_sense,
	 tCellTimingType timing_type,
	 vector<const CellTiming*>& timing_list) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 使用禁止の関数定義
  //////////////////////////////////////////////////////////////////////

  // コピーコンストラクタと代入演算子は、使用禁止。実体も定義しない。
  Cell(const Cell& src);

  const Cell&
  operator=(const Cell& src);

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELL_H

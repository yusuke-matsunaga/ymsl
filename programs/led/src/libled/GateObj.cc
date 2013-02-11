
/// @file GateObj.cc
/// @brief GateObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GateObj.h"
#include "Symbol.h"


BEGIN_NAMESPACE_YM_LED

// @brief コンストラクタ
// @param[in] sym シンボル
// @param[in] pos 位置
inline
GateObj::GateObj(const Symbol* sym,
		 const QPoint& pos) :
  mSymbol(sym),
  mPosition(pos)
{
}

// @brief コンストラクタ
// @param[in] sym シンボル
// @param[in] x 位置のX座標
// @param[in] y 位置のY座標
inline
GateObj::GateObj(const Symbol* sym,
		 ymuint x,
		 ymuint y) :
  mSymbol(sym),
  mPosition(x, y)
{
}

// @brief デストラクタ
inline
GateObj::~GateObj()
{
}

// @brief 位置を設定する．
// @param[in] pos 設定する位置
inline
void
GateObj::set_position(const QPoint& pos)
{
  mPosition = pos;
}

// @brief 位置を設定する．
// @param[in] x 位置のX座標
// @param[in] y 位置のY座標
inline
void
GateObj::set_position(ymuint x,
		      ymuint y)
{
  mPosition.setX(x);
  mPosition.setY(y);
}

// @brief 位置を取得する．
inline
QPoint
GateObj::position() const
{
  return mPosition;
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
QRect
GateObj::bounding_box() const
{
  QRect tmp = mSymbol->bounding_box();
  tmp.translate(mPosition);
  return tmp;
}

// @brief 入力数を得る．
ymuint
GateObj::ipin_num() const
{
  return mSymbol->ipin_num();
}

// @brief pos 番目の入力ピン位置を得る．
QPoint
GateObj::ipin_location(ymuint pos) const
{
  QPoint tmp = mSymbol->ipin_location(pos);
  return tmp += mPosition;
}

// @brief 出力数を得る．
ymuint
GateObj::opin_num() const
{
  return mSymbol->opin_num();
}

// @brief pos 番目の出力ピン位置を得る．
QPoint
GateObj::opin_location(ymuint pos) const
{
  QPoint tmp = mSymbol->opin_location(pos);
  return tmp += mPosition;
}

// @brief 描画する．
void
GateObj::draw(QPainter& painter) const
{
  painter.save();

  QMatrix matrix;
  matrix.translate(mPosition.x(), mPosition.y());

  mSymbol->draw(painter);

  painter.restore();
}

END_NAMESPACE_YM_LED

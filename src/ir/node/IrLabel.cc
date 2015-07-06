
/// @file IrLabel.cc
/// @brief IrLabel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLabel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrLabel::IrLabel() :
  IrNode(kLabel, NULL),
  mDefined(false)
{
}

// @brief デストラクタ
IrLabel::~IrLabel()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrLabel::is_static() const
{
  return true;
}

// @brief 定義済みの時に true を返す．
//
// kOpLabel のみ意味を持つ．
bool
IrLabel::is_defined() const
{
  return mDefined;
}

// @brief 定義済みにする．
//
// kOpLabel のみ意味を持つ．
void
IrLabel::set_defined()
{
  mDefined = true;
}

END_NAMESPACE_YM_YMSL

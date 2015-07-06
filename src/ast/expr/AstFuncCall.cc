
/// @file AstFuncCall.cc
/// @brief AstFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncCall.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
// @param[in] expr_num 引数の数
// @param[in] expr_list 引数リスト
// @param[in] loc ファイル位置
AstFuncCall::AstFuncCall(AstExpr* func,
			 ymuint expr_num,
			 AstExpr** expr_list,
			 const FileRegion& loc) :
  AstExpr(loc),
  mFunc(func),
  mExprNum(expr_num),
  mExprList(expr_list)
{
}

// @brief デストラクタ
AstFuncCall::~AstFuncCall()
{
}

// @brief 種類を返す．
AstExpr::Type
AstFuncCall::expr_type() const
{
  return kFuncCall;
}

// @brief 関数本体を返す．
const AstExpr*
AstFuncCall::func() const
{
  return mFunc;
}

// @brief 引数リストの要素数を返す．
//
// kFuncCall のみ有効
ymuint
AstFuncCall::arglist_num() const
{
  return mExprNum;
}

// @brief 引数リストの要素を返す．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
const AstExpr*
AstFuncCall::arglist_elem(ymuint pos) const
{
  ASSERT_COND( pos < arglist_num() );
  return mExprList[pos];
}

END_NAMESPACE_YM_YMSL

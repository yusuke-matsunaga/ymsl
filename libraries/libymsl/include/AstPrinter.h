#ifndef ASTPRINTER_H
#define ASTPRINTER_H

/// @file AstPrinter.h
/// @brief AstPrinter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstPrinter AstPrinter.h "AstPrinter.h"
/// @brief AST の内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class AstPrinter
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  AstPrinter(ostream& s);

  /// @brief デストラクタ
  ~AstPrinter();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 文を出力する．
  /// @param[in] stmt 文
  /// @param[in] indent インデントレベル
  void
  print_statement(const AstStatement* stmt,
		  ymuint indent = 0);

  /// @brief 式を出力する．
  /// @param[in] expr 式
  void
  print_expr(const AstExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 終端式を出力する．
  /// @param[in] leaf 式
  void
  print_leaf(const AstLeaf* leaf);

  /// @brief インデントする．
  /// @param[in] indent インデントレベル
  void
  print_indent(ymuint indent);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mS;

};

END_NAMESPACE_YM_YMSL

#endif // ASTPRINTER_H
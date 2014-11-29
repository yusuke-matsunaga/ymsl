#ifndef YMSLPARSER_H
#define YMSLPARSER_H

/// @file YmslParser.h
/// @brief YmslParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmslScanner.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslParser YmslParser.h "YmslParser.h"
/// @brief YMSL 用の構文解析器
//////////////////////////////////////////////////////////////////////
class YmslParser
{
public:

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式を読む．
  YmslAst*
  read_expr();

  /// @brief 項を読む．
  YmslAst*
  read_term();

  /// @brief ファクターを読む．
  YmslAst*
  read_factor();

  /// @brief プライマリを読む．
  YmslAst*
  read_primary();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  YmslScanner* mScanner;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLPARSER_H
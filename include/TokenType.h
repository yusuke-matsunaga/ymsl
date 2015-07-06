#ifndef TOKENTYPE_H
#define TOKENTYPE_H

/// @file TokenType.h
/// @brief TokenType の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

#if 0
/// @brief トークンの値
enum TokenType {
  /// @brief コロン(:)
  COLON,
  /// @brief セミコロン(;)
  SEMI,
  /// @brief コンマ(,)
  COMMA,
  /// @brief ドット(.)
  DOT,

  /// @brief プラス(+)
  PLUS,
  /// @brief マイナス(-)
  MINUS,
  /// @brief かける(*)
  MULT,
  /// @brief わる(/)
  DIV,
  /// @brief 余り(%)
  MOD,
  /// @brief ++
  PLUSPLUS,
  /// @brief --
  MINUSMINUS,

  /// @brief logical not(!)
  LNOT,
  /// @brief logical and(&&)
  LAND,
  /// @brief logical or(||)
  LOR,
  /// @brief 等しい(==)
  EQEQ,
  /// @brief 等しくない(!=)
  NOTEQ,
  /// @brief 小なり(<)
  LT,
  /// @brief 大なり(>)
  GT,
  /// @brief 小なりイコール(<=)
  LE,
  /// @brief 大なりイコール(>=)
  GE,

  /// @brief bitwise not(~)
  BITNOT,
  /// @brief bitwise and(&)
  BITAND,
  /// @brief bitwise or(|)
  BITOR,
  /// @brief bitwise xor(^)
  BITXOR,

  /// @brief 代入 (=)
  EQ,

  /// @brief 左括弧( ( )
  LP,
  /// @brief 右括弧( ) )
  RP,
  /// @brief 左中括弧( { )
  LCB,
  /// @brief 右中括弧( } )
  RCB,
  /// @brief 左角括弧( [ )
  LBK,
  /// @brief 右角括弧( ] )
  RBK,

  /// @brief 'if'
  IF,
  /// @brief 'else'
  ELSE,
  /// @brief 'elif'
  ELIF,
  /// @brief 'for'
  FOR,
  /// @brief 'while'
  WHILE,
  /// @brief 'do'
  DO,
  /// @brief 'goto'
  GOTO,
  /// @brief 'break'
  BREAK,
  /// @brief 'continue'
  CONTINUE,

  /// @brief 'function'
  FUNCTION,
  /// @brief 'return'
  RETURN,
  /// @brief 'var'
  VAR,

  /// @brief シンボル
  SYMBOL,
  /// @brief 整数値
  INT_NUM,
  /// @brief 浮動小数点数値
  FLOAT_NUM,
  /// @brief 文字列リテラル
  STRING,
  /// @brief エラー
  ERROR,
  /// @brief ファイルの末尾
  END
};
#else
typedef int TokenType;
#endif

/// @brief トークン番号から文字列を返す関数
extern
const char*
token2str(TokenType token);

END_NAMESPACE_YM_YMSL

#endif // TOKENTYPE_H

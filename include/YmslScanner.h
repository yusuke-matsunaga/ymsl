#ifndef YMSLSCANNER_H
#define YMSLSCANNER_H

/// @file YmslScanner.h
/// @brief YmslScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/Scanner.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/StrBuff.h"
#include "TokenType.h"
#include "OpCode.h"


BEGIN_NAMESPACE_YM_YMSL

#include "grammer.hh"

class RsrvWordDic;

//////////////////////////////////////////////////////////////////////
/// @class YmslScanner YmslScanner.h "YmslScanner.h"
/// @brief YMSL 用の字句解析器
//////////////////////////////////////////////////////////////////////
class YmslScanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  /// @param[in] ido 入力データ
  YmslScanner(IDO& ido);

  /// @brief デストラクタ
  ~YmslScanner();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを一つ読み込む．
  /// @param[out] loc ファイル上の位置情報を格納する変数
  /// @return トークンの型を返す．
  TokenType
  read_token(FileRegion& loc);

  /// @brief 読んだトークンを戻す．
  /// @param[in] token トークン
  /// @param[in] loc ファイル位置
  void
  unget_token(TokenType token,
	      const FileRegion& loc);

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;

  /// @brief 直前の read_token() に対応する整数値を返す．
  /// @note 型が INT_NUM でなかったときの値は不定
  Ymsl_INT
  cur_int() const;

  /// @brief 直前の read_token() に対応する実数値を返す．
  /// @note 型が FLOAT_NUM/INT_NUM でなかったときの値は不定
  Ymsl_FLOAT
  cur_float() const;

  /// @brief 現在のトークンの内容を出力する．(デバッグ用)
  /// @param[in] id トークン
  /// @param[in] s 出力先のストリーム
  void
  print_token(TokenType id,
	      ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() の下請け関数
  /// @return トークンの型を返す．
  TokenType
  scan();

  /// @brief c が文字の時に true を返す．
  /// @note mSymbolMode が true なら数字も文字とみなす．
  bool
  is_symbol(int c);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // read_token の結果の文字列を格納する
  StrBuff mCurString;

  // 予約後表
  RsrvWordDic* mDic;

  // unget したトークン
  // unget していない場合は ERROR を入れておく．
  TokenType mUngetToken;

  // unget したファイル位置
  FileRegion mUngetLoc;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
YmslScanner::cur_string() const
{
  return mCurString.c_str();
}

// @brief 直前の read_token() に対応する整数値を返す．
// @note 型が INT_NUM でなかったときの値は不定
inline
Ymsl_INT
YmslScanner::cur_int() const
{
  return strtol(cur_string(), NULL, 10);
}

// @brief 直前の read_token() に対応する実数値を返す．
// @note 型が FLOAT_NUM/INT_NUM でなかったときの値は不定
inline
Ymsl_FLOAT
YmslScanner::cur_float() const
{
  return strtod(cur_string(), NULL);
}

END_NAMESPACE_YM_YMSL


#endif // YMSLSCANNER_H

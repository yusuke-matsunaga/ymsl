﻿
/// @file YmslScanner.cc
/// @brief YmslScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslScanner.h"
#include "RsrvWordDic.h"
#include "OpCode.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_YMSL

#include "grammer.hh"

// @brief コンストラクタ
// @param[in] ido 入力データ
YmslScanner::YmslScanner(IDO& ido) :
  Scanner(ido)
{
  mDic = new RsrvWordDic;
  mUngetToken = DUMMY;
}

// デストラクタ
YmslScanner::~YmslScanner()
{
  delete mDic;
}

// @brief トークンを一つとってくる．
// @param[out] loc ファイル上の位置情報を格納する変数
TokenType
YmslScanner::read_token(FileRegion& loc)
{
  if ( mUngetToken != DUMMY ) {
    TokenType token = mUngetToken;
    loc = mUngetLoc;
    mUngetToken = DUMMY;
    return token;
  }

  TokenType type = scan();
  loc = cur_loc();
  return type;
}

// @brief read_token() の下請け関数
// @return トークンの型を返す．
TokenType
YmslScanner::scan()
{
  mCurString.clear();

 ST_INIT: // 初期状態
  int c = get();
  set_first_loc();
  if ( is_symbol(c) ) {
    mCurString.put_char(c);
    goto ST_ID;
  }
  if ( isdigit(c) ) {
    mCurString.put_char(c);
    goto ST_NUM1;
  }

  switch (c) {
  case '.':
    mCurString.put_char(c);
    goto ST_DOT;

  case EOF:
    return EOF;

  case ' ':
  case '\t':
  case '\n':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '\"':
    goto ST_DQ;

  case '\\':
    c = peek();
    if ( c == '\n' ) {
      // 無視する．
      accept();
      goto ST_INIT;
    }
    // それ以外はバックスラッシュがなかったことにする．
    goto ST_INIT;

  case '/':
    goto ST_DIV;

  case ':':
    return COLON;

  case ';':
    return SEMI;

  case ',':
    return COMMA;

  case '?':
    return QST;

  case '+':
    goto ST_PLUS;

  case '-':
    goto ST_MINUS;

  case '*':
    goto ST_MULT;

  case '%':
    goto ST_MOD;

  case '!':
    goto ST_NOT;

  case '&':
    goto ST_AND;

  case '|':
    goto ST_OR;

  case '~':
    return BITNEG;

  case '^':
    goto ST_XOR;

  case '=':
    goto ST_EQ;

  case '<':
    goto ST_LT;

  case '>':
    goto ST_GT;

  case '(':
    return LP;

  case ')':
    return RP;

  case '{':
    return LCB;

  case '}':
    return RCB;

  case '[':
    return LBK;

  case ']':
    return RBK;

  default:
    // それ以外はエラーなんじゃない？
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    "syntax error");
    return ERROR;
  }
  ASSERT_NOT_REACHED;

 ST_DOT: // '.' を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char('.');
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  return DOT;

 ST_PLUS: // '+' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQPLUS;
  }
  if ( c == '+' ) {
    accept();
    return PLUSPLUS;
  }
  return PLUS;

 ST_MINUS: // '-' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQMINUS;
  }
  if ( c == '-' ) {
    accept();
    return MINUSMINUS;
  }
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char('-');
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  return MINUS;

 ST_MULT: // '*' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQMULT;
  }
  return MULT;

 ST_MOD: // '%' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQMOD;
  }
  return MOD;

 ST_NOT: // '!' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return NOTEQ;
  }
  {
    ostringstream buf;
    buf << "unexpected character '!'";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_AND: // '&' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQBITAND;
  }
  return BITAND;

 ST_OR: // '|' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQBITOR;
  }
  return BITOR;

 ST_XOR: // '^' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQBITXOR;
  }
  return BITXOR;

 ST_EQ: // '=' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQEQ;
  }
  return EQ;

 ST_LT: // '<' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return LE;
  }
  if ( c == '<' ) {
    accept();
    c = peek();
    if ( c == '=' ) {
      accept();
      return EQLSHIFT;
    }
    return LSHIFT;
  }
  return LT;

 ST_GT: // '>' を読み込んだ時
  c = peek();
  if ( c == '=' ) {
    accept();
    return GE;
  }
  if ( c == '>' ) {
    accept();
    c = peek();
    if ( c == '=' ) {
      accept();
      return EQRSHIFT;
    }
    return RSHIFT;
  }
  return GT;

 ST_NUM1: // 一文字目が[0-9]の時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM1;
  }
  if ( c == '.' ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUMDOT;
  }
  return INT_VAL;

 ST_NUMDOT: // [0-9]+'.' を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  { // '.' の直後はかならず数字
    ostringstream buf;
    buf << "digit number expected after dot";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM2: // [0-9]*'.'[0-9]* を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM2;
  }
  if ( c == 'e' || c == 'E' ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM3;
  }
  return FLOAT_VAL;

 ST_NUM3: // [0-9]*'.'[0-9]*(e|E)を読み込んだ時
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  if ( c == '+' || c == '-' ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  { // (e|E) の直後はかならず数字か符号
    ostringstream buf;
    buf << "exponent value expected";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }

 ST_NUM4: // [0-9]*'.'[0-9]*(e|E)(+|-)?[0-9]*を読み込んだ直後
  c = peek();
  if ( isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM4;
  }
  return FLOAT_VAL;

 ST_ID: // 一文字目が[a-zA-Z_]の時
  c = peek();
  if ( is_symbol(c) || isdigit(c) ) {
    accept();
    mCurString.put_char(c);
    goto ST_ID;
  }
  return mDic->token(mCurString.c_str());

 ST_DQ: // "があったら次の"までを強制的に文字列だと思う．
  c = get();
  if ( c == '\"' ) {
    return STRING_VAL;
  }
  if ( c == '\n' ) {
    ostringstream buf;
    buf << "unexpected newline in quoted string.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  if ( c == EOF ) {
    ostringstream buf;
    buf << "unexpected end-of-file in quoted string.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
    return ERROR;
  }
  if ( c == '\\' ) {
    c = get();
    if ( c != '\n' ) {
      // 改行以外はバックスラッシュをそのまま解釈する．
      mCurString.put_char('\\');
    }
  }
  mCurString.put_char(c);
  goto ST_DQ;

 ST_DIV: // '/' を読み込んだ直後
  c = peek();
  if ( c == '=' ) {
    accept();
    return EQDIV;
  }
  if ( c == '/' ) { // C++ スタイルのコメント
    accept();
    goto ST_COMMENT2;
  }
  if ( c == '*' ) { // C スタイルのコメント
    accept();
    goto ST_COMMENT3;
  }
  return DIV;

 ST_COMMENT2: // 改行まで読み飛ばす．
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return EOF;
  }
  goto ST_COMMENT2;

 ST_COMMENT3: // "/*" を読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  goto ST_COMMENT3;

 ST_COMMENT4: // "/* 〜 *" まで読み込んだ直後
  c = get();
  if ( c == EOF ) {
    goto ST_EOF;
  }
  if ( c == '/' ) {
    goto ST_INIT;
  }
  if ( c == '*' ) {
    goto ST_COMMENT4;
  }
  goto ST_COMMENT3;

 ST_EOF:
  {
    ostringstream buf;
    buf << "Unexpected end-of-file in comment block.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_LEX",
		    buf.str());
  }
  return ERROR;
}

// @brief 読んだトークンを戻す．
// @param[in] token トークン
// @param[in] loc ファイル位置
void
YmslScanner::unget_token(TokenType token,
			 const FileRegion& loc)
{
  ASSERT_COND( mUngetToken == DUMMY );
  mUngetToken = token;
  mUngetLoc = loc;
}

// @brief c が文字の時に true を返す．
// @note mSymbolMode が true なら数字も文字とみなす．
bool
YmslScanner::is_symbol(int c)
{
  if ( isalpha(c) || c == '_' ) {
    return true;
  }
  return false;
}

// @brief 現在のトークンの内容を出力する．(デバッグ用)
// @param[in] id トークン
// @param[in] s 出力先のストリーム
void
YmslScanner::print_token(TokenType id,
			 ostream& s) const
{
  switch ( id ) {
  case SYMBOL:     cout << "SYMBOL[" << cur_string() << "]"; break;
  case INT_VAL:    cout << "INT["    << cur_int() << "]"; break;
  case FLOAT_VAL:  cout << "FLOAT["  << cur_float() << "]"; break;
  case STRING_VAL: cout << "STRING[" << cur_string() << "]"; break;
  case EOF:        cout << "EOF"; break;
  default:         cout << mDic->str(id); break;
  }
}

END_NAMESPACE_YM_YMSL

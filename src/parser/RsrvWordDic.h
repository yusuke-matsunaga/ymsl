﻿#ifndef RSRVWORDDIC_H
#define RSRVWORDDIC_H

/// @file RsrvWordDic.h
/// @brief RsrvWordDic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "TokenType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class RsrvWordDic RsrvWordDic.h "RsrvWordDic.h"
/// @brief YMSL の予約語テーブル
//////////////////////////////////////////////////////////////////////
class RsrvWordDic
{
public:

  /// @brief コンストラクタ
  RsrvWordDic();

  /// @brief デストラクタ
  ~RsrvWordDic();


public:

  /// @brief str が予約語ならそのトークン番号を返す．
  /// @param[in] str 文字列
  /// @return str が予約語ならそのトークン番号を返す．
  /// そうでないときは SYMBOL を返す．
  TokenType
  token(const char* str) const;

  /// @brief トークンから文字列を取り出す．
  /// @param[in] token トークン番号
  /// @return token に対応した文字列を返す．\n
  /// トークンが正しくなければ NULL を返す．
  const char*
  str(TokenType token) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 値を覚えておくためのセル
  struct Cell
  {
    // 文字列
    const char* mStr;

    // トークンの値
    TokenType mTok;

    // ハッシュ用のリンク
    Cell* mLink1;

    // トークンをキーとしたハッシュ用のリンク
    Cell* mLink2;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の本体の配列
  Cell* mCellArray;

  // 文字列をキーとしてトークンを持つハッシュ表
  Cell** mTable1;

  // トークンをキーとして文字列を持つハッシュ表
  Cell** mTable2;

  // テーブルサイズ
  ymuint mSize;

};

END_NAMESPACE_YM_YMSL

#endif // RSRVWORDDIC_H

#ifndef YMSLASTSYMBOL_H
#define YMSLASTSYMBOL_H

/// @file YmslAstSymbol.h
/// @brief YmslAstSymbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstSymbol YmslAstSymbol.h "YmslAstSymbol.h"
/// @brief 整数型定数を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstSymbol :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAstSymbol(ShString val,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstSymbol();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 文字列型の値を返す．
  virtual
  const char*
  str_val() const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mVal;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTSYMBOL_H
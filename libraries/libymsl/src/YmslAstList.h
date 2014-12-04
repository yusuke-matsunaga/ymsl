#ifndef YMSLASTLIST_H
#define YMSLASTLIST_H

/// @file YmslAstList.h
/// @brief YmslAstList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstList YmslAstList.h "YmslAstList.h"
/// @brief リストを表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstList :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  YmslAstList();

  /// @brief デストラクタ
  virtual
  ~YmslAstList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 子供の数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置( 0 <= pos < child_num() )
  virtual
  YmslAst*
  child(ymuint pos) const;

  /// @brief 子供を追加する．
  virtual
  void
  add_child(YmslAst* child);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リストの本体
  vector<YmslAst*> mList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTLIST_H

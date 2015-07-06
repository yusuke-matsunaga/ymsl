#ifndef ASTMAPTYPE_H
#define ASTMAPTYPE_H

/// @file AstMapType.h
/// @brief AstMapType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstMapType AstMapType.h "AstMapType.h"
/// @brief array 型を表す AstType
//////////////////////////////////////////////////////////////////////
class AstMapType :
  public AstType
{
public:

  /// @brief コンストラクタ
  /// @param[in] key_type キーの方
  /// @param[in] elem_type 要素の型
  /// @param[in] loc ファイル位置
  AstMapType(AstType* key_type,
	     AstType* elem_type,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstMapType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief map 型の時にキーの型を返す．
  virtual
  const AstType*
  key_type() const;

  /// @brief array/set/map 型の時に要素の型を返す．
  virtual
  const AstType*
  elem_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キーの方
  AstType* mKeyType;

  // 要素の型
  AstType* mElemType;

};

END_NAMESPACE_YM_YMSL


#endif // ASTMAPTYPE_H

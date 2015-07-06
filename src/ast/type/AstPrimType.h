#ifndef ASTPRIMTYPE_H
#define ASTPRIMTYPE_H

/// @file AstPrimType.h
/// @brief AstPrimType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstPrimType AstPrimType.h "AstPrimType.h"
/// @brief プリミティブ型を表す AstType
//////////////////////////////////////////////////////////////////////
class AstPrimType :
  public AstType
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] loc ファイル位置
  AstPrimType(TypeId type,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstPrimType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  TypeId mType;

};

END_NAMESPACE_YM_YMSL

#endif // ASTPRIMTYPE_H

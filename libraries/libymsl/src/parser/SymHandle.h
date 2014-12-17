#ifndef SYMHANDLE_H
#define SYMHANDLE_H

/// @file SymHandle.h
/// @brief SymHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class SymHandle SymHandle.h "SymHandle.h"
/// @brief Symbol Table のハンドル
//////////////////////////////////////////////////////////////////////
class SymHandle
{
  friend class YmslDict;

public:

  /// @brief コンストラクタ
  SymHandle();

  /// @brief デストラクタ
  virtual
  ~SymHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const = 0;

  /// @brief 変数宣言を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  AstVarDecl*
  vardecl() const;

  /// @brief 関数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  AstFuncDecl*
  func() const;

  /// @brief ラベルステートメントを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  AstStatement*
  statement() const;

  /// @brief 名前空間を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  YmslSubspace*
  subspace() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  SymHandle* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class VarHandle SymHandle.h "SymHandle.h"
/// @brief AstVarDecl を保持する SymHandle
//////////////////////////////////////////////////////////////////////
class VarHandle :
  public SymHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] vardecl 変数宣言
  VarHandle(AstVarDecl* vardecl);

  /// @brief デストラクタ
  virtual
  ~VarHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 変数宣言を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  AstVarDecl*
  vardecl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数宣言
  AstVarDecl* mVarDecl;

};


//////////////////////////////////////////////////////////////////////
/// @class FuncHandle SymHandle.h "SymHandle.h"
/// @brief AstFuncDecl を保持する SymHandle
//////////////////////////////////////////////////////////////////////
class FuncHandle :
  public SymHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  FuncHandle(AstFuncDecl* func);

  /// @brief デストラクタ
  virtual
  ~FuncHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 関数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  AstFuncDecl*
  func() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  AstFuncDecl* mFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class StmtHandle SymHandle.h "SymHandle.h"
/// @brief AstStatement を保持する SymHandle
//////////////////////////////////////////////////////////////////////
class StmtHandle :
  public SymHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] statement ラベルすてーとめんと
  StmtHandle(AstStatement* statement);

  /// @brief デストラクタ
  virtual
  ~StmtHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 関数宣言を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  AstStatement*
  statement() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメント
  AstStatement* mStatement;

};


//////////////////////////////////////////////////////////////////////
/// @class SubspaceHandle SymHandle.h "SymHandle.h"
/// @brief AstStatement を保持する SymHandle
//////////////////////////////////////////////////////////////////////
class SubspaceHandle :
  public SymHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] subspace 名前空間
  SubspaceHandle(YmslSubspace* subspace);

  /// @brief デストラクタ
  virtual
  ~SubspaceHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 名前空間を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  YmslSubspace*
  subspace() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前空間
  YmslSubspace* mSubspace;

};

END_NAMESPACE_YM_YMSL

#endif // SYMHANDLE_H

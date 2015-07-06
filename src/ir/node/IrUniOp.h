#ifndef IRUNIOP_H
#define IRUNIOP_H

/// @file IrUniOp.h
/// @brief IrUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrUniOp IrUniOp.h "IrUniOp.h"
/// @brief 単項演算を表すノード
//////////////////////////////////////////////////////////////////////
class IrUniOp :
  public IrOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] type 型
  /// @param[in] src1 オペランド
  IrUniOp(OpCode opcode,
	  const Type* type,
	  IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrUniOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief オペランド数を返す．
  ///
  /// 演算子のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num() )
  ///
  /// 演算子のみ有効
  virtual
  IrNode*
  operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 第1ソース
  IrNode* mSrc1;

};

END_NAMESPACE_YM_YMSL

#endif // IRUNIOP_H

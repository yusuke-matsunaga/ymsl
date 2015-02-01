#ifndef IRFALSE_H
#define IRFALSE_H

/// @file IrFalse.h
/// @brief IrFalse のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFalse IrFalse.h "IrFalse.h"
/// @brief true を表すノード
//////////////////////////////////////////////////////////////////////
class IrFalse :
  public IrNode
{
public:

  /// @brief コンストラクタ
  IrFalse();

  /// @brief デストラクタ
  virtual
  ~IrFalse();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief opcode を返す．
  virtual
  OpCode
  opcode() const;

};

END_NAMESPACE_YM_YMSL

#endif // IRFALSE_H

#ifndef IRRETURN_H
#define IRRETURN_H

/// @file IrReturn.h
/// @brief IrReturn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrReturn IrReturn.h "IrReturn.h"
/// @brief return 命令を表すクラス
//////////////////////////////////////////////////////////////////////
class IrReturn :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] ret_val 返り値を表す式
  IrReturn(IrNode* ret_val);

  /// @brief デストラクタ
  virtual
  ~IrReturn();


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

  /// @brief 返り値
  virtual
  IrNode*
  return_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 返り値
  IrNode* mReturnVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRRETURN_H

#ifndef IRFUNCTION_H
#define IRFUNCTION_H

/// @file IrFunction.h
/// @brief IrFunction のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"

BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFunction IrFunction.h "IrFunction.h"
/// @brief 関数の中間表現
//////////////////////////////////////////////////////////////////////
class IrFunction
{
public:

  /// @brief コンストラクタ
  IrFunction();

  /// @brief デストラクタ
  ~IrFunction();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数本体
  const Function* mFunction;

  // 引数のリスト
  vector<const Var*> mArgList;

  // 引数のデフォルト値のリスト
  vector<IrNode*> mArgInitList;

  // 変数のリスト
  vector<const Var*> mVarList;

  // 本体のノードリスト
  vector<IrNode*> mNodeList;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCTION_H

#ifndef YMSLPRINT_H
#define YMSLPRINT_H

/// @file YmslPrint.h
/// @brief YmslPrint のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VsmBuiltinFunc.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslPrint YmslPrint.h "YmslPrint.h"
/// @brief print 関数
//////////////////////////////////////////////////////////////////////
class YmslPrint :
  public VsmBuiltinFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 型
  YmslPrint(ShString name,
	    const Type* type);

  /// @brief デストラクタ
  virtual
  ~YmslPrint();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  virtual
  ymuint
  arg_num() const;

  /// @brief 本当の実行関数
  /// @param[in] arg_list 引数のリスト
  /// @param[in] ret_val 返り値を格納する変数
  ///
  /// 実際の派生クラスが実装する必要がある．
  virtual
  VsmValue
  _execute(const vector<VsmValue>& arg_list) const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLPRINT_H

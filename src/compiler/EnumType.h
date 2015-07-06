#ifndef ENUMTYPE_H
#define ENUMTYPE_H

/// @file EnumType.h
/// @brief EnumType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NamedType.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

class EnumConst;

//////////////////////////////////////////////////////////////////////
/// @class EnumType EnumType.h "EnumType.h"
/// @brief enum 型を表すクラス
//////////////////////////////////////////////////////////////////////
class EnumType :
  public NamedType
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] elem_list 要素名と値のリスト
  EnumType(ShString name,
	   const vector<pair<ShString, Ymsl_INT> >& elem_list);

  /// @brief デストラクタ
  virtual
  ~EnumType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定された型にキャスト可能な場合に true を返す．
  /// @param[in] type 指定された型
  virtual
  bool
  castable_to(const Type* type) const;

  /// @brief 列挙型の数を得る．
  ///
  /// enum のみ有効
  virtual
  ymuint
  enum_num() const;

  /// @brief 列挙型の定数値を得る．
  /// @param[in] index インデックス ( 0 <= index < enum_num() )
  ///
  /// enum のみ有効
  virtual
  Ymsl_INT
  enum_elem_val(ymuint index) const;

  /// @brief 列挙型の定数名を得る．
  /// @param[in] index インデックス ( 0 <= index < enum_num() )
  ///
  /// enum のみ有効
  virtual
  ShString
  enum_elem_name(ymuint index) const;

  /// @brief 列挙型のインデックスを得る．
  /// @param[in] name 列挙型の定数の名前
  ///
  /// enum のみ有効
  /// 該当する名前がなければ -1 を返す．
  virtual
  int
  enum_index(ShString name) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint mElemNum;

  // 要素の配列
  EnumConst* mElemArray;

};

END_NAMESPACE_YM_YMSL

#endif // ENUMTYPE_H

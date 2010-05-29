#ifndef YM_VERILOG_VL_VLDECL_H
#define YM_VERILOG_VL_VLDECL_H

/// @file ym_verilog/vl/VlNamedObj.h
/// @brief VlNamedObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlDelay;
class VlExpr;
class VlRange;

//////////////////////////////////////////////////////////////////////
/// @class VlDecl VlDecl.h <ym_verilog/vl/VlDecl.h>
/// @brief エラボレーション中の名前付きオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlDecl :
  public VlNamedObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDecl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const = 0;
  
  /// @brief 定数値を持つ型のときに true を返す．
  virtual
  bool
  is_consttype() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  right_range() const = 0;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const = 0;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const = 0;
  
  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const = 0;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const = 0;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  charge() const = 0;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  virtual
  const VlDelay*
  delay() const = 0;
  
  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const VlExpr*
  init_value() const = 0;

  /// @brief localparam のときに true 返す．
  virtual
  bool
  is_local_param() const = 0;

  /// @brief 配列型オブジェクトの場合の次元数の取得
  virtual
  ymuint32
  dimension() const = 0;
  
  /// @brief 範囲の取得
  /// @param[in] pos 位置 (0 <= pos < dimension_list_size())
  virtual
  const VlRange*
  range(ymuint32 pos) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLDECL_H

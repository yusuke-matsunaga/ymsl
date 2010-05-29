#ifndef LIBYM_VERILOG_ELB_ELBDECL_H
#define LIBYM_VERILOG_ELB_ELBDECL_H

/// @file libym_verilog/elaborator/include/ElbDecl.h
/// @brief ElbDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlDecl.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbDeclHead.h ElbDecl.h "ElbDecl.h"
/// @brief 宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class ElbDeclHead
{
protected:
  
  /// @brief コンストラクタ
  ElbDeclHead();
  
  /// @brief デストラクタ
  virtual
  ~ElbDeclHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const = 0;

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_signed() const;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  right_range() const;

  /// @brief ビット幅を返す．
  /// @note このクラスでは 1 を返す．
  virtual
  ymuint32
  bit_size() const = 0;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  bit_offset(int index) const = 0;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  /// @note このクラスでは kVpiVarNone を返す．
  virtual
  tVpiVarType
  data_type() const;
  
  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note このクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note このクラスでは kVpiNsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  /// @note このクラスでは NULL を返す．
  virtual
  ElbDelay*
  delay() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の設定
  /// @note このクラスではなにもしない．
  virtual
  void
  set_delay(ElbDelay* delay);
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeclBase ElbDecl.h "ElbDecl.h"
/// @brief 名前付きの宣言要素の基底クラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
/// IEEE Std 1364-2001 26.6.8 Variables
/// IEEE Std 1364-2001 26.6.11 Named event
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbDeclBase :
  public VlDecl
{
protected:
  
  /// @brief コンストラクタ
  ElbDeclBase();
  
  /// @brief デストラクタ
  virtual
  ~ElbDeclBase();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  /// @note デフォルト値としてこのクラスでは kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;
  
  /// @brief 定数値を持つ型のときに true を返す．
  /// @note デフォルト値としてこのクラスでは false を返す．
  virtual
  bool
  is_consttype() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_signed() const;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  const VlExpr*
  right_range() const;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  /// @note デフォルト値としてこのクラスでは kVpiVarNone を返す．
  virtual
  tVpiVarType
  data_type() const;
  
  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note デフォルト値としてこのクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note デフォルト値としてこのクラスでは kVpiVsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  /// @note デフォルト値としてこのクラスでは NULL を返す．
  virtual
  const VlDelay*
  delay() const;
    
  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  init_value() const;
  
  /// @brief localparam のときに true 返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_local_param() const;

  /// @brief 配列型オブジェクトの場合の次元数の取得
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  dimension() const;
  
  /// @brief 範囲の取得
  /// @param[in] pos 位置 ( 0 <= pos < dimension() )
  /// @note このクラスでは NULL を返す．
  virtual
  const VlRange*
  range(ymuint32 pos) const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclBase の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _right_range() const = 0;
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbDeclBase* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDecl ElbDecl.h "ElbDecl.h"
/// @brief 名前付きの宣言要素を表すクラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
/// IEEE Std 1364-2001 26.6.8 Variables
/// IEEE Std 1364-2001 26.6.11 Named event
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbDecl :
  public ElbDeclBase
{
protected:
  
  /// @brief コンストラクタ
  ElbDecl();
  
  /// @brief デストラクタ
  virtual
  ~ElbDecl();

  
public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  virtual
  void
  set_signed() = 0;
  
  /// @brief 初期値の設定
  /// @param[in] expr 初期値
  /// @note このクラスでは何もしない．
  virtual
  void
  set_init(ElbExpr* expr);
  
  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  get_scalar() const = 0;
  
  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(tVpiScalarVal val) = 0;
  
  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  get_logic() const = 0;
  
  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const = 0;
  
  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val) = 0;

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const = 0;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val) = 0;
  
  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(int index) const = 0;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		tVpiScalarVal val) = 0;

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const = 0;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val) = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeclArray ElbDecl.h "ElbDecl.h"
/// @brief 名前付きの宣言要素を表すクラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
/// IEEE Std 1364-2001 26.6.8 Variables
/// IEEE Std 1364-2001 26.6.11 Named event
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbDeclArray :
  public ElbDeclBase
{
protected:
  
  /// @brief コンストラクタ
  ElbDeclArray();
  
  /// @brief デストラクタ
  virtual
  ~ElbDeclArray();

  
public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 要素の型を返す．
  virtual
  tVpiObjType
  elem_type() const = 0;
  
  /// @brief スカラー値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  tVpiScalarVal
  get_scalar(const vector<int>& index_array) const = 0;
  
  /// @brief スカラー値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_scalar(const vector<int>& index_array,
	     tVpiScalarVal val) = 0;
  
  /// @brief real 型の値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  double
  get_real(const vector<int>& index_array) const = 0;
  
  /// @brief real 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_real(const vector<int>& index_array,
	   double val) = 0;

  /// @brief bitvector 型の値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(const vector<int>& index_array,
		BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const = 0;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const vector<int>& index_array,
		const BitVector& val) = 0;
  
  /// @brief ビット選択値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(const vector<int>& index_array,
		int index) const = 0;

  /// @brief ビット値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(const vector<int>& index_array,
		int index,
		tVpiScalarVal val) = 0;

  /// @brief 範囲選択値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  get_partselect(const vector<int>& index_array,
		 int left,
		 int right,
		 BitVector& val) const = 0;

  /// @brief 範囲値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(const vector<int>& index_array,
		 int left,
		 int right,
		 const BitVector& val) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBDECL_H

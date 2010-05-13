#ifndef LIBYM_VERILOG_ELB_IMPL_EIDECLARRAY_H
#define LIBYM_VERILOG_ELB_IMPL_EIDECLARRAY_H

/// @file libym_verilog/elb_impl/EiDeclArray.h
/// @brief EiDeclArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiDeclArray.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "EiRange.h"

#include <ym_verilog/BitVector.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclArray EiDecl.h "EiDecl.h"
/// @brief 配列型の ElbDecl
//////////////////////////////////////////////////////////////////////
class EiDeclArray :
  public ElbDeclArray
{
  friend class EiFactory;
  
protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  EiDeclArray(ElbDeclHead* head,
	      const PtNamedBase* pt_item,
	      ymuint32 dim_size,
	      EiRange* range_array);

  /// @brief デストラクタ
  virtual
  ~EiDeclArray();
  

public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;
  

public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;
  
  /// @brief 名前の取得
  virtual
  const char*
  name() const;
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;
  
  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;
  
  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  virtual
  const VlDelay*
  delay() const;

  /// @brief 配列型オブジェクトの場合の次元数の取得
  virtual
  ymuint32
  dimension_list_size() const;
  
  /// @brief 範囲の取得
  /// @param[in] pos 位置 (0 <= pos < dimension_list_size())
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
  _left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _right_range() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 要素の型を返す．
  virtual
  tVpiObjType
  elem_type() const;

  /// @brief インデックスからオフセットを計算する．
  /// @param[in] index_array インデックスの配列
  ymuint32
  calc_offset(const vector<int>& index_array) const;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  int
  bit_offset(int index) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbDeclHead* mHead;
  
  // パース木の宣言要素
  const PtNamedBase* mPtItem;
  
  // dimension の配列
  EiRangeArray mRangeList;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayN EiDecl.h "EiDecl.h"
/// @brief 値を持たない EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayN :
  public EiDeclArray
{
  friend class EiFactory;
  
private:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  EiDeclArrayN(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint32 dim_size,
	       EiRange* range_array);
  
  /// @brief デストラクタ
  virtual
  ~EiDeclArrayN();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief スカラー値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  tVpiScalarVal
  get_scalar(const vector<int>& index_array) const;
    
  /// @brief スカラー値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_scalar(const vector<int>& index_array,
	     tVpiScalarVal val);

  /// @brief real 型の値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  double
  get_real(const vector<int>& index_array) const;
  
  /// @brief real 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_real(const vector<int>& index_array,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(const vector<int>& index_array,
		BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const vector<int>& index_array,
		const BitVector& val);
  
  /// @brief ビット選択値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(const vector<int>& index_array,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(const vector<int>& index_array,
		int index,
		tVpiScalarVal val);

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
		 BitVector& val) const;

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
		 const BitVector& val);
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayS EiDecl.h "EiDecl.h"
/// @brief スカラー値を持つ EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayS :
  public EiDeclArray
{
  friend class EiFactory;
  
private:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  /// @param[in] varray 値を納める配列
  EiDeclArrayS(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint32 dim_size,
	       EiRange* range_array,
	       tVpiScalarVal* varray);
  
  /// @brief デストラクタ
  virtual
  ~EiDeclArrayS();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief スカラー値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  tVpiScalarVal
  get_scalar(const vector<int>& index_array) const;
    
  /// @brief スカラー値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_scalar(const vector<int>& index_array,
	     tVpiScalarVal val);

  /// @brief real 型の値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  double
  get_real(const vector<int>& index_array) const;
  
  /// @brief real 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_real(const vector<int>& index_array,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(const vector<int>& index_array,
		BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const vector<int>& index_array,
		const BitVector& val);
  
  /// @brief ビット選択値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(const vector<int>& index_array,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(const vector<int>& index_array,
		int index,
		tVpiScalarVal val);

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
		 BitVector& val) const;

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
		 const BitVector& val);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 値
  tVpiScalarVal* mValArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayR EiDecl.h "EiDecl.h"
/// @brief 実数値を持つ EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayR :
  public EiDeclArray
{
  friend class EiFactory;
  
private:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  /// @param[in] varray 値を納める配列
  EiDeclArrayR(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint32 dim_size,
	       EiRange* range_array,
	       double* varray);
  
  /// @brief デストラクタ
  virtual
  ~EiDeclArrayR();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief スカラー値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  tVpiScalarVal
  get_scalar(const vector<int>& index_array) const;
    
  /// @brief スカラー値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_scalar(const vector<int>& index_array,
	     tVpiScalarVal val);

  /// @brief real 型の値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  double
  get_real(const vector<int>& index_array) const;
  
  /// @brief real 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_real(const vector<int>& index_array,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(const vector<int>& index_array,
		BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const vector<int>& index_array,
		const BitVector& val);
  
  /// @brief ビット選択値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(const vector<int>& index_array,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(const vector<int>& index_array,
		int index,
		tVpiScalarVal val);

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
		 BitVector& val) const;

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
		 const BitVector& val);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 値
  double* mValArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayV EiDecl.h "EiDecl.h"
/// @brief ビットベクタ値を持つ EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayV :
  public EiDeclArray
{
  friend class EiFactory;
  
private:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  /// @param[in] varray 値を納める配列
  EiDeclArrayV(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint32 dim_size,
	       EiRange* range_array,
	       BitVector* varray);
  
  /// @brief デストラクタ
  virtual
  ~EiDeclArrayV();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief スカラー値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  tVpiScalarVal
  get_scalar(const vector<int>& index_array) const;
    
  /// @brief スカラー値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_scalar(const vector<int>& index_array,
	     tVpiScalarVal val);

  /// @brief real 型の値を返す．
  /// @param[in] index_array インデックスの配列
  virtual
  double
  get_real(const vector<int>& index_array) const;
  
  /// @brief real 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_real(const vector<int>& index_array,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(const vector<int>& index_array,
		BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const vector<int>& index_array,
		const BitVector& val);
  
  /// @brief ビット選択値を返す．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(const vector<int>& index_array,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index_array インデックスの配列
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(const vector<int>& index_array,
		int index,
		tVpiScalarVal val);

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
		 BitVector& val) const;

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
		 const BitVector& val);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 値
  BitVector* mValArray;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_IMPL_EIDECLARRAY_H

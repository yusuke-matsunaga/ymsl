#ifndef LIBYM_VERILOG_ELB_IMPL_EIIMPNET_H
#define LIBYM_VERILOG_ELB_IMPL_EIIMPNET_H

/// @file libym_verilog/elb_impl/EiImpNet.h
/// @brief EiNet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiImpNet.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiImpNet EiImpNet.h "EiImpNet.h"
/// @brief 暗黙のネットを表すクラス
//////////////////////////////////////////////////////////////////////
class EiImpNet :
  public ElbDecl
{
  friend class EiFactory;
 
private:
  
  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木のプライマリ式
  EiImpNet(const VlNamedObj* parent,
	   const PtExpr* pt_expr,
	   tVpiNetType net_type);
  
  /// @brief デストラクタ
  virtual
  ~EiImpNet();
  

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

  /// @brief ビット幅を返す．
  /// @note このクラスは 1 を返す．
  virtual
  ymuint32
  bit_size() const;
  
  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const;

  /// @brief dimension list のサイズの取得
  /// @return dimension list のサイズ
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  dimension_list_size() const;
  
  /// @brief 範囲の取得
  /// @param[in] pos 位置 (0 <= pos < dimension_list_size())
  /// @note このクラスでは NULL を返す．
  virtual
  const VlRange*
  range(ymuint32 pos) const;
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  virtual
  void
  set_signed();
  
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
  
  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  get_scalar() const;
    
  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(tVpiScalarVal val);
  
  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;
  
  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val);
  
  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		tVpiScalarVal val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木のプライマリ式
  const PtExpr* mPtExpr;

  // ネット型
  tVpiNetType mNetType;
  
  // 値
  tVpiScalarVal mVal;
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_IMPL_EIIMPNET_H


/// @file libym_verilog/elb_impl/EiParameter.cc
/// @brief EiParameter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParameter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "EiFactory.h"
#include "EiParameter.h"
#include "ElbExpr.h"

#include <ym_verilog/pt/PtDecl.h>
#include <ym_verilog/pt/PtItem.h>
#include <ym_verilog/pt/PtMisc.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief parameter 宣言のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
ElbParamHead*
EiFactory::new_ParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head,
			 ElbExpr* left,
			 ElbExpr* right,
			 int left_val,
			 int right_val)
{
  EiParamHead* head = NULL;
  if ( left && right ) {
    void* p = mAlloc.get_memory(sizeof(EiParamHeadV));
    head = new (p) EiParamHeadV(parent, pt_head,
				left, right,
				left_val, right_val);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(EiParamHead));
    head = new (p) EiParamHead(parent, pt_head);
  }
  return head;
}

// @brief parameter 宣言を生成する．
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期割り当て式
// @param[in] is_local localparam の時 true
ElbParameter*
EiFactory::new_Parameter(ElbParamHead* head,
			 const PtNamedBase* pt_item,
			 bool is_local)
{
  EiParameter* param = NULL;
  void* p = NULL;
  
  switch ( head->type() ) {
  case kVpiParameter:
  case kVpiSpecParam:
    if ( is_local ) {
      p = mAlloc.get_memory(sizeof(EiLocalParam));
      param = new (p) EiLocalParam(head, pt_item);
    }
    else {
      p = mAlloc.get_memory(sizeof(EiParameter));
      param = new (p) EiParameter(head, pt_item);
    }
    break;
    
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  
  return param;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamHead
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
EiParamHead::EiParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head) :
  mParent(parent),
  mPtHead(pt_head)
{
}
  
// @brief デストラクタ
EiParamHead::~EiParamHead()
{
}

// @brief 型の取得
tVpiObjType
EiParamHead::type() const
{
  switch ( mPtHead->type() ) {
  case kPtDecl_Param:
  case kPtDecl_LocalParam:
    return kVpiParameter;
    
  case kPtDecl_SpecParam:
    return kVpiSpecParam;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return kVpiParameter;
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiParamHead::parent() const
{
  return mParent;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiParamHead::is_signed() const
{
  return mPtHead->is_signed();
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiParamHead::left_range() const
{
  return NULL;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiParamHead::right_range() const
{
  return NULL;
}

// @brief ビット幅を返す．
ymuint32
EiParamHead::bit_size() const
{
  switch ( mPtHead->data_type() ) {
  case kVpiVarInteger:
    return kVpiSizeInteger;
      
  case kVpiVarReal:
    return kVpiSizeReal;
      
  case kVpiVarTime:
    return kVpiSizeTime;
      
  default:
    break;
  }
  // int とみなす．
  return kVpiSizeInteger;
}

// @brief LSB からのオフセット値の取得
// @param[in] index インデックス
// @retval index の LSB からのオフセット index が範囲内に入っている．
// @retval -1 index が範囲外
int
EiParamHead::bit_offset(int index) const
{
  switch ( mPtHead->data_type() ) {
  case kVpiVarReal:
    return -1;
      
  case kVpiVarTime:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
      return index;
    }
    return -1;
      
  case kVpiVarInteger:
  default:
    // int とみなす．
    if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
      return index;
    }
    return -1;
  }
  return -1;
}

// @brief データ型の取得
tVpiVarType
EiParamHead::data_type() const
{
  return mPtHead->data_type();
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamHeadV
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
EiParamHeadV::EiParamHeadV(const VlNamedObj* parent,
			   const PtDeclHead* pt_head,
			   ElbExpr* left,
			   ElbExpr* right,
			   int left_val,
			   int right_val) :
  EiParamHead(parent, pt_head)
{
  mRange.set(left, right, left_val, right_val);
}
  
// @brief デストラクタ
EiParamHeadV::~EiParamHeadV()
{
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiParamHeadV::left_range() const
{
  return mRange.left_range();
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiParamHeadV::right_range() const
{
  return mRange.right_range();
}

// @brief ビット幅を返す．
ymuint32
EiParamHeadV::bit_size() const
{
  return mRange.size();
}

// @brief LSB からのオフセット値の取得
// @param[in] index インデックス
// @retval index の LSB からのオフセット index が範囲内に入っている．
// @retval -1 index が範囲外
int
EiParamHeadV::bit_offset(int index) const
{
  return mRange.offset(index);
}


//////////////////////////////////////////////////////////////////////
// クラス EiParameter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiParameter::EiParameter(ElbParamHead* head,
			 const PtNamedBase* pt_item) :
  mHead(head),
  mPtItem(pt_item),
  mExpr(NULL)
{
}

// @brief デストラクタ
EiParameter::~EiParameter()
{
}

// @brief 型の取得
// @return vpi_user.h で定義された型 (vpiModule など)
tVpiObjType
EiParameter::type() const
{
  return mHead->type();
}

// @brief ファイル位置の取得
FileRegion
EiParameter::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiParameter::parent() const
{
  return mHead->parent();
}
  
// @brief 名前の取得
const char*
EiParameter::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
tVpiValueType
EiParameter::value_type() const
{
#warning "TODO: 意味を確認"
  switch ( type() ) {
  case kVpiNet:
  case kVpiReg:
    if ( is_signed() ) {
      return pack(kVpiValueSS, bit_size());
    }
    else {
      return pack(kVpiValueUS, bit_size());
    }
    
  case kVpiIntegerVar:
    return kVpiValueInteger;
    
  case kVpiRealVar:
    return kVpiValueReal;
    
  case kVpiTimeVar:
    return kVpiValueTime;
    
  case kVpiParameter:
  case kVpiSpecParam:
  case kVpiConstant:
    // ここにはこない
    assert_not_reached(__FILE__, __LINE__);
    break;
    
  default:
    // 上記以外は形無し
    break;
  }
  
  return kVpiValueNone;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiParameter::is_signed() const
{
  return mHead->is_signed();
}

// @brief ビット幅を返す．
ymuint32
EiParameter::bit_size() const
{
  return mHead->bit_size();
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiParameter::data_type() const
{
  return mHead->data_type();
}
  
// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
EiParameter::init_value() const
{
  return mExpr;
}
  
// @brief localparam のときに true 返す．
// @note このクラスでは false を返す．
bool
EiParameter::is_local_param() const
{
  return false;
}

// @brief 初期値の設定
// @param[in] expr 初期値
void
EiParameter::set_expr(ElbExpr* expr)
{
  mExpr = expr;
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiParameter::_left_range() const
{
  return mHead->left_range();
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiParameter::_right_range() const
{
  return mHead->right_range();
}

// @brief int 型の値を返す．
// @param[out] val 結果を格納する変数
// @return 整数値に変換できたら true を返す．
bool
EiParameter::eval_int(int& val) const
{
  return mExpr->eval_int(val);
}

// @brief スカラー値を返す．
tVpiScalarVal
EiParameter::eval_scalar() const
{
  return mExpr->eval_scalar();
}

// @brief 論理値を返す．
tVpiScalarVal
EiParameter::eval_logic() const
{
  return mExpr->eval_logic();
}

// @brief 論理値を返す．
bool
EiParameter::eval_bool() const
{
  return mExpr->eval_bool();
}

// @brief real 型の値を返す．
double
EiParameter::eval_real() const
{
  return mExpr->eval_real();
}

// @brief VlTime 型の値を返す．
// @param[out] val 結果を格納する変数
// @return VlTime 値に変換できたら true を返す．
// @note eval_bitvector() の結果から変換する．
bool
EiParameter::eval_time(VlTime& val) const
{
  return mExpr->eval_time(val);
}

// @brief bitvector 型の値を返す．
void
EiParameter::eval_bitvector(BitVector& bitvector,
			    tVpiValueType req_type) const
{
  mExpr->eval_bitvector(bitvector, req_type);
}

// @brief LSB からのオフセット値の取得
// @param[in] index インデックス
// @retval index の LSB からのオフセット index が範囲内に入っている．
// @retval -1 index が範囲外
int
EiParameter::bit_offset(int index) const
{
  return mHead->bit_offset(index);
}


//////////////////////////////////////////////////////////////////////
// クラス EiLocalParam
////////////////////////////////////////////////////////////////////// 
  
// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiLocalParam::EiLocalParam(ElbParamHead* head,
			   const PtNamedBase* pt_item) :
  EiParameter(head, pt_item)
{
}
  
// @brief デストラクタ
EiLocalParam::~EiLocalParam()
{
}
  
// @brief localparam のときに true 返す．
// @note このクラスでは true を返す．
bool
EiLocalParam::is_local_param() const
{
  return true;
}


END_NAMESPACE_YM_VERILOG

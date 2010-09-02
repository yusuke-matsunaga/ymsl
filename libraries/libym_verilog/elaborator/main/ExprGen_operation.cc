
/// @file libym_verilog/elaborator/ExprGen_opration.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_operation.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtOpr から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] env 生成時の環境
ElbExpr*
ExprGen::instantiate_opr(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  tVpiOpType op_type = pt_expr->opr_type();
  ymuint opr_size = pt_expr->operand_num();
  
  ElbExpr* opr0 = NULL;
  ElbExpr* opr1 = NULL;
  ElbExpr* opr2 = NULL;
  ElbExpr* expr = NULL;
  bool real_check = false;
  switch ( op_type ) {
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    error_illegal_edge_descriptor(pt_expr);
    return NULL;

  case kVpiBitNegOp:
  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryXorOp:
  case kVpiUnaryXNorOp: real_check = true;
  case kVpiPlusOp:
  case kVpiMinusOp:
  case kVpiNotOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    if ( !opr0 ) {
      return NULL;
    }
    if ( real_check && opr0->value_type() == kVpiValueReal ) {
      error_illegal_real_type(pt_expr->operand(0));
      return NULL;
    }
    return factory().new_UnaryOp(pt_expr, op_type, opr0);
    
  case kVpiBitAndOp:
  case kVpiBitOrOp:
  case kVpiBitXNorOp:
  case kVpiBitXorOp:
  case kVpiLShiftOp:
  case kVpiRShiftOp:
  case kVpiArithLShiftOp:
  case kVpiArithRShiftOp: real_check = true;
  case kVpiAddOp:
  case kVpiSubOp:
  case kVpiMultOp:
  case kVpiDivOp:
  case kVpiModOp:
  case kVpiPowerOp:
  case kVpiLogAndOp:
  case kVpiLogOrOp:
  case kVpiCaseEqOp:
  case kVpiCaseNeqOp:
  case kVpiEqOp:
  case kVpiNeqOp:
  case kVpiGeOp:
  case kVpiGtOp:
  case kVpiLeOp:
  case kVpiLtOp:
    assert_cond(opr_size == 2, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    if ( !opr0 || !opr1 ) {
      return NULL;
    }
    if ( real_check ) {
      if ( opr0->value_type() == kVpiValueReal ) {
	error_illegal_real_type(pt_expr->operand(0));
	return NULL;
      }
      if ( opr1->value_type() == kVpiValueReal ) {
	error_illegal_real_type(pt_expr->operand(1));
	return NULL;
      }
    }
    expr = factory().new_BinaryOp(pt_expr, op_type, opr0, opr1);
    break;

  case kVpiConditionOp:
  case kVpiMinTypMaxOp:
    assert_cond(opr_size == 3, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    opr2 = instantiate_expr(parent, env, pt_expr->operand(2));
    if ( !opr0 || !opr1 || !opr2 ) {
      return NULL;
    }
    expr = factory().new_TernaryOp(pt_expr, op_type, opr0, opr1, opr2);
    break;

  case kVpiConcatOp:
    {
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for (ymuint i = 0; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type() == kVpiValueReal ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i] = expr1;
      }
      
      expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();
    }
    break;
    
  case kVpiMultiConcatOp:
    {
      const PtExpr* pt_expr0 = pt_expr->operand(0);
      ElbExpr* rep = instantiate_constant_expr(parent, pt_expr0);
      if ( !rep ) {
	return NULL;
      }
      int rep_num;
      if ( !expr_to_int(rep, rep_num) ) {
	return NULL;
      }
      ElbExpr** opr_list = factory().new_ExprList(opr_size - 1);
      for (ymuint i = 1; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type() == kVpiValueReal ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i - 1] = expr1;
      }
      expr = factory().new_MultiConcatOp(pt_expr, rep, rep_num,
					 opr_size - 1, opr_list);
      expr->set_selfsize();
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    return NULL;
  }
  
  // attribute instance の生成
  //instantiate_attribute(pt_expr->attr_top(), false, expr);
  
  return expr;
}

END_NAMESPACE_YM_VERILOG

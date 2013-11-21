
/// @file DopBase.cc
/// @brief DopBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DopBase.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] mgr AtpgMgr
DetectOp*
new_DopBase(AtpgMgr& mgr)
{
  return new DopBase(mgr._fault_mgr());
}


//////////////////////////////////////////////////////////////////////
// クラス DopBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr FaultMgr
DopBase::DopBase(FaultMgr& fmgr) :
  mMgr(fmgr)
{
}

// @brief デストラクタ
DopBase::~DopBase()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
DopBase::operator()(TpgFault* f,
		    TestVector* tv)
{
  mMgr.set_status(f, kFsDetected);
}

END_NAMESPACE_YM_SATPG

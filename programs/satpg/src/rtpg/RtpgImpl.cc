
/// @file RtpgImpl.cc
/// @brief Rtpg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "RtpgImpl.h"
#include "AtpgMgr.h"
#include "FaultMgr.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "Fsim.h"
#include "RtpgStats.h"
#include "Fop2Rtpg.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

Rtpg*
new_Rtpg(AtpgMgr& mgr)
{
  return new RtpgImpl(mgr._fault_mgr(), mgr._tv_mgr(),
		      mgr._tv_list(), mgr._fsim());
}


//////////////////////////////////////////////////////////////////////
// クラス RtpgImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fmgr 故障マネージャ
// @param[in] tvmgr テストベクタマネージャ
// @param[in] tvlist テストベクタのリスト
// @param[in] fsim 故障シミュレータ
RtpgImpl::RtpgImpl(FaultMgr& fmgr,
		   TvMgr& tvmgr,
		   vector<TestVector*>& tvlist,
		   Fsim& fsim) :
  mFaultMgr(fmgr),
  mTvMgr(tvmgr),
  mTvList(tvlist),
  mFsim(fsim)
{
}

// @brief デストラクタ
RtpgImpl::~RtpgImpl()
{
}

// @brief 乱数生成器を初期化する．
// @param[in] seed 乱数の種
void
RtpgImpl::init(ymuint32 seed)
{
  mRandGen.init(seed);
}

// @brief RTPGを行なう．
// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
// @param[in] max_i 故障検出できないシミュレーション回数の上限
// @param[in] max_pat 最大のパタン数
// @param[in] stats 実行結果の情報を格納する変数
void
RtpgImpl::run(ymuint min_f,
	      ymuint max_i,
	      ymuint max_pat,
	      RtpgStats& stats)
{
  StopWatch local_timer;

  local_timer.start();

  ymuint fnum = mFaultMgr.remain_list().size();
  ymuint undet_i = 0;
  ymuint epat_num = 0;
  ymuint total_det_count = 0;

  TestVector* tv_array[kPvBitLen];
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    tv_array[i] = mTvMgr.new_vector();
  }

  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  Fop2Rtpg op(mFsim, mFaultMgr);
  vector<FsimOp2*> op_list(1, &op);

  ymuint pat_num = 0;
  for ( ; ; ) {
    if ( pat_num < max_pat ) {
      TestVector* tv = tv_array[cur_array.size()];
      tv->set_from_random(mRandGen);
      cur_array.push_back(tv);
      ++ pat_num;
      if ( cur_array.size() < kPvBitLen ) {
	continue;
      }
    }
    else if ( cur_array.empty() ) {
      break;
    }

    op.clear_count();

    mFsim.ppsfp(cur_array, op_list);

    ymuint det_count = 0;
    for (ymuint i = 0; i < cur_array.size(); ++ i) {
      ymuint det_count1 = op.count(i);
      if ( det_count1 > 0 ) {
	det_count += det_count1;
	TestVector* tv = cur_array[i];
	mTvList.push_back(tv);
	tv_array[i] = mTvMgr.new_vector();
	++ epat_num;
      }
    }
    cur_array.clear();

    total_det_count += det_count;

    if ( total_det_count == fnum ) {
      // すべての故障を検出した．
      break;
    }
    if ( det_count < min_f ) {
      // 検出故障数の下限を下回った
      break;
    }
    if ( det_count > 0 ) {
      undet_i = 0;
    }
    else {
      ++ undet_i;
      if ( undet_i > max_i ) {
	// 未検出の回数が max_i を越えた．
	break;
      }
    }
  }

  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mTvMgr.delete_vector(tv_array[i]);
  }

  local_timer.stop();
  USTime time = local_timer.time();

  stats.set(total_det_count, pat_num, epat_num, time);
}

END_NAMESPACE_YM_SATPG
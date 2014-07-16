
/// @file SatEngineBase.cc
/// @brief SatEngineBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineBase.h"

#include "DetectOp.h"
#include "UntestOp.h"
#include "DtpgStats.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "BackTracer.h"
#include "LitMap.h"
#include "logic/SatSolver.h"
#include "logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_gvar(SatSolver& solver,
	 TpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId gvar = solver.new_var();
  node->set_gvar(gvar);
}

// @brief ノードに正常回路用の変数を設定する．
// @param[in] solver SAT ソルバー
// @param[in] node 対象のノード
void
set_fvar(SatSolver& solver,
	 TpgNode* node)
{
  // ノードそのものに割り当てる．
  VarId fvar = solver.new_var();
  VarId dvar = solver.new_var();
  node->set_fvar(fvar, dvar);
}

// バッファの入出力の関係を表す CNF 式を生成する．
inline
void
make_buff_cnf(SatSolver& solver,
	      Literal i,
	      Literal o)
{
  solver.add_clause( i, ~o);
  solver.add_clause(~i,  o);
}

// 2入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and2_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause(~i0, ~i1, o);
}

// 3入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and3_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal i2,
	      Literal o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause( i2, ~o);
  solver.add_clause(~i0, ~i1, ~i2, o);
}

// 4入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and4_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal i2,
	      Literal i3,
	      Literal o)
{
  solver.add_clause( i0, ~o);
  solver.add_clause( i1, ~o);
  solver.add_clause( i2, ~o);
  solver.add_clause( i3, ~o);
  solver.add_clause(~i0, ~i1, ~i2, ~i3, o);
}

// 多入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_and_cnf(SatSolver& solver,
	     const LitMap& litmap,
	     Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: assert_not_reached(__FILE__, __LINE__); break;
  case 1: make_buff_cnf(solver, litmap.input(0), output); return;
  case 2: make_and2_cnf(solver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_and3_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_and4_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  vector<Literal> tmp(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    solver.add_clause(litmap.input(i), ~output);
    tmp[i] = ~litmap.input(i);
  }
  tmp[n] = output;
  solver.add_clause(tmp);
}

// 2入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or2_cnf(SatSolver& solver,
	     Literal i0,
	     Literal i1,
	     Literal o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause( i0,  i1, ~o);
}

// 3入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or3_cnf(SatSolver& solver,
	     Literal i0,
	     Literal i1,
	     Literal i2,
	     Literal o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause(~i2,  o);
  solver.add_clause( i0,  i1, i2, ~o);
}

// 4入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or4_cnf(SatSolver& solver,
	     Literal i0,
	     Literal i1,
	     Literal i2,
	     Literal i3,
	     Literal o)
{
  solver.add_clause(~i0,  o);
  solver.add_clause(~i1,  o);
  solver.add_clause(~i2,  o);
  solver.add_clause(~i3,  o);
  solver.add_clause( i0,  i1, i2, i3, ~o);
}

// 多入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_or_cnf(SatSolver& solver,
	    const LitMap& litmap,
	    Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: assert_not_reached(__FILE__, __LINE__); break;
  case 1: make_buff_cnf(solver, litmap.input(0), output); return;
  case 2: make_or2_cnf(solver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_or3_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  case 4: make_or4_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), litmap.input(3), output); return;
  default: break;
  }

  vector<Literal> tmp(n + 1);
  for (ymuint i = 0; i < n; ++ i) {
    solver.add_clause(~litmap.input(i), output);
    tmp[i] = litmap.input(i);
  }
  tmp[n] = ~output;
  solver.add_clause(tmp);
}

// 2入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_xor2_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal o)
{
  solver.add_clause( i0, ~i1,  o);
  solver.add_clause(~i0,  i1,  o);
  solver.add_clause( i0,  i1, ~o);
  solver.add_clause(~i0, ~i1, ~o);
}

// 3入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_xor3_cnf(SatSolver& solver,
	      Literal i0,
	      Literal i1,
	      Literal i2,
	      Literal o)
{
  solver.add_clause(~i0,  i1,  i2,  o);
  solver.add_clause( i0, ~i1,  i2,  o);
  solver.add_clause( i0,  i1, ~i2,  o);
  solver.add_clause( i0,  i1,  i2, ~o);
  solver.add_clause( i0, ~i1, ~i2, ~o);
  solver.add_clause(~i0,  i1, ~i2, ~o);
  solver.add_clause(~i0, ~i1,  i2, ~o);
  solver.add_clause(~i0, ~i1, ~i2,  o);
}

// 多入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
inline
void
make_xor_cnf(SatSolver& solver,
	     const LitMap& litmap,
	     Literal output)
{
  ymuint n = litmap.input_size();
  switch ( n ) {
  case 0: assert_not_reached(__FILE__, __LINE__); break;
  case 1: make_buff_cnf(solver, litmap.input(0), output); return;
  case 2: make_xor2_cnf(solver, litmap.input(0), litmap.input(1), output); return;
  case 3: make_xor3_cnf(solver, litmap.input(0), litmap.input(1), litmap.input(2), output); return;
  default: break;
  }

  VarId tmp_var = solver.new_var();
  Literal tmp_lit(tmp_var, false);
  make_xor2_cnf(solver, litmap.input(0), litmap.input(1), tmp_lit);

  for (ymuint i = 2; i < n; ++ i) {
    Literal tmp_out;
    if ( i == n - 1 ) {
      tmp_out = output;
    }
    else {
      VarId new_var = solver.new_var();
      tmp_out = Literal(new_var, false);
    }
    make_xor2_cnf(solver, litmap.input(i), tmp_lit, tmp_out);
    tmp_lit = tmp_out;
  }
}

END_NONAMESPACE


// @brief コンストラクタ
SatEngineBase::SatEngineBase(const string& sat_type,
			     const string& sat_option,
			     ostream* sat_outp,
			     ymuint max_id,
			     BackTracer& bt,
			     DetectOp& dop,
			     UntestOp& uop) :
  mSatType(sat_type),
  mSatOption(sat_option),
  mSatOutP(sat_outp),
  mMaxNodeId(max_id),
  mBackTracer(bt),
  mDetectOp(dop),
  mUntestOp(uop)
{
  mNemesis = false;
  mExtNemesis = false;
  mTgGrasp = false;
  mExtTgGrasp = false;
  mUseDominator = false;
  mTimerEnable = false;
}

// @brief デストラクタ
SatEngineBase::~SatEngineBase()
{
}

// @brief オプションを設定する．
void
SatEngineBase::set_option(const string& option_str)
{
  for (string::size_type next = 0; ; ++ next) {
    string::size_type pos = option_str.find(':', next);
    if ( pos == next ) {
      continue;
    }
    string option = option_str.substr(next, pos - next);
    if ( option == "NEMESIS" ) {
      mNemesis = true;
      mExtNemesis = false;
      mTgGrasp = false;
      mExtTgGrasp = false;
    }
    else if ( option == "EXT-NEMESIS" ) {
      mNemesis = true;
      mExtNemesis = true;
      mTgGrasp = false;
      mExtTgGrasp = false;
    }
    else if ( option == "TG-GRASP" ) {
      mNemesis = false;
      mTgGrasp = true;
      mExtTgGrasp = false;
    }
    else if ( option == "EXT-TG-GRASP" ) {
      mNemesis = false;
      mTgGrasp = true;
      mExtTgGrasp = true;
    }
    else if ( option == "DOM" ) {
      mUseDominator = true;
    }
    else if ( option == "NODOM" ) {
      mUseDominator = false;
    }
    if ( pos == string::npos ) {
      break;
    }
    next = pos;
  }
}

// @brief 統計情報をクリアする．
void
SatEngineBase::clear_stats()
{
  mStats.mCnfGenCount = 0;
  mStats.mCnfGenTime.set(0.0, 0.0, 0.0);

  mStats.mDetCount = 0;
  mStats.mDetTime.set(0.0, 0.0, 0.0);
  clear_sat_stats(mStats.mDetStats);
  clear_sat_stats(mStats.mDetStatsMax);

  mStats.mRedCount = 0;
  mStats.mRedTime.set(0.0, 0.0, 0.0);
  clear_sat_stats(mStats.mRedStats);
  clear_sat_stats(mStats.mRedStatsMax);

  mStats.mPartRedCount = 0;
  mStats.mPartRedTime.set(0.0, 0.0, 0.0);
  clear_sat_stats(mStats.mPartRedStats);
  clear_sat_stats(mStats.mPartRedStatsMax);

  mStats.mAbortCount = 0;
  mStats.mAbortTime.set(0.0, 0.0, 0.0);
}

// @brief 統計情報を得る．
// @param[in] stats 結果を格納する構造体
void
SatEngineBase::get_stats(DtpgStats& stats) const
{
  stats = mStats;
}

// @breif 時間計測を制御する．
void
SatEngineBase::timer_enable(bool enable)
{
  mTimerEnable = enable;
}

BEGIN_NONAMESPACE

struct Lt
{
  bool
  operator()(TpgNode* left,
	     TpgNode* right)
  {
    return left->output_id2() < right->output_id2();
  }
};

END_NONAMESPACE

// @brief 故障位置を与えてその TFO の TFI リストを作る．
// @param[in] solver SAT ソルバ
// @param[in] fnode_list 故障位置のノードのリスト
//
// 結果は mTfoList に格納される．
// 故障位置の TFO が mTfoList の [0: mTfoEnd1 - 1] に格納される．
void
SatEngineBase::mark_region(SatSolver& solver,
			   const vector<TpgNode*>& fnode_list)
{
  mMarkArray.clear();
  mMarkArray.resize(mMaxNodeId, 0U);

  mTfoList.clear();
  mTfoList.reserve(mMaxNodeId);

  mInputList.clear();
  mOutputList.clear();

  // 故障のあるノードの TFO を mTfoList に入れる．
  // TFO の TFI のノードを mTfiList に入れる．
  ymuint nf = fnode_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    TpgNode* fnode = fnode_list[i];
    if ( !tfo_mark(fnode) ) {
      set_tfo_mark(fnode);
      if ( fnode->is_input() ) {
	mInputList.push_back(fnode);
      }
      set_gvar(solver, fnode);
      set_fvar(solver, fnode);
    }
  }

  for (ymuint rpos = 0; rpos < mTfoList.size(); ++ rpos) {
    TpgNode* node = mTfoList[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* fonode = node->active_fanout(i);
      if ( !tfo_mark(fonode) ) {
	set_tfo_mark(fonode);
	set_gvar(solver, fonode);
	set_fvar(solver, fonode);
      }
    }
  }

  mTfoEnd = mTfoList.size();
  for (ymuint rpos = 0; rpos < mTfoList.size(); ++ rpos) {
    TpgNode* node = mTfoList[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* finode = node->fanin(i);
      if ( !tfo_mark(finode) && !tfi_mark(finode) ) {
	set_tfi_mark(finode);
	set_gvar(solver, finode);
      }
    }
  }

  sort(mOutputList.begin(), mOutputList.end(), Lt());
}

// @brief 節の作成用の作業領域の使用を開始する．
// @param[in] exp_size 予想されるサイズ
void
SatEngineBase::tmp_lits_begin(ymuint exp_size)
{
  mTmpLits.clear();
  if ( exp_size > 0 ) {
    mTmpLits.reserve(exp_size);
  }
}

// @brief 作業領域にリテラルを追加する．
void
SatEngineBase::tmp_lits_add(Literal lit)
{
  mTmpLits.push_back(lit);
}

// @brief 作業領域の冊を SAT ソルバに加える．
void
SatEngineBase::tmp_lits_end(SatSolver& solver)
{
  solver.add_clause(mTmpLits);
}

// @brief タイマーをスタートする．
void
SatEngineBase::cnf_begin()
{
  timer_start();
}

// @brief タイマーを止めて CNF 作成時間に加える．
void
SatEngineBase::cnf_end()
{
  USTime time = timer_stop();
  mStats.mCnfGenTime += time;
  ++ mStats.mCnfGenCount;
}

// @brief 時間計測を開始する．
void
SatEngineBase::timer_start()
{
  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }
}

/// @brief 時間計測を終了する．
USTime
SatEngineBase::timer_stop()
{
  USTime time(0, 0, 0);
  if ( mTimerEnable ) {
    mTimer.stop();
    time = mTimer.time();
  }
  return time;
}

// @brief 正常回路のノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
void
SatEngineBase::make_gnode_cnf(SatSolver& solver,
			      TpgNode* node)
{
  Literal output(node->gvar(), false);
  make_node_cnf(solver, node, GvarLitMap(node), output);
}

// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
void
SatEngineBase::make_fnode_cnf(SatSolver& solver,
			      TpgNode* node)
{
  if ( !node->has_flt_var() ) {
    Literal output(node->fvar(), false);
    make_node_cnf(solver, node, FvarLitMap(node), output);
    return;
  }
  ymuint ni = node->fanin_num();
  vector<VarId> ivars(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    VarId f0_var = node->if0var(i);
    VarId f1_var = node->if1var(i);
    if ( f0_var == kVarIdIllegal ) {
      if ( f1_var == kVarIdIllegal ) {
	ivars[i] = inode->fvar();
      }
      else {
	VarId tmp_var = solver.new_var();
	make_flt1_cnf(solver, inode->fvar(), f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
    else {
      if ( f1_var == kVarIdIllegal ) {
	VarId tmp_var = solver.new_var();
	make_flt0_cnf(solver, inode->fvar(), f0_var, tmp_var);
	ivars[i] = tmp_var;
      }
      else {
	VarId tmp_var = solver.new_var();
	make_flt01_cnf(solver, inode->fvar(), f0_var, f1_var, tmp_var);
	ivars[i] = tmp_var;
      }
    }
  }

  VarId f0_var = node->of0var();
  VarId f1_var = node->of1var();
  VarId ovar = node->fvar();
  if ( f0_var == kVarIdIllegal ) {
    if ( f1_var == kVarIdIllegal ) {
      ;
    }
    else {
      ovar = solver.new_var();
      make_flt1_cnf(solver, ovar, f1_var, node->fvar());
    }
  }
  else {
    if ( f1_var == kVarIdIllegal ) {
      ovar = solver.new_var();
      make_flt0_cnf(solver, ovar, f0_var, node->fvar());
    }
    else {
      ovar = solver.new_var();
      make_flt01_cnf(solver, ovar, f0_var, f1_var, node->fvar());
    }
  }
  Literal output(ovar, false);

  if ( node->is_input() ) {
    Literal glit(node->gvar(), false);
    make_buff_cnf(solver, glit, output);
  }
  else {
    make_node_cnf(solver, node, VectLitMap(ivars), output);
  }
}

// @brief 故障ゲートの CNF を作る．
void
SatEngineBase::make_fault_cnf(SatSolver& solver,
			      TpgFault* fault)
{
  TpgNode* node = fault->node();
  int fval = fault->val();

  if ( fault->is_output_fault() ) {
    Literal flit(node->fvar(), false);
    if ( fval == 0 ) {
      solver.add_clause(~flit);
    }
    else {
      solver.add_clause(flit);
    }
  }
  else {
    ymuint fpos = fault->pos();

    Literal output(node->fvar(), false);

    // fpos 以外の入力を ivars[] に入れる．
    ymuint ni = node->fanin_num();
    vector<VarId> ivars;
    ivars.reserve(ni - 1);
    for (ymuint i = 0; i < ni; ++ i) {
      if ( i == fpos ) {
	continue;
      }
      TpgNode* inode = node->fanin(i);
      ivars.push_back(inode->gvar());
    }

    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kTgGateNand:
      output = ~output;
      // わざと次に続く

    case kTgGateAnd:
      assert_cond( fval == 1, __FILE__, __LINE__);
      make_and_cnf(solver, VectLitMap(ivars), output);
      break;

    case kTgGateNor:
      output = ~output;
      // わざと次に続く

    case kTgGateOr:
      assert_cond( fval == 0, __FILE__, __LINE__);
      make_or_cnf(solver, VectLitMap(ivars), output);
      break;

    case kTgGateXnor:
      output = ~output;
      // わざと次に続く

    case kTgGateXor:
      if ( fval == 1 ) {
	output = ~output;
      }
      make_xor_cnf(solver, VectLitMap(ivars), output);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

void
SatEngineBase::make_dchain_cnf(SatSolver& solver,
			       TpgNode* node,
			       TpgFault* fault)
{
  Literal glit(node->gvar(), false);
  Literal flit(node->fvar(), false);
  Literal dlit(node->dvar(), false);

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに dlit が 1 の時，正常回路と故障回路で異なっていなければならない．
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause( glit,  flit, ~dlit);

  if ( mNemesis ) {
    if ( !node->is_output() ) {
      // dlit が 1 の時，ファンアウトの dlit が最低1つは 1 でなければならない．
      ymuint nfo = node->active_fanout_num();
      tmp_lits_begin(nfo + 1);
      tmp_lits_add(~dlit);
      for (ymuint j = 0; j < nfo; ++ j) {
	TpgNode* onode = node->active_fanout(j);
	tmp_lits_add(Literal(onode->dvar(), false));
      }
      tmp_lits_end(solver);

      if ( mExtNemesis ) {
	for (TpgNode* idom = node->imm_dom();
	     idom != NULL; idom = idom->imm_dom() ) {
	  Literal idlit(idom->dvar(), false);
	  solver.add_clause(~dlit, idlit);
	}
      }
    }
  }
  if ( mTgGrasp ) {
    // XOR(glit, flit) -> dlit を追加する．
    // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);

    if ( mExtTgGrasp ) {
      if ( node != fault->node() ) {
	make_dlit_cnf(solver, node);
      }
    }
  }
}

void
SatEngineBase::make_dchain_cnf(SatSolver& solver,
			       TpgNode* node)
{
  Literal glit(node->gvar(), false);
  Literal flit(node->fvar(), false);
  Literal dlit(node->dvar(), false);

  // 正常回路と故障回路で異なっているとき dlit が 1 となる．
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause( glit,  flit, ~dlit);

  if ( mTgGrasp ) {
    // XOR(glit, flit, dlit) を追加する．
    // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．

    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);

    if ( mExtTgGrasp ) {
      // 全てのファンインに故障差が伝搬していなければ
      // このゲートの出力にも故障差は伝搬しない．
      // ただし，このゲートの出力に故障がある場合を
      // 考慮しなければならない．
      if ( node->has_flt_var() ) {
	ymuint ni = node->fanin_num();
	tmp_lits_begin(ni * 3 + 3);
	tmp_lits_add(~dlit);
	if ( node->of0var() != kVarIdIllegal ) {
	  tmp_lits_add(Literal(node->of0var(), false));
	}
	if ( node->of1var() != kVarIdIllegal ) {
	  tmp_lits_add(Literal(node->of1var(), false));
	}
	for (ymuint i = 0; i < ni; ++ i) {
	  TpgNode* inode = node->fanin(i);
	  if ( inode->has_fvar() ) {
	    Literal idlit(inode->dvar(), false);
	    tmp_lits_add(idlit);
	  }
	  if ( node->if0var(i) != kVarIdIllegal ) {
	    tmp_lits_add(Literal(node->if0var(i), false));
	  }
	  if ( node->if1var(i) != kVarIdIllegal ) {
	    tmp_lits_add(Literal(node->if1var(i), false));
	  }
	}
      }
      else {
	make_dlit_cnf(solver, node);
      }
    }
  }

  if ( mNemesis ) {
    if ( !node->is_output() ) {
      ymuint nfo = node->active_fanout_num();
      tmp_lits_begin(nfo + 1);
      tmp_lits_add(~dlit);
      for (ymuint j = 0; j < nfo; ++ j) {
	TpgNode* onode = node->active_fanout(j);
	tmp_lits_add(Literal(onode->dvar(), false));
      }
      tmp_lits_end(solver);
    }
  }
}

// @brief ノードの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] node 対象のノード
// @param[in] litmap 入出力のリテラルを保持するクラス
void
SatEngineBase::make_node_cnf(SatSolver& solver,
			     TpgNode* node,
			     const LitMap& litmap,
			     Literal output)
{
  if ( node->is_input() ) {
    return;
  }

  if ( node->is_output() ) {
    Literal input = litmap.input(0);
    make_buff_cnf(solver, input, output);
    return;
  }

  if ( node->is_logic() ) {
    make_gate_cnf(solver, node->gate_type(), litmap, output);
    return;
  }
}

// @brief ゲートの入出力の関係を表す CNF を作る．
// @param[in] solver SATソルバ
// @param[in] type ゲートの種類
// @param[in] litmap 入出力のリテラルを保持するクラス
void
SatEngineBase::make_gate_cnf(SatSolver& solver,
			     tTgGateType type,
			     const LitMap& litmap,
			     Literal output)
{
  ymuint ni = litmap.input_size();
  switch ( type ) {
  case kTgGateNot:
    output = ~output;
    // わざと次に続く

  case kTgGateBuff:
    make_buff_cnf(solver, litmap.input(0), output);
    break;

  case kTgGateNand:
    output = ~output;
    // わざと次に続く
  case kTgGateAnd:
    make_and_cnf(solver, litmap, output);
    break;

  case kTgGateNor:
    output = ~output;
    // わざと次に続く

  case kTgGateOr:
    make_or_cnf(solver, litmap, output);
    break;

  case kTgGateXnor:
    output = ~output;
    // わざと次に続く

  case kTgGateXor:
    make_xor_cnf(solver, litmap, output);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

// @brief ノードの故障差関数を表すCNFを作る．
void
SatEngineBase::make_dlit_cnf(SatSolver& solver,
			     TpgNode* node)
{
  Literal dlit(node->dvar());

  ymuint ni = node->fanin_num();

  if ( node->is_logic() ) {
    // ゲートの種類ごとの処理
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      // なにもしない．
      break;

    case kTgGateAnd:
    case kTgGateNand:
      for (ymuint i = 0; i < ni; ++ i) {
	TpgNode* inode = node->fanin(i);
	if ( inode->has_fvar() ) {
	  // side input が 0 かつ故障差が伝搬していなければ dlit も 0
	  Literal iglit(inode->gvar(), false);
	  Literal iflit(inode->fvar(), false);
	  solver.add_clause(iglit, iflit, ~dlit);
	}
	else {
	  // side input が 0 なら dlit も 0
	  Literal iglit(inode->gvar(), false);
	  solver.add_clause(iglit, ~dlit);
	}
      }
      break;

    case kTgGateOr:
    case kTgGateNor:
      for (ymuint i = 0; i < ni; ++ i) {
	TpgNode* inode = node->fanin(i);
	if ( inode->has_fvar() ) {
	  // side input が 1 かつ故障差が伝搬していなければ dlit も 0
	  Literal iglit(inode->gvar(), false);
	  Literal iflit(inode->fvar(), false);
	  solver.add_clause(~iglit, ~iflit, ~dlit);
	}
	else {
	  // side input が 1 なら dlit も 0
	  Literal iglit(inode->gvar(), false);
	  solver.add_clause(~iglit, ~dlit);
	}
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // なにもしない．
      break;
    }
  }

  // 全ゲートタイプ共通
  // 全てのファンインに故障差が伝搬していなければ
  // このゲートの出力にも故障差は伝搬しない．
  tmp_lits_begin(ni + 1);
  tmp_lits_add(~dlit);
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->has_fvar() ) {
      Literal idlit(inode->dvar(), false);
      tmp_lits_add(idlit);
    }
  }
  tmp_lits_end(solver);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
SatEngineBase::make_flt0_cnf(SatSolver& solver,
			     VarId ivar,
			     VarId fvar,
			     VarId ovar)
{
  Literal ilit(ivar, false);
  Literal flit(fvar, false);
  Literal olit(ovar, false);

  solver.add_clause( ilit,        ~olit);
  solver.add_clause(       ~flit, ~olit);
  solver.add_clause(~ilit,  flit,  olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar 故障変数
// @param[in] ovar 出力の変数
void
SatEngineBase::make_flt1_cnf(SatSolver& solver,
			     VarId ivar,
			     VarId fvar,
			     VarId ovar)
{
  Literal ilit(ivar, false);
  Literal flit(fvar, false);
  Literal olit(ovar, false);

  solver.add_clause(~ilit,         olit);
  solver.add_clause(       ~flit,  olit);
  solver.add_clause( ilit,  flit, ~olit);
}

// @brief 故障挿入回路を表す CNF 式を作る．
// @param[in] solver SAT ソルバー
// @param[in] ivar 入力の変数
// @param[in] fvar0 故障変数
// @param[in] fvar1 故障変数
// @param[in] ovar 出力の変数
void
SatEngineBase::make_flt01_cnf(SatSolver& solver,
			      VarId ivar,
			      VarId fvar0,
			      VarId fvar1,
			      VarId ovar)
{
  Literal ilit(ivar, false);
  Literal f0lit(fvar0, false);
  Literal f1lit(fvar1, false);
  Literal olit(ovar, false);

  solver.add_clause(       ~f0lit,         ~olit);
  solver.add_clause(               ~f1lit,  olit);
  solver.add_clause( ilit,  f0lit,  f1lit, ~olit);
  solver.add_clause(~ilit,  f0lit,  f1lit,  olit);
}

// @brief 一つの SAT問題を解く．
Bool3
SatEngineBase::solve(SatSolver& solver,
		     TpgFault* f)
{
  SatStats prev_stats;
  solver.get_stats(prev_stats);

  timer_start();

  Bool3 ans = solver.solve(mTmpLits, mModel);

  USTime time = timer_stop();

  SatStats sat_stats;
  solver.get_stats(sat_stats);

  sub_sat_stats(sat_stats, prev_stats);

  if ( ans == kB3True ) {
    // パタンが求まった．
    detect_op(f, sat_stats, time);
  }
  else if ( ans == kB3False ) {
    // 検出不能と判定された．
    untest_op(f, sat_stats, time);
  }
  else { // ans == kB3X つまりアボート
    abort_op(f, sat_stats, time);
  }

  return ans;
}

// @brief 一つの SAT問題を解く．
Bool3
SatEngineBase::_solve(SatSolver& solver)
{
  Bool3 ans = solver.solve(mTmpLits, mModel);

  return ans;
}

// @brief 検出した場合の処理
void
SatEngineBase::detect_op(TpgFault* fault,
			 const SatStats& sat_stats,
			 const USTime& time)
{
  // バックトレースを行う．
  TestVector* tv = mBackTracer(fault->node(), mModel, mInputList, mOutputList);

  // パタンの登録などを行う．
  mDetectOp(fault, tv);

  ++ mStats.mDetCount;
  mStats.mDetTime += time;
  add_sat_stats(mStats.mDetStats, sat_stats);
  max_sat_stats(mStats.mDetStatsMax, sat_stats);
}

// @brief 検出不能と判定した時の処理
void
SatEngineBase::untest_op(TpgFault* fault,
			 const SatStats& sat_stats,
			 const USTime& time)
{
  mUntestOp(fault);

  ++ mStats.mRedCount;
  mStats.mRedTime += time;
  add_sat_stats(mStats.mRedStats, sat_stats);
  max_sat_stats(mStats.mRedStatsMax, sat_stats);
}

// @brief 部分的な検出不能と判定した時の処理
void
SatEngineBase::partially_untest_op(TpgFault* fault,
				   const SatStats& sat_stats,
				   const USTime& time)
{
  ++ mStats.mPartRedCount;
  mStats.mPartRedTime += time;
  add_sat_stats(mStats.mPartRedStats, sat_stats);
  max_sat_stats(mStats.mPartRedStatsMax, sat_stats);
}

// @brief アボートした時の処理
void
SatEngineBase::abort_op(TpgFault* fault,
			const SatStats& sat_stats,
			const USTime& time)
{
  ++ mStats.mAbortCount;
  mStats.mAbortTime += time;
}

// SatStats をクリアする．
void
SatEngineBase::clear_sat_stats(SatStats& stats)
{
  stats.mRestart = 0;
  stats.mVarNum = 0;
  stats.mConstrClauseNum = 0;
  stats.mConstrLitNum = 0;
  stats.mLearntClauseNum = 0;
  stats.mLearntLitNum = 0;
  stats.mConflictNum = 0;
  stats.mDecisionNum = 0;
  stats.mPropagationNum = 0;
}

// SatStats をたす．
void
SatEngineBase::add_sat_stats(SatStats& dst_stats,
			     const SatStats& src_stats)
{
  dst_stats.mRestart += src_stats.mRestart;
  dst_stats.mVarNum += src_stats.mVarNum;
  dst_stats.mConstrClauseNum += src_stats.mConstrClauseNum;
  dst_stats.mConstrLitNum += src_stats.mConstrLitNum;
  dst_stats.mLearntClauseNum += src_stats.mLearntClauseNum;
  dst_stats.mLearntLitNum += src_stats.mLearntLitNum;
  dst_stats.mConflictNum += src_stats.mConflictNum;
  dst_stats.mDecisionNum += src_stats.mDecisionNum;
  dst_stats.mPropagationNum += src_stats.mPropagationNum;
}

// SatStats を引く
void
SatEngineBase::sub_sat_stats(SatStats& dst_stats,
			     const SatStats& src_stats)
{
  dst_stats.mRestart -= src_stats.mRestart;
  dst_stats.mVarNum -= src_stats.mVarNum;
  dst_stats.mConstrClauseNum -= src_stats.mConstrClauseNum;
  dst_stats.mConstrLitNum -= src_stats.mConstrLitNum;
  dst_stats.mLearntClauseNum -= src_stats.mLearntClauseNum;
  dst_stats.mLearntLitNum -= src_stats.mLearntLitNum;
  dst_stats.mConflictNum -= src_stats.mConflictNum;
  dst_stats.mDecisionNum -= src_stats.mDecisionNum;
  dst_stats.mPropagationNum -= src_stats.mPropagationNum;
}

// SatStats の各々の最大値をとる．
void
SatEngineBase::max_sat_stats(SatStats& dst_stats,
			     const SatStats& src_stats)
{
  if ( dst_stats.mRestart < src_stats.mRestart ) {
    dst_stats.mRestart = src_stats.mRestart;
  }
  if ( dst_stats.mVarNum < src_stats.mVarNum ) {
    dst_stats.mVarNum += src_stats.mVarNum;
  }
  if ( dst_stats.mConstrClauseNum < src_stats.mConstrClauseNum ) {
    dst_stats.mConstrClauseNum += src_stats.mConstrClauseNum;
  }
  if ( dst_stats.mConstrLitNum < src_stats.mConstrLitNum ) {
    dst_stats.mConstrLitNum += src_stats.mConstrLitNum;
  }
  if ( dst_stats.mLearntClauseNum < src_stats.mLearntClauseNum ) {
    dst_stats.mLearntClauseNum += src_stats.mLearntClauseNum;
  }
  if ( dst_stats.mLearntLitNum < src_stats.mLearntLitNum ) {
    dst_stats.mLearntLitNum += src_stats.mLearntLitNum;
  }
  if ( dst_stats.mConflictNum < src_stats.mConflictNum ) {
    dst_stats.mConflictNum += src_stats.mConflictNum;
  }
  if ( dst_stats.mDecisionNum < src_stats.mDecisionNum ) {
    dst_stats.mDecisionNum += src_stats.mDecisionNum;
  }
  if ( dst_stats.mPropagationNum < src_stats.mPropagationNum ) {
    dst_stats.mPropagationNum += src_stats.mPropagationNum;
  }
}

// @brief ノードの変数割り当てフラグを消す．
void
SatEngineBase::clear_node_mark()
{
  for (vector<TpgNode*>::iterator p = mTfoList.begin();
       p != mTfoList.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_var();
  }
}

END_NAMESPACE_YM_SATPG

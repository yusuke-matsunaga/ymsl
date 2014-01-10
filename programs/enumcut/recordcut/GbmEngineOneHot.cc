
/// @file GbmEngineOneHot.cc
/// @brief GbmEngineOneHot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngineOneHot.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmEngineOneHot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
// @param[in] node_num ノード数
// @param[in] conf_num 設定変数の数
// @param[in] input_num 入力数
// @param[in] rep 関数の対称変数の代表番号を収める配列
//            rep[pos] に pos 番めの入力の代表番号が入る．
GbmEngineOneHot::GbmEngineOneHot(SatSolver& solver,
				 ymuint node_num,
				 ymuint conf_num,
				 ymuint input_num,
				 const vector<ymuint>& rep) :
  GbmEngine(solver, node_num, conf_num),
  mInputNum(input_num),
  mRep(rep),
  mIorderVarArray(input_num * input_num)
{
}

// @brief デストラクタ
GbmEngineOneHot::~GbmEngineOneHot()
{
}

// @brief 変数を初期化する．
void
GbmEngineOneHot::init_vars()
{
  init_conf_vars();

  // 入力順用の変数を作る．
  for (ymuint i = 0; i < mInputNum; ++ i) {
    ymuint base = i * mInputNum;
    for (ymuint j = 0; j < mInputNum; ++ j) {
      VarId vid = new_var();
      mIorderVarArray[base + j] = vid;
      if ( debug() ) {
	cout << "Iorder[" << i << "][" << j << "] = " << vid << endl;
      }
    }
  }
}

// @brief 入力値を割り当てて CNF 式を作る．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] oid 出力のノード番号
// @param[in] oval 出力の値
// @note 結果のCNF式は SAT ソルバに追加される．
bool
GbmEngineOneHot::make_cnf(const RcfNetwork& network,
			  ymuint bit_pat,
			  ymuint oid,
			  bool oval)
{
  ymuint ni = network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();
    VarId vid = new_var();
    set_node_var(id, GbmLit(vid));
    if ( debug() ) {
      cout << " lut_input#" << i << ": " << vid << endl;
    }

    // 入力と外部入力の間の関係式を作る．
    for (ymuint j = 0; j < ni; ++ j) {
      Literal lit0(mIorderVarArray[i * ni + j], kPolNega);
      tPol pol = ( bit_pat & (1U << j) ) ? kPolPosi : kPolNega;
      Literal lit1(vid, pol);
      add_clause(lit0, lit1);
    }
    // 2つの変数が同時に true になってはいけないというルール
    for (ymuint j = 0; j < ni; ++ j) {
      Literal lit0(mIorderVarArray[i * ni + j], kPolNega);
      for (ymuint k = j + 1; k < ni; ++ k) {
	Literal lit1(mIorderVarArray[i * ni + k], kPolNega);
	add_clause(lit0, lit1);
      }
    }
    // 最低1つの変数が true にならなければならないというルール
    vector<Literal> tmp_lits(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      tmp_lits[j] = Literal(mIorderVarArray[i * ni + j], kPolPosi);
    }
    add_clause(tmp_lits);

    // 異なる LUT 入力におなじ入力が接続してはいけないというルール
    for (ymuint j = 0; j < ni; ++ j) {
      Literal lit0(mIorderVarArray[i * ni + j], kPolNega);
      for (ymuint k = 0; k < i; ++ k) {
	Literal lit1(mIorderVarArray[k * ni + j], kPolNega);
	add_clause(lit0, lit1);
      }
    }

    // LUT の対称性を考慮したルール
    ymuint pred;
    if ( network.get_pred(i, pred) ) {
      for (ymuint j = 0; j < ni; ++ j) {
	Literal lit0(mIorderVarArray[i * ni + j], kPolNega);
	for (ymuint k = j + 1; k < ni; ++ k) {
	  Literal lit1(mIorderVarArray[pred * ni + k], kPolNega);
	  add_clause(lit0, lit1);
	}
      }
    }

    // 関数の対称性を考慮したルール
    for (ymuint j = 0; j < ni; ++ j) {
      ymuint cur_rep = mRep[j];
      if ( cur_rep != j ) {
	vector<Literal> tmp_lits(i + 1);
	Literal lit0(mIorderVarArray[i * ni + j], kPolNega);
	for (ymuint k = 0; k < i; ++ k) {
	  Literal lit1(mIorderVarArray[k * ni + cur_rep], kPolPosi);
	  tmp_lits[k] = lit1;
	}
	tmp_lits[i] = lit0;
	add_clause(tmp_lits);
      }
    }
  }

  return make_nodes_cnf(network, oid, oval);
}

// @brief SAT モデルから入力順を取り出す．
// @param[in] model SAT モデル
// @param[out] iorder 入力順
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
void
GbmEngineOneHot::get_iorder(const vector<Bool3>& model,
			    vector<ymuint>& iorder) const
{
  ymuint ni = iorder.size();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint base = i * ni;
    for (ymuint j = 0; j < ni; ++ j) {
      VarId vid = mIorderVarArray[base + j];
      if ( model[vid.val()] == kB3True ) {
	iorder[i] = j;
	break;
      }
    }
  }
}

END_NAMESPACE_YM

#ifndef FSIM3_H
#define FSIM3_H

/// @file src/fsim3/Fsim3.h
/// @brief Fsim3 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "fsim3_nsdef.h"
#include "Fsim.h"
#include "EventQ.h"
#include "FsimFault.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM3

class SimFFR;
class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class Fsim3 Fsim3.h "Fsim3.h"
/// @brief 故障シミュレーションを行うモジュール
//////////////////////////////////////////////////////////////////////
class Fsim3 :
  public Fsim
{
public:

  /// @brief コンストラクタ
  Fsim3();

  /// @brief デストラクタ
  virtual
  ~Fsim3();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  virtual
  void
  set_network(const TpgNetwork& network);

  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[out] det_faults 検出された故障を格納するリスト
  virtual
  void
  run(TestVector* tv,
      vector<const TpgFault*>& det_faults);

  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[out] det_faults 検出された故障を格納するリストの配列
  virtual
  void
  run(const vector<TestVector*>& tv_array,
      vector<vector<const TpgFault*> >& det_faults);

  /// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  virtual
  bool
  run(TestVector* tv,
      const TpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部の故障リストの更新を行なう．
  void
  update_faults();

  /// @brief FFR 内の故障シミュレーションを行う．
  bool
  ffr_simulate(SimFFR* ffr,
	       vector<FsimFault*>& det_flist);

  /// @brief 正常値の計算を行う．
  /// @note 値の変わったノードは mGvalClearArray に積まれる．
  void
  calc_gval();

  /// @brief 正常値をクリアする．
  /// @note mGvalClearArray を使う．
  void
  clear_gval();

  /// @brief 故障値の計算を行う．
  /// @note この関数を抜けた時点で故障値はクリアされている．
  bool
  calc_fval(SimNode* root);


private:
  //////////////////////////////////////////////////////////////////////
  // TpgNode と SimNode の対応関係を表すためのデータ構造
  //////////////////////////////////////////////////////////////////////

  struct EdgeMap {
    SimNode* mNode;
    ymuint32 mPos;
  };


private:

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  /// 内部で clear_faults() を呼ぶ．
  void
  clear();

  /// @brief FsimFault を破棄する．
  void
  clear_faults();

  /// @brief node に対応する SimNode を得る．
  SimNode*
  find_simnode(const TpgNode* node) const;

  /// @brief node の pos 番めの入力に対応する枝を得る．
  void
  find_simedge(const TpgNode* node,
	       ymuint pos,
	       SimNode*& simnode,
	       ymuint& ipos) const;

  /// @brief 外部入力ノードを作る．
  SimNode*
  make_input();

  /// @brief プリミティブに対応したノードを作る．
  /// @param[in] prim プリミティブ
  /// @param[in] inputs もとのノードの入力の SimNode
  /// @param[in] emap もとのノードの枝の対応関係を記録する配列
  /// @note inputs のサイズはノードの入力数 x 2
  SimNode*
  make_primitive(const TpgPrimitive* prim,
		 const vector<SimNode*>& inputs,
		 const vector<EdgeMap*>& emap);

  /// @brief logic ノードを作る．
  SimNode*
  make_node(tTgGateType type,
	    const vector<SimNode*>& inputs);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const TpgNetwork* mNetwork;

  // TpgNode の id をキーにして SimNode を入れる配列
  vector<SimNode*> mSimMap;

  // TpgNode の id と入力番号から枝の情報を取り出す配列
  vector<vector<EdgeMap> > mEdgeMap;

  // 全ての SimNode を納めた配列
  vector<SimNode*> mNodeArray;

  // 外部入力に対応する SimNode を納めた配列
  vector<SimNode*> mInputArray;

  // 外部出力に対応する SimNode を納めた配列
  vector<SimNode*> mOutputArray;

  // 入力からのトポロジカル順に並べた logic ノードの配列
  vector<SimNode*> mLogicArray;

  // FFR を納めた配列
  vector<SimFFR> mFFRArray;

  // イベントキュー
  EventQ mEventQ;

  // 正常値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mGvalClearArray;

  // 故障値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mFvalClearArray;

  // 故障シミュレーション用の故障の配列
  vector<FsimFault> mFsimFaults;

};

END_NAMESPACE_YM_SATPG_FSIM3

#endif // FSIM3_H

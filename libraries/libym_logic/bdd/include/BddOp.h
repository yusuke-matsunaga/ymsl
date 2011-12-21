#ifndef BDDOP_H
#define BDDOP_H

/// @file BddOp.h
/// @brief BddOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddBinOp BddOp.h "BddOp.h"
/// @brief BDD 間の二項演算を表すクラス
//////////////////////////////////////////////////////////////////////
class BddBinOp
{
public:

  /// @brief コンストラクタ
  BddBinOp() { }

  /// @brief デストラクタ
  virtual
  ~BddBinOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left, right オペランド
  /// @return 演算結果を買えす．
  virtual
  BddEdge
  apply(BddEdge left,
	BddEdge right) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // f と g のノードの子供のノードとレベルを求める．
  static
  ymuint
  split(BddEdge f,
	BddEdge g,
	BddEdge& f_0,
	BddEdge& f_1,
	BddEdge& g_0,
	BddEdge& g_1);

  // idx が top に等しいときには e の子供を e_0, e_1 にセットする．
  // 等しくなければ e をセットする．
  static
  void
  split1(ymuint top,
	 ymuint level,
	 BddEdge e,
	 const BddNode* vp,
	 tPol pol,
	 BddEdge& e_0,
	 BddEdge& e_1);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// f と g のノードの子供のノードとレベルを求める．
inline
ymuint
BddBinOp::split(BddEdge f,
		BddEdge g,
		BddEdge& f_0,
		BddEdge& f_1,
		BddEdge& g_0,
		BddEdge& g_1)
{
  BddNode* f_vp = f.get_node();
  BddNode* g_vp = g.get_node();
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( g_level < level ) {
    level = g_level;
  }
  split1(level, f_level, f, f_vp, f_pol, f_0, f_1);
  split1(level, g_level, g, g_vp, g_pol, g_0, g_1);
  return level;
}

// idx が top に等しいときには e の子供を e_0, e_1 にセットする．
// 等しくなければ e をセットする．
inline
void
BddBinOp::split1(ymuint top,
		 ymuint level,
		 BddEdge e,
		 const BddNode* vp,
		 tPol pol,
		 BddEdge& e_0,
		 BddEdge& e_1)
{
  if ( level == top ) {
    e_0 = vp->edge0(pol);
    e_1 = vp->edge1(pol);
  }
  else {
    e_0 = e_1 = e;
  }
}

END_NAMESPACE_YM_BDD

#endif // BDDOP_H

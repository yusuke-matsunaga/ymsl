#ifndef BOTTOMUP_H
#define BOTTOMUP_H

/// @file ButtomUp.h
/// @brief ButtomUp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCut.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class BottomUp BottomUp.h "BottomUp.h"
/// @brief ボトムアップのカット列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class BottomUp :
  public EnumCut
{
public:

  /// @brief コンストラクタ
  BottomUp();

  /// @brief デストラクタ
  virtual
  ~BottomUp();


public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  virtual
  void
  operator()(const BdnMgr& network,
	     ymuint limit,
	     EnumCutOp* op);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  mark_cut1(const BdnNode* node);

  void
  clear_cut1(const BdnNode* node);

  void
  mark_cut2(const BdnNode* node);

  void
  clear_cut2(const BdnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct NodeInfo
  {
    // カットのリスト
    list<vector<const BdnNode*> > mCutList;

    // マーク1
    ymuint8 mMark1;

    // マーク2
    ymuint8 mMark2;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの情報
  vector<NodeInfo> mNodeInfo;

  // カットの入力を保持する作業領域
  const BdnNode** mTmpInputs;

  // mTmpInputs の要素数
  ymuint32 mInputNum;

};

END_NAMESPACE_YM_NETWORKS

#endif // BOTTOMUP_H

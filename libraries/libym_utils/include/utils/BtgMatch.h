#ifndef UTILS_BTGMATCH_H
#define UTILS_BTGMATCH_H

/// @file utils/BtgMatch.h
/// @brief BtgMatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class BtgNode;
class BtgEdge;

//////////////////////////////////////////////////////////////////////
/// @class BtgMatch BtgMatch.h "ym_utils/BtgMatch.h"
/// @brief 2部グラフの最大マッチングを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class BtgMatch
{
public:

  /// @brief 空のコンストラクタ
  BtgMatch();

  /// @brief コンストラクタ
  /// @param[in] node1_num 節点グループ1の要素数
  /// @param[in] node2_num 節点グループ2の要素数
  BtgMatch(ymuint node1_num,
	   ymuint node2_num);

  /// @brief デストラクタ
  ~BtgMatch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 節点数を設定する．
  /// @param[in] node1_num 節点グループ1の要素数
  /// @param[in] node2_num 節点グループ2の要素数
  /// @note 以前の内容はクリアされる．
  void
  resize(ymuint node1_num,
	 ymuint node2_num);

  /// @brief 節点グループ1の要素数を返す．
  ymuint
  node1_num() const;

  /// @brief 節点グループ2の要素数を返す．
  ymuint
  node2_num() const;

  /// @brief 枝をすべて取り除く
  void
  clear_edge();

  /// @brief 枝を追加する．
  /// @param[in] node1_id 節点グループ1のノード番号 ( 0 <= node1_id < node1_num() )
  /// @param[in] node2_id 節点グループ2のノード番号 ( 0 <= node2_id < node2_num() )
  /// @param[in] weight 枝の重み (省略されたらデフォルトは1)
  /// @note 既に同じ枝があったら上書きされる．
  void
  add_edge(ymuint node1_id,
	   ymuint node2_id,
	   ymuint weight = 1);

  /// @brief 枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝の情報を返す．
  /// @param[in] edge_id 枝のID番号 ( 0 <= edge_id < edge_num() )
  /// @param[out] node1_id 節点グループ1のノード番号
  /// @param[out] node2_id 節点グループ2のノード番号
  /// @param[out] weight 枝の重み
  void
  edge_info(ymuint edge_id,
	    ymuint& node1_id,
	    ymuint& node2_id,
	    ymuint& weight) const;

  /// @brief 最大マッチングを求める．
  /// @param[in] edge_list マッチング結果の枝を格納するリスト
  /// @return マッチング結果の重みの総和を返す．
  ymuint
  calc_match(vector<ymuint>& edge_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 節点グループ1でまわす calc_match()
  void
  calc_match1();

  /// @brief 節点グループ2でまわす calc_match()
  void
  calc_match2();

  /// @brief 節点グループ1でまわす calc_wmatch()
  void
  calc_wmatch1();

  /// @brief 節点グループ2でまわす calc_wmatch()
  void
  calc_wmatch2();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 節点グループ1の要素数
  ymuint32 mNode1Num;

  // 節点グループ1のノードの配列
  BtgNode* mNode1Array;

  // 節点グループ2の要素数
  ymuint32 mNode2Num;

  // 節点グループ2のノードの配列
  BtgNode* mNode2Array;

  // 枝のリスト
  vector<BtgEdge*> mEdgeList;

};

END_NAMESPACE_YM

#endif // DSA_BTGMATCH_H

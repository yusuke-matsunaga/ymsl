#ifndef YM_NETWORKS_CMN_CMNLATCH_H
#define YM_NETWORKS_CMN_CMNLATCH_H

/// @file ym_networks/CmnLatch.h
/// @brief CmnLatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnLatch CmnLatch.h "ym_networks/CmnLatch.h"
/// @brief ラッチを表すクラス
//////////////////////////////////////////////////////////////////////
class CmnLatch :
  public DlElem
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] cell セル
  CmnLatch(const string& name,
	   const CmnLatchCell* cell);

  /// @brief デストラクタ
  ~CmnLatch();


public:

  /// @brief ID番号の取得
  /// @return ID番号を返す．
  /// @note ID番号はラッチの生成時に CmnMgr により自動的に割り振られる．
  /// @sa CmnMgr
  ymuint
  id() const;

  /// @brief 名前を返す．
  string
  name() const;

  /// @brief 肯定のデータ出力ノードを返す．
  const CmnNode*
  output1() const;

  /// @brief 否定のデータ出力ノードを返す．
  const CmnNode*
  output2() const;

  /// @brief データ入力ノードを返す．
  const CmnNode*
  input() const;

  /// @brief ラッチイネーブルノードを返す．
  const CmnNode*
  enable() const;

  /// @brief クリア信号のノードを返す．
  /// @note NULL の場合もある．
  const CmnNode*
  clear() const;

  /// @brief プリセット信号のノードを返す．
  /// @note NULL の場合もある．
  const CmnNode*
  preset() const;

  /// @brief 対応するセルを返す．
  const CmnLatchCell*
  cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 名前
  string mName;

  // 肯定のデータ出力ノード
  CmnNode* mOutput1;

  // 否定のデータ出力ノード
  CmnNode* mOutput2;

  // データ入力ノード
  CmnNode* mInput;

  // ラッチイネーブルノード
  CmnNode* mEnable;

  // クリア信号ノード
  CmnNode* mClear;

  // プリセット信号ノード
  CmnNode* mPreset;

  // セル
  const CmnLatchCell* mCell;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号の取得
// @return ID番号を返す．
// @note ID番号はラッチの生成時に CmnMgr により自動的に割り振られる．
// @sa CmnMgr
inline
ymuint
CmnLatch::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
string
CmnLatch::name() const
{
  return mName;
}

// @brief 肯定のデータ出力ノードを返す．
inline
const CmnNode*
CmnLatch::output1() const
{
  return mOutput1;
}

// @brief 否定のデータ出力ノードを返す．
inline
const CmnNode*
CmnLatch::output2() const
{
  return mOutput2;
}

// @brief データ入力ノードを返す．
inline
const CmnNode*
CmnLatch::input() const
{
  return mInput;
}

// @brief ラッチイネーブルノードを返す．
inline
const CmnNode*
CmnLatch::enable() const
{
  return mEnable;
}

// @brief クリア信号のノードを返す．
// @note NULL の場合もある．
inline
const CmnNode*
CmnLatch::clear() const
{
  return mClear;
}

// @brief プリセット信号のノードを返す．
// @note NULL の場合もある．
inline
const CmnNode*
CmnLatch::preset() const
{
  return mPreset;
}

// @brief 対応するセルを返す．
inline
const CmnLatchCell*
CmnLatch::cell() const
{
  return mCell;
}

END_NAMESPACE_YM_CMN

#endif // YM_NETWORKS_CMN_CMNLATCH_H

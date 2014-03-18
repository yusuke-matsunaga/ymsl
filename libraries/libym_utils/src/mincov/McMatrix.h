#ifndef MCMATRIX_H
#define MCMATRIX_H

/// @file McMatrix.h
/// @brief McMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov_nsdef.h"
#include "McRowHead.h"
#include "McColHead.h"
#include "utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McMatrix McMatrix.h "McMatrix.h"
/// @brief mincov 用の行列を表すクラス
//////////////////////////////////////////////////////////////////////
class McMatrix
{
public:

  /// @brief コンストラクタ
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @note 要素を持たない行列となる．
  McMatrix(ymuint32 row_size,
	   ymuint32 col_size);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  McMatrix(const McMatrix& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const McMatrix&
  operator=(const McMatrix& src);

  /// @brief デストラクタ
  ~McMatrix();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  ymuint32
  row_size() const;

  /// @brief 列数を返す．
  ymuint32
  col_size() const;

  /// @brief 行を取り出す．
  /// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
  const McRowHead*
  row(ymuint32 row_pos) const;

  /// @brief 行の先頭を取り出す．
  const McRowHead*
  row_front() const;

  /// @brief 行の末尾を取り出す．
  const McRowHead*
  row_back() const;

  /// @brief row が終端かどうか調べる．
  /// @param[in] row 対象の行
  /// @return row が終端の時 true を返す．
  bool
  is_row_end(const McRowHead* row) const;

  /// @brief 実効的な行数を返す．
  ymuint32
  remain_row_size() const;

  /// @brief 列を取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  const McColHead*
  col(ymuint32 col_pos) const;

  /// @brief 列の先頭を取り出す．
  const McColHead*
  col_front() const;

  /// @brief 列の末尾を取り出す．
  const McColHead*
  col_back() const;

  /// @brief col が終端かどうか調べる．
  /// @param[in] col 対象の行
  /// @return col が終端の時 true を返す．
  bool
  is_col_end(const McColHead* col) const;

  /// @brief 実効的な列数を返す．
  ymuint32
  remain_col_size() const;

  /// @brief 列のコストを取り出す．
  /// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
  ymuint32
  col_cost(ymuint32 col_pos) const;

  /// @brief 列集合のコストを返す．
  /// @param[in] col_list 列のリスト
  ymuint32
  cost(const vector<ymuint32>& col_list) const;

  /// @brief 列集合がカバーになっているか検証する．
  /// @param[in] col_list 列のリスト
  /// @retval true col_list がカバーになっている．
  /// @retval false col_list でカバーされていない行がある．
  bool
  verify(const vector<ymuint32>& col_list) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  /// @note 行/列のサイズは不変
  void
  clear();

  /// @brief サイズを変更する．
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  /// @note 内容はクリアされる．
  void
  resize(ymuint32 row_size,
	 ymuint32 col_size);

  /// @brief 要素を追加する．
  /// @param[in] row_pos 追加する要素の行番号
  /// @param[in] col_pos 追加する要素の列番号
  /// @return 追加された要素を返す．
  McCell*
  insert_elem(ymuint32 row_pos,
	      ymuint32 col_pos);

  /// @brief 列のコストを設定する．
  /// @param[in] col_pos 列番号
  /// @param[in] value 設定する値
  void
  set_col_cost(ymuint32 col_pos,
	       ymuint32 value);

  /// @brief 列を選択し，被覆される行を削除する．
  /// @param[in] col_pos 選択した列
  void
  select_col(ymuint32 col_pos);

  /// @brief 行を削除する．
  void
  delete_row(ymuint32 row_pos);

  /// @brief 列を削除する．
  void
  delete_col(ymuint32 col_pos);

  /// @brief 簡単化を行う．
  /// @param[out] selected_cols 簡単化中で選択された列の集合を追加する配列
  void
  reduce(vector<ymuint32>& selected_cols);

  /// @brief 行支配を探し，行を削除する．
  /// @return 削除された行があったら true を返す．
  bool
  row_dominance();

  /// @brief 列支配を探し，列を削除する．
  /// @return 削除された列があったら true を返す．
  bool
  col_dominance();

  /// @brief 必須列を探し，列を選択する．
  /// @param[out] selected_cols 選択された列を追加する列集合
  /// @return 選択された列があったら true を返す．
  bool
  essential_col(vector<ymuint32>& selected_cols);

  /// @brief 削除スタックにマーカーを書き込む．
  void
  save();

  /// @brief 直前のマーカーまで処理を戻す．
  void
  restore();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする．
  void
  copy(const McMatrix& src);

  /// @brief 行を復元する．
  void
  restore_row(ymuint32 row_pos);

  /// @brief 列を復元する．
  void
  restore_col(ymuint32 col_pos);

  /// @brief セルの生成
  McCell*
  alloc_cell();

  /// @brief セルの解放
  void
  free_cell(McCell* cell);

  /// @brief スタックが空の時 true を返す．
  bool
  stack_empty();

  /// @brief スタックに境界マーカーを書き込む．
  void
  push_marker();

  /// @brief スタックに行削除の印を書き込む．
  void
  push_row(ymuint32 row_pos);

  /// @brief スタックに列削除の印を書き込む．
  void
  push_col(ymuint32 col_pos);

  /// @brief スタックに値を積む．
  void
  push(ymuint32 val);

  /// @brief スタックから取り出す．
  ymuint32
  pop();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルを確保するためのアロケータ
  UnitAlloc mCellAlloc;

  // 行数
  ymuint32 mRowSize;

  // 列数
  ymuint32 mColSize;

  // 行の先頭の配列
  McRowHead* mRowArray;

  // 行の先頭をつなぐリンクトリストのダミー
  McRowHead mRowHead;

  // 列の先頭の配列
  McColHead* mColArray;

  // 列の先頭をつなぐリンクトリストのダミー
  McColHead mColHead;

  // 削除の履歴を覚えておくスタック
  ymuint32* mDelStack;

  // mDelStack のポインタ
  ymuint32 mStackTop;

  // マーク用のブール配列
  bool* mMarkArray;

  // 行番号のリスト
  ymuint32* mRowIdList;

  // mRowIdList の要素数
  ymuint32 mRowIdListNum;

  // 列番号のリスト
  ymuint32* mColIdList;

  // mColIdList の要素数
  ymuint32 mColIdListNum;

  // row_dominance で用いるベクタ
  vector<const McRowHead*> mRowVector;

  // col_dominance で用いるベクタ
  vector<const McColHead*> mColVector;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行数を返す．
inline
ymuint32
McMatrix::row_size() const
{
  return mRowSize;
}

// @brief 列数を返す．
inline
ymuint32
McMatrix::col_size() const
{
  return mColSize;
}

// @brief 行の先頭を取り出す．
// @param[in] row_pos 行位置 ( 0 <= row_pos < row_size() )
inline
const McRowHead*
McMatrix::row(ymuint32 row_pos) const
{
  return &mRowArray[row_pos];
}

// @brief 行の先頭を取り出す．
inline
const McRowHead*
McMatrix::row_front() const
{
  return mRowHead.mNext;
}

// @brief 行の末尾を取り出す．
inline
const McRowHead*
McMatrix::row_back() const
{
  return mRowHead.mPrev;
}

// @brief row が終端かどうか調べる．
// @param[in] row 対象の行
// @return row が終端の時 true を返す．
inline
bool
McMatrix::is_row_end(const McRowHead* row) const
{
  return row == &mRowHead;
}

// @brief 列の先頭を取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
const McColHead*
McMatrix::col(ymuint32 col_pos) const
{
  return &mColArray[col_pos];
}

// @brief 列の先頭を取り出す．
inline
const McColHead*
McMatrix::col_front() const
{
  return mColHead.mNext;
}

// @brief 列の末尾を取り出す．
inline
const McColHead*
McMatrix::col_back() const
{
  return mColHead.mPrev;
}

// @brief col が終端かどうか調べる．
// @param[in] col 対象の行
// @return col が終端の時 true を返す．
inline
bool
McMatrix::is_col_end(const McColHead* col) const
{
  return col == &mColHead;
}

// @brief 列のコストを取り出す．
// @param[in] col_pos 列位置 ( 0 <= col_pos < col_size() )
inline
ymuint32
McMatrix::col_cost(ymuint32 col_pos) const
{
  return col(col_pos)->cost();
}

// @brief 列のコストを設定する．
// @param[in] col_pos 列番号
// @param[in] value 設定する値
inline
void
McMatrix::set_col_cost(ymuint32 col_pos,
		       ymuint32 value)
{
  mColArray[col_pos].set_cost(value);
}

// @brief スタックが空の時 true を返す．
inline
bool
McMatrix::stack_empty()
{
  return mStackTop == 0;
}

// @brief スタックに境界マーカーを書き込む．
inline
void
McMatrix::push_marker()
{
  push(0U);
}

// @brief スタックに行削除の印を書き込む．
inline
void
McMatrix::push_row(ymuint32 row_pos)
{
  push((row_pos << 2) | 1U);
}

// @brief スタックに列削除の印を書き込む．
inline
void
McMatrix::push_col(ymuint32 col_pos)
{
  push((col_pos << 2) | 3U);
}

// @brief スタックに値を積む．
inline
void
McMatrix::push(ymuint32 val)
{
  mDelStack[mStackTop] = val;
  ++ mStackTop;
}

// @brief スタックから取り出す．
inline
ymuint32
McMatrix::pop()
{
  -- mStackTop;
  return mDelStack[mStackTop];
}

END_NAMESPACE_YM_MINCOV

#endif // MCMATRIX_H
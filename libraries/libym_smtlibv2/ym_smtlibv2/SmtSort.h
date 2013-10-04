#ifndef YM_SMTLIBV2_SMTSORT_H
#define YM_SMTLIBV2_SMTSORT_H

/// @file ym_smtlibv2/SmtSort.h
/// @brief SmtSort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtId;

//////////////////////////////////////////////////////////////////////
/// @class SmtSort SmtSort.h "ym_smtlibv2/SmtSort.h"
/// @brief 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtSort
{
  friend class SmtSortMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 型名
  explicit
  SmtSort(const SmtId* name);

  /// @brief デストラクタ
  virtual
  ~SmtSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 名前を返す．
  const SmtId*
  name() const;

  /// @brief 複合型の場合の要素数を返す．
  /// @note 単純な型の場合には 0 を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief 複合型の場合の要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
  virtual
  const SmtSort*
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 名前
  const SmtId* mName;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を返す．
inline
ymuint
SmtSort::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
const SmtId*
SmtSort::name() const
{
  return mName;
}

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTSORT_H

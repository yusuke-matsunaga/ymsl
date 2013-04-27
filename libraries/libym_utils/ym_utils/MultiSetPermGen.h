#ifndef YM_UTILS_MULTISETPERMGEN_H
#define YM_UTILS_MULTISETPERMGEN_H

/// @file MultiSetPermGen.h
/// @brief MultiSetPermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

class MspgIterator;

//////////////////////////////////////////////////////////////////////
/// @class MultiSetPermGen MultiSetPermGen.h "MultiSetPermGen.h"
/// @brief 重複を許した集合の組み合わせを作るクラス
//////////////////////////////////////////////////////////////////////
class MultiSetPermGen :
  public MultiSetGenBase
{
public:

  typedef MspgIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetPermGen(const vector<ymuint>& num_array,
		   ymuint k);

  /// @brief デストラクタ
  ~MultiSetPermGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の反復子を返す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MspgIterator MultiSetPermGen.h "MultiSetPermGen.h"
/// @brief MultiSetPermGen の反復子
//////////////////////////////////////////////////////////////////////
class MspgIterator :
  public MsGenIterator
{
  friend class MultiSetPermGen;

public:

  /// @brief 空のコンストラクタ
  MspgIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MspgIterator(const MspgIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MspgIterator&
  operator=(const MspgIterator& src);

  /// @brief デストラクタ
  ~MspgIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  MspgIterator
  operator++();

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] parent 親の MultiSetPermGen オブジェクト
  MspgIterator(const MultiSetPermGen* parent);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
inline
MultiSetPermGen::MultiSetPermGen(const vector<ymuint>& num_array,
				 ymuint k) :
  MultiSetGenBase(num_array, k)
{
}

// @brief デストラクタ
inline
MultiSetPermGen::~MultiSetPermGen()
{
}

// @brief 先頭の反復子を返す．
inline
MultiSetPermGen::iterator
MultiSetPermGen::begin()
{
  return iterator(this);
}

// @brief 空のコンストラクタ
inline
MspgIterator::MspgIterator()
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
MspgIterator::MspgIterator(const MspgIterator& src) :
  MsGenIterator(src)
{
}

// @brief コンストラクタ
// @param[in] parent 親の MultiSetPermGen オブジェクト
inline
MspgIterator::MspgIterator(const MultiSetPermGen* parent) :
  MsGenIterator(parent)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
inline
const MspgIterator&
MspgIterator::operator=(const MspgIterator& src)
{
  copy(src);
  return *this;
}

// @brief デストラクタ
inline
MspgIterator::~MspgIterator()
{
}

// @brief 末尾のチェック
// @return 末尾の時に true を返す．
inline
bool
MspgIterator::is_end() const
{
  if ( parent() == NULL ) {
    return true;
  }
  return elem(0) == group_num();
}

END_NAMESPACE_YM

#endif // YM_UTILS_MULTISETPERMGEN_H

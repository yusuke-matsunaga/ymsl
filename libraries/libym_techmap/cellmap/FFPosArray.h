#ifndef LIBYM_TECHMAP_CELLMAP_FFPOSARRAY_H
#define LIBYM_TECHMAP_CELLMAP_FFPOSARRAY_H

/// @file libym_techmap/cellmap/FFPosArray.h
/// @brief FFPosArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/cellmap_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class FFPosArray FFPosArray.h "FFPosArray.h"
/// @brief FF のピン番号の情報を表すクラス
/// @note 実際には制御ピンの極性情報も合わせて持つ．
//////////////////////////////////////////////////////////////////////
class FFPosArray
{
public:

  /// @brief コンストラクタ
  FFPosArray();

  /// @brief シグネチャを指定するコンストラクタ
  explicit
  FFPosArray(ymuint sig);

  /// @brief デストラクタ
  ~FFPosArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シグネチャを返す．
  ymuint
  signature() const;

  /// @brief データ入力のピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief クロック入力のピン番号を返す．
  ymuint
  clock_pos() const;

  /// @brief クロック入力の極性情報を返す．
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  ymuint
  clock_sense() const;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  ymuint
  clear_pos() const;

  /// @brief クリア入力の極性情報を返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  clear_sense() const;

  /// @brief クリア入力を持つとき true を返す．
  bool
  has_clear() const;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
  ymuint
  preset_pos() const;

  /// @brief プリセット入力の極性情報を返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  preset_sense() const;

  /// @brief プリセット入力を持つとき true を返す．
  bool
  has_preset() const;

  /// @brief 肯定出力のピン番号を返す．
  ymuint
  q_pos() const;

  /// @brief 否定出力のピン番号を返す．
  ymuint
  iq_pos() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シグネチャを設定する．
  /// @param[in] sig 設定するシグネチャ
  void
  set_signature(ymuint sig);

  /// @brief 個々の情報を設定する．
  /// @param[in] data_pos データ入力のピン番号
  /// @param[in] clock_pos クロック入力のピン番号
  /// @param[in] clock_sense クロック入力の極性情報
  /// @param[in] clear_pos クリア入力のピン番号
  /// @param[in] clear_sense クリア入力の極性情報
  /// @param[in] preset_pos プリセット入力のピン番号
  /// @param[in] preset_sense プリセット入力の極性情報
  /// @param[in] q_pos 肯定出力のピン番号
  /// @param[in] iq_pos 否定出力のピン番号
  void
  set(ymuint data_pos,
      ymuint clock_pos,
      ymuint clock_sense,
      ymuint clear_pos,
      ymuint clear_sense,
      ymuint preset_pos,
      ymuint preset_sense,
      ymuint q_pos,
      ymuint iq_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン番号をパックしたもの
  ymuint32 mData;

};

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_FFPOSARRAY_H

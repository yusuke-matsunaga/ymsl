#ifndef FSIM_SNINPUT_H
#define FSIM_SNINPUT_H

/// @file fsim/SnInput.h
/// @brief SnInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnInput.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "SimNode.h"
#include "SnGate.h"


BEGIN_NAMESPACE_YM_ATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SnInput SimNode.h
/// @brief 入力ノード
//////////////////////////////////////////////////////////////////////
class SnInput :
  public SimNode
{
public:

  /// @brief コンストラクタ
  explicit
  SnInput(ymuint32 id);

  /// @brief デストラクタ
  virtual
  ~SnInput();


public:

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(ymuint pos) const;

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  /// @retval true 新しい値に更新された．
  /// @retval false 値は変わらなかった．
  /// @note 結果は mFval にセットされる．
  virtual
  PackedVal
  _calc_fval2();
  
  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnBuff SimNode.h
/// @brief BUFFノード
//////////////////////////////////////////////////////////////////////
class SnBuff :
  public SnGate1
{
public:

  /// @brief コンストラクタ
  SnBuff(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnBuff();


public:

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();
  
  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  calc_gobs2(ymuint ipos);
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNot SimNode.h
/// @brief NOTノード
//////////////////////////////////////////////////////////////////////
class SnNot :
  public SnBuff
{
public:

  /// @brief コンストラクタ
  SnNot(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNot();


public:

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_ATPG_FSIM

#endif // FSIM_SNINPUT_H

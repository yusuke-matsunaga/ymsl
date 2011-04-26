#ifndef LIBYM_CELL_DOTLIB_PTCELL_H
#define LIBYM_CELL_DOTLIB_PTCELL_H

/// @file libym_cell/dotlib/PtCell.h
/// @brief PtCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtCell PtCell.h "PtCell.h"
/// @brief cell グループを表すパース木の要素
//////////////////////////////////////////////////////////////////////
class PtCell :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] value 値(ここではセル名)
  PtCell(const PtValue* value);

  /// @brief デストラクタ
  virtual
  ~PtCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供を追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] node 追加する子供のノード
  virtual
  void
  add_child(const ShString& attr_name,
	    PtNode* node);

  /// @brief ピンを追加する．
  /// @param[in] pin 対象のピン
  void
  add_pin(PtPin* pin);

  /// @brief バスを追加する．
  /// @param[in] bus 対象のバス
  void
  add_bus(PtBus* bus);

  /// @brief バンドルを追加する．
  /// @param[in] bundle 対象のバンドル
  void
  add_bundle(PtBundle* bundel);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  /// @note このクラスでは常に 1
  virtual
  ymuint
  value_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  virtual
  const PtValue*
  value(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // PtCell に独自の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief pin グループの数を返す．
  ymuint
  pin_num() const;

  /// @brief pin グループのパース木を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  const PtPin*
  pin(ymuint pos) const;

  /// @brief bus グループの数を返す．
  ymuint
  bus_num() const;

  /// @brief bus グループのパース木を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
  const PtBus*
  bus(ymuint pos) const;

  /// @brief bundle グループの数を返す．
  ymuint
  bundle_num() const;

  /// @brief bundle グループのパース木を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
  const PtBundle*
  bundle(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値(名前)
  const PtValue* mValue;

  // ピンのリスト(配列)
  vector<const PtPin*> mPinList;

  // バスのリスト(配列)
  vector<const PtBus*> mBusList;

  // バンドルのリスト(配列)
  vector<const PtBundle*> mBundleList;

  // ピン以外の子供のリスト(配列)
  vector<const PtNode*> mChildList;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTCELL_H
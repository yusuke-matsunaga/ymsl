#ifndef LIBYM_CELL_DOTLIB_PTMGR_H
#define LIBYM_CELL_DOTLIB_PTMGR_H

/// @file libym_cell/dotlib/PtMgr.h
/// @brief PtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtMgr PtMgr.h "PtMgr.h"
/// @brief PtNode, PtValue を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class PtMgr
{
public:

  /// @brief コンストラクタ
  PtMgr();

  /// @brief デストラクタ
  ~PtMgr();


public:

  /// @brief 初期化する．
  /// @note 以前，生成されたオブジェクトは(デストラクタ抜きで)破壊される．
  void
  init();

  /// @brief simple attribute を表す PtNode を生成する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  PtNode*
  new_ptsimple(const string& attr_name,
	       const FileRegion& attr_loc,
	       const PtValue* value);

  /// @brief complex attribute を表す PtNode を生成する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値のリスト
  PtNode*
  new_ptcomplex(const string& attr_name,
		const FileRegion& attr_loc,
		const vector<const PtValue*>& value_list);

  /// @brief group statement を表す PtNode を生成する．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値のリスト
  PtNode*
  new_ptgroup(const string& attr_name,
	      const FileRegion& attr_loc,
	      const vector<const PtValue*>& value_list);

  /// @brief 整数値を表す PtValue を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtValue*
  new_value(int value,
	    const FileRegion& loc);

  /// @brief 実数値を表す PtValue を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtValue*
  new_value(double value,
	    const FileRegion& loc);

  /// @brief 文字列シンボルを表す PtValue を生成する．
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  PtValue*
  new_value(const string& value,
	    const FileRegion& loc);

  /// @brief 演算子を表す PtValue を生成する．
  /// @param[in] type 型
  /// @param[in] opr1, opr2 オペランド
  PtValue*
  new_value(tTokenType type,
	    PtValue* opr1,
	    PtValue* opr2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTMGR_H

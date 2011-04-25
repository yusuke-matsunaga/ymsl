
/// @file libym_cell/dotlib/SimpleHandler.cc
/// @brief SimpleHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"
#include "DotlibParser.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "PtNode.h"
#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス SimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
SimpleHandler::SimpleHandler(GroupHandler* parent) :
  DotlibHandler(parent->parser(), parent->ptmgr(), parent)
{
}

// @brief デストラクタ
SimpleHandler::~SimpleHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
SimpleHandler::read_attr(const string& attr_name,
			 const FileRegion& attr_loc)
{
  if ( !expect(COLON) ) {
    return false;
  }

  tTokenType value_type = parser().read_token( symbol_mode() );
  PtValue* value = NULL;
  switch ( value_type ) {
  case INT_NUM:
    value = ptmgr().new_value(parser().cur_int(), parser().cur_loc());
    break;

  case FLOAT_NUM:
    value = ptmgr().new_value(parser().cur_float(), parser().cur_loc());
    break;

  case SYMBOL:
    value = ptmgr().new_value(parser().cur_string(), parser().cur_loc());
    break;

  default:
    put_msg(__FILE__, __LINE__, parser().cur_loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. int/float/string value is expected.");
    return false;
  }

  if ( debug() ) {
    cout << attr_name << " : " << value << endl;
  }

  if ( !read_value(attr_name, attr_loc, value) ) {
    return false;
  }

  PtNode* node = ptmgr().new_ptsimple(attr_name, attr_loc, value);
  parent()->pt_node()->add_child(node);

  return expect_nl();
}

// @brief シンボルモードで read_token() を呼ぶときに true を返す．
// @note デフォルトの実装では false を返す．
bool
SimpleHandler::symbol_mode()
{
  return false;
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
// @note デフォルトの実装ではなにもしないで true を返す．
bool
SimpleHandler::read_value(const string& attr_name,
			  const FileRegion& attr_loc,
			  const PtValue* value)
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SymSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SymSimpleHandler::SymSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
SymSimpleHandler::~SymSimpleHandler()
{
}

// @brief シンボルモードで read_token() を呼ぶときに true を返す．
bool
SymSimpleHandler::symbol_mode()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IntSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IntSimpleHandler::IntSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
IntSimpleHandler::~IntSimpleHandler()
{
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
bool
IntSimpleHandler::read_value(const string& attr_name,
			     const FileRegion& attr_loc,
			     const PtValue* value)
{
  if ( value->type() != INT_NUM ) {
    put_msg(__FILE__, __LINE__, value->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. int value is expected.");
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス FloatSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FloatSimpleHandler::FloatSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
FloatSimpleHandler::~FloatSimpleHandler()
{
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
bool
FloatSimpleHandler::read_value(const string& attr_name,
			       const FileRegion& attr_loc,
			       const PtValue* value)
{
  if ( value->type() != FLOAT_NUM && value->type() != INT_NUM ) {
    put_msg(__FILE__, __LINE__, value->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. float value is expected.");
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス StrSimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StrSimpleHandler::StrSimpleHandler(GroupHandler* parent) :
  SimpleHandler(parent)
{
}

// @brief デストラクタ
StrSimpleHandler::~StrSimpleHandler()
{
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
bool
StrSimpleHandler::read_value(const string& attr_name,
			     const FileRegion& attr_loc,
			     const PtValue* value)
{
  if ( value->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, value->loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "Syntax error. string value is expected.");
  }
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB

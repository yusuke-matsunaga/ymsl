
/// @file libym_cell/dotlib/GroupHandler.cc
/// @brief GroupHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"
#include "DotlibParser.h"
#include "PtMgr.h"
#include "PtNode.h"
#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス GroupHandler
//////////////////////////////////////////////////////////////////////

// @brief 親がない場合のコンストラクタ
// @param[in] parser パーサー
// @param[in] ptmgr パース木を管理するオブジェクト
GroupHandler::GroupHandler(DotlibParser& parser,
			   PtMgr& ptmgr) :
  DotlibHandler(parser, ptmgr)
{
}

// @brief デストラクタ
GroupHandler::~GroupHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
GroupHandler::read_attr(const ShString& attr_name,
			const FileRegion& attr_loc)
{
  vector<const PtValue*> value_list;

  if ( !parse_complex(value_list) ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << "( ";
    const char* comma = "";
    for (vector<const PtValue*>::const_iterator p = value_list.begin();
	 p != value_list.end(); ++ p) {
      cout << comma << *p;
      comma = ", ";
    }
    cout << " ) {" << endl;
  }

  if ( !begin_group(attr_name, attr_loc, value_list) ) {
    return false;
  }

  if ( !expect(LCB) ) {
    return false;
  }

  for ( ; ; ) {
    tTokenType type = parser().read_token();
    if ( type == NL ) {
      continue;
    }
    if ( type == RCB ) {
      break;
    }
    FileRegion loc = parser().cur_loc();
    if ( type != SYMBOL ) {
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      "string value is expected.");
      return false;
    }
    ShString name(parser().cur_string());
    DotlibHandler* handler = find_handler(name);
    if ( handler == NULL ) {
      ostringstream buf;
      buf << name << ": unknown keyword.";
      put_msg(__FILE__, __LINE__, loc,
	      kMsgError,
	      "DOTLIB_PARSER",
	      buf.str());
      return false;
    }
    if ( !handler->read_attr(name, loc) ) {
      return false;
    }
  }

  if ( !expect(NL) ) {
    return false;
  }

  if ( debug() ) {
    cout << "}" << endl;
  }

  return true;
}

// @brief ハンドラの登録を行う．
// @param[in] attr_name 属性名
// @param[in] handler 対応付けるハンドラ
// @note エラーが起きたら false を返す．
bool
GroupHandler::reg_handler(const char* attr_name,
			  DotlibHandler* handler)
{
  return reg_handler(ShString(attr_name), handler);
}

// @brief ハンドラの登録を行う．
// @param[in] attr_name 属性名
// @note エラーが起きたら false を返す．
bool
GroupHandler::reg_handler(const ShString& attr_name,
			  DotlibHandler* handler)
{
  mHandlerMap.insert(make_pair(attr_name, handler));
  return true;
}

// @brief ハンドラを取り出す．
// @param[in] attr_name 属性名
// @note なければ NULL を返す．
DotlibHandler*
GroupHandler::find_handler(const ShString& attr_name)
{
  hash_map<ShString, DotlibHandler*>::const_iterator p
    = mHandlerMap.find(attr_name);
  if ( p == mHandlerMap.end() ) {
    return NULL;
  }
  else {
    return p->second;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス GenGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
GenGroupHandler::GenGroupHandler(GroupHandler* parent) :
  GroupHandler(parent->parser(), parent->ptmgr()),
  mParent(parent)
{
  mPtNode = NULL;
}

// @brief デストラクタ
GenGroupHandler::~GenGroupHandler()
{
}

// @brief 対応する PtNode を返す．
PtNode*
GenGroupHandler::pt_node()
{
  return mPtNode;
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
bool
GenGroupHandler::begin_group(const ShString& attr_name,
			     const FileRegion& attr_loc,
			     const vector<const PtValue*>& value_list)
{
  mPtNode = ptmgr().new_ptgroup(value_list);
  parent()->pt_node()->add_child(attr_name, mPtNode);
  return true;
}

// @brief group statement の最後に呼ばれる関数
bool
GenGroupHandler::end_group()
{
  return true;
}

// @brief 親のハンドラを得る．
GroupHandler*
GenGroupHandler::parent()
{
  return mParent;
}

END_NAMESPACE_YM_CELL_DOTLIB

/// @file libym_dotlib/DotlibNodeImpl.cc
/// @brief DotlibNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

BEGIN_NONAMESPACE

// インデント用の空白文字列を作る．
string
indent_str(ymuint indent)
{
  string ans;
  for (ymuint i = 0; i < indent; ++ i) {
    ans += ' ';
  }
  return ans;
}

// str 中に [a-zA-Z0-9_] 以外の文字が含まれていたら
// " " で囲んで出力する．
void
dump_str(ostream& s,
	 const ShString& str)
{
  bool need_quote = false;
  for (const char* p = str; *p; ++ p) {
    int c = *p;
    if ( !isalnum(c) && c != '_' ) {
      need_quote = true;
      break;
    }
  }
  if ( need_quote ) {
    s << "\"";
  }
  s << str;
  if ( need_quote ) {
    s << "\"";
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス DotlibNodeImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibNodeImpl::DotlibNodeImpl() :
  mNext(NULL)
{
}

// @brief デストラクタ
DotlibNodeImpl::~DotlibNodeImpl()
{
}

// @brief 整数型(kInt)の時に true を返す．
bool
DotlibNodeImpl::is_int() const
{
  return false;
}

// @brief 数値型(kInt か kFloat)の時に true を返す．
bool
DotlibNodeImpl::is_float() const
{
  return false;
}

// @brief 文字列型(kString)の時に true を返す．
bool
DotlibNodeImpl::is_string() const
{
  return false;
}

// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
bool
DotlibNodeImpl::is_opr() const
{
  return false;
}

// @brief リスト型(kList)の時に true を返す．
bool
DotlibNodeImpl::is_list() const
{
  return false;
}

// @brief グループ型(kGroup)の時に true を返す．
bool
DotlibNodeImpl::is_group() const
{
  return false;
}

// @brief 属性型(kAttr)の時に true を返す．
bool
DotlibNodeImpl::is_attr() const
{
  return false;
}

// @brief 整数値を返す．
// @note is_int() = true の時のみ意味を持つ．
int
DotlibNodeImpl::int_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 数値を返す．
// @note is_float() = true の時のみ意味を持つ．
double
DotlibNodeImpl::float_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0.0;
}

// @brief 定数シンボルを返す．
// @note is_string() = true の時のみ意味を持つ．
ShString
DotlibNodeImpl::string_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief 第一オペランドを返す．
// @note is_opr() = true の時のみ意味を持つ．
const DotlibNode*
DotlibNodeImpl::opr1() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 第二オペランドを返す．
// @note is_opr() = true の時のみ意味を持つ．
const DotlibNode*
DotlibNodeImpl::opr2() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief リストの先頭の要素を返す．
// @note is_list() = true の時のみ意味を持つ．
const DotlibNode*
DotlibNodeImpl::top() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief リストの要素数を返す．
// @note is_list() = true の時のみ意味を持つ．
ymuint
DotlibNodeImpl::list_size() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief グループの値を得る．
// @note is_group() = true の時のみ意味を持つ．
const DotlibNode*
DotlibNodeImpl::group_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 先頭の属性を得る．
// @note is_group() = true の時のみ意味を持つ．
const DotlibNode*
DotlibNodeImpl::attr_top() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 属性名を得る．
// @note is_attr() = true の時のみ意味を持つ．
ShString
DotlibNodeImpl::attr_name() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief 属性の値を得る．
// @note is_attr() = true の時のみ意味を持つ．
const DotlibNode*
DotlibNodeImpl::attr_value() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 次の要素を得る．
const DotlibNode*
DotlibNodeImpl::next() const
{
  return mNext;
}

// @brief 要素を追加する．
// @param[in] node 追加する要素
// @note type() が kList の時のみ意味を持つ．
void
DotlibNodeImpl::add_node(DotlibNodeImpl* node)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief attribute を設定する．
// @param[in] attr 属性
// @note type() が kGroup の時のみ意味を持つ．
void
DotlibNodeImpl::add_attr(DotlibNodeImpl* attr)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス DotlibNodeBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
DotlibNodeBase::DotlibNodeBase(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
DotlibNodeBase::~DotlibNodeBase()
{
}

// @brief ファイル上の位置を返す．
FileRegion
DotlibNodeBase::loc() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
/// クラス DotlibInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibInt::DotlibInt(int value,
		     const FileRegion& loc) :
  DotlibNodeBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
DotlibInt::~DotlibInt()
{
}

// @brief 型を得る．
DotlibNode::tType
DotlibInt::type() const
{
  return kInt;
}

// @brief 整数型(kInt)の時に true を返す．
bool
DotlibInt::is_int() const
{
  return true;
}

// @brief 数値型(kInt か kFloat)の時に true を返す．
bool
DotlibInt::is_float() const
{
  return true;
}

// @brief 整数値を返す．
// @note type() が kInt の時のみ意味を持つ．
int
DotlibInt::int_value() const
{
  return mValue;
}

// @brief 数値を返す．
// @note type() が kFloat の時のみ意味を持つ．
double
DotlibInt::float_value() const
{
  return static_cast<double>(mValue);
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
DotlibInt::dump(ostream& s,
		ymuint indent) const
{
  s << int_value();
}


//////////////////////////////////////////////////////////////////////
/// クラス DotlibFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibFloat::DotlibFloat(double value,
			 const FileRegion& loc) :
  DotlibNodeBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
DotlibFloat::~DotlibFloat()
{
}

// @brief 型を得る．
DotlibNode::tType
DotlibFloat::type() const
{
  return kFloat;
}

// @brief 数値型(kInt か kFloat)の時に true を返す．
bool
DotlibFloat::is_float() const
{
  return true;
}

// @brief 数値を返す．
// @note type() が kFloat の時のみ意味を持つ．
double
DotlibFloat::float_value() const
{
  return mValue;
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
DotlibFloat::dump(ostream& s,
		  ymuint indent) const
{
  s << float_value();
}


//////////////////////////////////////////////////////////////////////
/// クラス DotlibString
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibString::DotlibString(ShString value,
			   const FileRegion& loc) :
  DotlibNodeBase(loc),
  mValue(value)
{
}

// @brief デストラクタ
DotlibString::~DotlibString()
{
}

// @brief 型を得る．
DotlibNode::tType
DotlibString::type() const
{
  return kString;
}

// @brief 文字列型(kString)の時に true を返す．
bool
DotlibString::is_string() const
{
  return true;
}

// @brief 定数シンボルを返す．
// @note type() が SYMBOL の時のみ意味を持つ．
ShString
DotlibString::string_value() const
{
  return mValue;
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
DotlibString::dump(ostream& s,
		   ymuint indent) const
{
  dump_str(s, string_value());
}


//////////////////////////////////////////////////////////////////////
// クラス DotlibOpr
//////////////////////////////////////////////////////////////////////

// @brief 表すコンストラクタ
// @param[in] type 演算子の型
// @param[in] opr1, opr2 オペランド
DotlibOpr::DotlibOpr(tType type,
		     const DotlibNode* opr1,
		     const DotlibNode* opr2) :
  mType(type),
  mOpr1(opr1),
  mOpr2(opr2)
{
}

// @brief デストラクタ
DotlibOpr::~DotlibOpr()
{
}

// @brief 型を得る．
DotlibNode::tType
DotlibOpr::type() const
{
  return mType;
}

// @brief 演算子型(kPlus, kMinsu, kMult, kDiv)の時に true を返す．
bool
DotlibOpr::is_opr() const
{
  return true;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
DotlibOpr::loc() const
{
  return FileRegion(mOpr1->loc(), mOpr2->loc());
}

// @brief 第一オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const DotlibNode*
DotlibOpr::opr1() const
{
  return mOpr1;
}

// @brief 第二オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const DotlibNode*
DotlibOpr::opr2() const
{
  return mOpr2;
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
DotlibOpr::dump(ostream& s,
		ymuint indent) const
{
  for (ymuint i = 0; i < indent; ++ i) {
    s << ' ';
  }
  s << "( ";
  opr1()->dump(s, 0);
  switch ( type() ) {
  case kPlus:  s << " + "; break;
  case kMinus: s << " - "; break;
  case kMult:  s << " * "; break;
  case kDiv:   s << " / "; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  opr2()->dump(s, 0);
  s << " )";
}


//////////////////////////////////////////////////////////////////////
// クラス DotlibList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibList::DotlibList() :
  mTop(NULL),
  mTail(NULL)
{
}

// @brief デストラクタ
DotlibList::~DotlibList()
{
}

// @brief 型を得る．
DotlibNode::tType
DotlibList::type() const
{
  return kList;
}

// @brief リスト型(kList)の時に true を返す．
bool
DotlibList::is_list() const
{
  return true;
}

// @brief 式全体のファイル上の位置を返す．
FileRegion
DotlibList::loc() const
{
  if ( mTop == NULL ) {
    return FileRegion();
  }
  return FileRegion(mTop->loc(), mTail->loc());
}

// @brief リストの先頭の要素を返す．
// @note type() が kList の時のみ意味をもつ．
const DotlibNode*
DotlibList::top() const
{
  return mTop;
}

// @brief リストの要素数を返す．
// @note is_list() = true の時のみ意味を持つ．
ymuint
DotlibList::list_size() const
{
  ymuint n = 0;
  for (const DotlibNode* v = top(); v; v = v->next()) {
    ++ n;
  }
  return n;
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
DotlibList::dump(ostream& s,
		 ymuint indent) const
{
  s << "(";
  const char* comma = "";
  for (const DotlibNode* v = top(); v; v = v->next()) {
    s << comma;
    v->dump(s, 0);
    comma = ", ";
  }
  s << ")";
}

// @brief 要素を追加する．
// @param[in] node 追加する要素
// @note type() が kList の時のみ意味を持つ．
void
DotlibList::add_node(DotlibNodeImpl* node)
{
  if ( mTop != NULL ) {
    mTail->mNext = node;
    mTail = node;
  }
  else {
    mTop = mTail = node;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DotlibGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibGroup::DotlibGroup(const DotlibNode* value,
			 const FileRegion& loc) :
  DotlibNodeBase(loc),
  mValue(value),
  mAttrTop(NULL),
  mAttrTail(NULL)
{
}

// @brief デストラクタ
DotlibGroup::~DotlibGroup()
{
}

// @brief 型を得る．
DotlibNode::tType
DotlibGroup::type() const
{
  return kGroup;
}

// @brief グループ型(kGroup)の時に true を返す．
bool
DotlibGroup::is_group() const
{
  return true;
}

// @brief グループの値を得る．
// @note type() が kGroup の時のみ意味を持つ．
const DotlibNode*
DotlibGroup::group_value() const
{
  return mValue;
}

// @brief 先頭の属性を得る．
// @note type() が kGroup の時のみ意味を持つ．
const DotlibNode*
DotlibGroup::attr_top() const
{
  return mAttrTop;
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
DotlibGroup::dump(ostream& s,
		  ymuint indent) const
{
  s << ' ';
  group_value()->dump(s, 0);
  s << " {" << endl;
  for (const DotlibNode* node = attr_top(); node; node = node->next()) {
    node->dump(s, indent + 2);
  }
  s << indent_str(indent) << "}";
}

// @brief attribute を設定する．
// @param[in] attr 属性
// @note type() が kGroup の時のみ意味を持つ．
void
DotlibGroup::add_attr(DotlibNodeImpl* attr)
{
  if ( mAttrTop != NULL ) {
    mAttrTail->mNext = attr;
    mAttrTail = attr;
  }
  else {
    mAttrTop = mAttrTail = attr;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DotlibAttr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] attr_name 属性名
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibAttr::DotlibAttr(const ShString& attr_name,
		       const DotlibNode* value,
		       const FileRegion& loc) :
  DotlibNodeBase(loc),
  mAttrName(attr_name),
  mValue(value)
{
}

// @brief デストラクタ
DotlibAttr::~DotlibAttr()
{
}

// @brief 型を得る．
DotlibNode::tType
DotlibAttr::type() const
{
  return kAttr;
}

// @brief 属性型(kAttr)の時に true を返す．
bool
DotlibAttr::is_attr() const
{
  return true;
}

// @brief 属性名を得る．
ShString
DotlibAttr::attr_name() const
{
  return mAttrName;
}

// @brief 属性の値を得る．
const DotlibNode*
DotlibAttr::attr_value() const
{
  return mValue;
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
DotlibAttr::dump(ostream& s,
		 ymuint indent) const
{
  s << indent_str(indent) << attr_name() << ": ";
  attr_value()->dump(s, indent);
  if ( attr_value()->type() != kGroup ) {
    s << ";";
  }
  s << endl;
}

END_NAMESPACE_YM_DOTLIB
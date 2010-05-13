
/// @file libym_verilog/elb/TagHash.cc
/// @brief TagHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TagHash.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "TagHash.h"
#include <ym_verilog/vl/VlObj.h>
#include "ElbScopeHandle.h"
#include "ElbModule.h"
#include "ElbParamAssign.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"
#include "ElbTaskFunc.h"
#include "ElbContAssign.h"
#include "ElbProcess.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス TagHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリ確保用のオブジェクト
TagHash::TagHash(AllocBase& alloc) :
  mAlloc(alloc),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
TagHash::~TagHash()
{
  delete [] mTable;
}
  
// @brief 内容を空にする
void
TagHash::clear()
{
  for (ymuint32 i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief Cell を登録する．
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[in] cell 対象の Cell
void
TagHash::put_cell(const VlNamedObj* parent,
		  int tag,
		  TagHashCell* cell)
{
  if ( mNum >= mLimit ) {
    // テーブルを拡張する．
    ymuint32 old_size = mSize;
    TagHashCell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint32 i = 0; i < old_size; ++ i) {
      for (TagHashCell* cell = old_table[i]; cell; ) {
	TagHashCell* next = cell->mLink;
	ymuint32 pos = hash_func(cell->mParent, cell->mTag);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint32 pos = hash_func(parent, tag);
  cell->mParent = parent;
  cell->mTag = tag;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}
  
// @brief タグから該当する Cell を探す．
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
TagHashCell*
TagHash::find_cell(const VlNamedObj* parent,
		   int tag) const
{
  ymuint32 pos = hash_func(parent, tag);
  for (TagHashCell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mParent == parent && cell->mTag == tag ) {
      return cell;
    }
  }
  return NULL;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
size_t
TagHash::allocated_size() const
{
  return sizeof(TagHashCell*) * mSize;
}

// @brief テーブルの領域を確保する．
void
TagHash::alloc_table(ymuint32 size)
{
  mSize = size;
  mLimit = static_cast<ymuint32>(mSize * 1.8);
  mTable = new TagHashCell*[mSize];
  for (ymuint32 i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ハッシュ値を計算する．
ymuint32
TagHash::hash_func(const VlNamedObj* parent,
		   int tag) const
{
  return ((reinterpret_cast<ympuint>(parent) * tag) >> 8) % mSize;
}


//////////////////////////////////////////////////////////////////////
// TagHashCell
//////////////////////////////////////////////////////////////////////

// internal scope を追加する．
void
TagHashCell::add_internalscope(ElbScopeHandle* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// internal scope の先頭を得る．
ElbScopeHandle*
TagHashCell::internalscope()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
    
// module array を追加する．
void
TagHashCell::add_modulearray(ElbModuleArray* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// 宣言要素を追加する．
void
TagHashCell::add_decl(ElbDeclBase* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// 宣言要素の先頭を得る．
ElbDeclBase*
TagHashCell::decl()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// parameter 宣言を追加する．
void
TagHashCell::add_parameter(ElbParameter* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// parameter 宣言の先頭を得る．
ElbParameter*
TagHashCell::parameter()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
    
// defparam を追加する．
void
TagHashCell::add_defparam(ElbDefParam* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// defparam の先頭を得る．
ElbDefParam*
TagHashCell::defparam()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// param assign を追加する．
void
TagHashCell::add_paramassign(ElbParamAssign* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}
      
// param assign の先頭を得る．
ElbParamAssign*
TagHashCell::paramassign()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// module array の先頭を得る．
ElbModuleArray*
TagHashCell::modulearray()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
    
// module を追加する．
void
TagHashCell::add_module(ElbModule* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// module の先頭を得る．
ElbModule*
TagHashCell::module()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
    
// primitive array を追加する．
void
TagHashCell::add_primarray(ElbPrimArray* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// primitive array の先頭を得る．
ElbPrimArray*
TagHashCell::primarray()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
    
// primitive を追加する．
void
TagHashCell::add_primitive(ElbPrimitive* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// primitive の先頭を得る．
ElbPrimitive*
TagHashCell::primitive()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}
  
// @brief タスクを追加する．
void
TagHashCell::add_task(ElbTask* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief タスクの先頭を得る．
ElbTask*
TagHashCell::task()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 関数を追加する．
void
TagHashCell::add_function(ElbFunction* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 関数の先頭を得る．
ElbFunction*
TagHashCell::function()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// continuous assignment を追加する．
void
TagHashCell::add_contassign(ElbContAssign* obj)
{
  assert_not_reached(__FILE__, __LINE__);
}

// continuous assignment の先頭を得る．
ElbContAssign*
TagHashCell::contassign()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// process を追加する．
void
TagHashCell::add_process(ElbProcess* process)
{
  assert_not_reached(__FILE__, __LINE__);
}

// process の先頭を得る．
ElbProcess*
TagHashCell::process()
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// internal scope 用のセル
//////////////////////////////////////////////////////////////////////
class CellScope :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellScope(ElbScopeHandle* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_internalscope(ElbScopeHandle* obj);

  /// @brief block scope の先頭を得る．
  virtual
  ElbScopeHandle*
  internalscope();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbScopeHandle* mTop;

  // 末尾の要素
  ElbScopeHandle* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellScope::CellScope(ElbScopeHandle* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellScope::add_internalscope(ElbScopeHandle* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief internal scope の先頭を得る．
ElbScopeHandle*
CellScope::internalscope()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellScope::num()
{
  return mNum;
}

// @brief internal scope を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_internalscope(ElbScopeHandle* obj)
{
  const VlNamedObj* parent = obj->obj()->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiInternalScope);
  if ( cell ) {
    cell->add_internalscope(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellScope));
    TagHashCell* cell = new (p) CellScope(obj);
    put_cell(parent, vpiInternalScope, cell);
  }
}

// @brief internal scope のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_internalscope_list(const VlNamedObj* parent,
				 vector<const VlScope*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiInternalScope);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbScopeHandle* obj = cell->internalscope(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素用のセル
//////////////////////////////////////////////////////////////////////
class CellDecl :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellDecl(ElbDeclBase* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_decl(ElbDeclBase* obj);

  /// @brief 宣言要素の先頭を得る．
  virtual
  ElbDeclBase*
  decl();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbDeclBase* mTop;

  // 末尾の要素
  ElbDeclBase* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellDecl::CellDecl(ElbDeclBase* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellDecl::add_decl(ElbDeclBase* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 宣言要素の先頭を得る．
ElbDeclBase*
CellDecl::decl()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellDecl::num()
{
  return mNum;
}

// @brief 宣言要素を追加する．
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[in] obj 登録する要素
void
TagHash::add_decl(int tag,
		  ElbDeclBase* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, tag);
  if ( cell ) {
    cell->add_decl(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellDecl));
    TagHashCell* cell = new (p) CellDecl(obj);
    put_cell(parent, tag, cell);
  }
}

// @brief タグから該当する宣言要素のリストを返す．
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
// @note scope というスコープ内の tag というタグを持つ要素を
// decl_list に入れる．
bool
TagHash::find_decl_list(const VlNamedObj* parent,
			int tag,
			vector<const VlDecl*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, tag);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbDeclBase* obj = cell->decl(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// parameter 宣言用のセル
//////////////////////////////////////////////////////////////////////
class CellParam :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellParam(ElbParameter* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_parameter(ElbParameter* obj);

  /// @brief parameter 宣言の先頭を得る．
  virtual
  ElbParameter*
  parameter();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbParameter* mTop;

  // 末尾の要素
  ElbParameter* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellParam::CellParam(ElbParameter* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellParam::add_parameter(ElbParameter* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief parameter 宣言の先頭を得る．
ElbParameter*
CellParam::parameter()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellParam::num()
{
  return mNum;
}

// @brief parameter 宣言を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_parameter(ElbParameter* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiParameter);
  if ( cell ) {
    cell->add_parameter(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellParam));
    TagHashCell* cell = new (p) CellParam(obj);
    put_cell(parent, vpiParameter, cell);
  }
}

// @brief タグから該当する宣言要素のリストを返す．
// @param[in] parent 親のスコープ
// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
// @note scope というスコープ内の tag というタグを持つ要素を
// decl_list に入れる．
bool
TagHash::find_param_list(const VlNamedObj* parent,
			 vector<const VlDecl*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiParameter);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbParameter* obj = cell->parameter(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// defparam 用のセル
//////////////////////////////////////////////////////////////////////
class CellDefParam :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellDefParam(ElbDefParam* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_defparam(ElbDefParam* obj);

  /// @brief defparam の先頭を得る．
  virtual
  ElbDefParam*
  defparam();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbDefParam* mTop;

  // 末尾の要素
  ElbDefParam* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellDefParam::CellDefParam(ElbDefParam* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellDefParam::add_defparam(ElbDefParam* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief defparam の先頭を得る．
ElbDefParam*
CellDefParam::defparam()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellDefParam::num()
{
  return mNum;
}

// @brief defparam を追加する．
// @param[in] dobj 登録する要素
void
TagHash::add_defparam(ElbDefParam* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiDefParam);
  if ( cell ) {
    cell->add_defparam(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellDefParam));
    TagHashCell* cell = new (p) CellDefParam(obj);
    put_cell(parent, vpiDefParam, cell);
  }
}

// @brief defparam のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_defparam_list(const VlNamedObj* parent,
			    vector<const VlDefParam*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiDefParam);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbDefParam* obj = cell->defparam(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// param assign 用のセル
//////////////////////////////////////////////////////////////////////
class CellParamAssign :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellParamAssign(ElbParamAssign* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_paramassign(ElbParamAssign* obj);
      
  /// @brief param assign の先頭を得る．
  virtual
  ElbParamAssign*
  paramassign();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbParamAssign* mTop;

  // 末尾の要素
  ElbParamAssign* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellParamAssign::CellParamAssign(ElbParamAssign* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellParamAssign::add_paramassign(ElbParamAssign* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief param assign の先頭を得る．
ElbParamAssign*
CellParamAssign::paramassign()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellParamAssign::num()
{
  return mNum;
}

// @brief param assign を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_paramassign(ElbParamAssign* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiParamAssign);
  if ( cell ) {
    cell->add_paramassign(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellParamAssign));
    TagHashCell* cell = new (p) CellParamAssign(obj);
    put_cell(parent, vpiParamAssign, cell);
  }
}

// @brief param assign のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_paramassign_list(const VlNamedObj* parent,
			       vector<const VlParamAssign*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiParamAssign);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbParamAssign* obj = cell->paramassign(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// module array 用のセル
//////////////////////////////////////////////////////////////////////
class CellModuleArray :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellModuleArray(ElbModuleArray* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_modulearray(ElbModuleArray* obj);

  /// @brief module array の先頭を得る．
  virtual
  ElbModuleArray*
  modulearray();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbModuleArray* mTop;

  // 末尾の要素
  ElbModuleArray* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellModuleArray::CellModuleArray(ElbModuleArray* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellModuleArray::add_modulearray(ElbModuleArray* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief module array の先頭を得る．
ElbModuleArray*
CellModuleArray::modulearray()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellModuleArray::num()
{
  return mNum;
}

// @brief module array を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_modulearray(ElbModuleArray* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiModuleArray);
  if ( cell ) {
    cell->add_modulearray(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellModuleArray));
    TagHashCell* cell = new (p) CellModuleArray(obj);
    put_cell(parent, vpiModuleArray, cell);
  }
}

// @brief module array のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_modulearray_list(const VlNamedObj* parent,
			       vector<const VlModuleArray*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiModuleArray);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbModuleArray* obj = cell->modulearray(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// module 用のセル
//////////////////////////////////////////////////////////////////////
class CellModule :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellModule(ElbModule* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_module(ElbModule* obj);

  /// @brief module の先頭を得る．
  virtual
  ElbModule*
  module();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbModule* mTop;

  // 末尾の要素
  ElbModule* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellModule::CellModule(ElbModule* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellModule::add_module(ElbModule* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief module の先頭を得る．
ElbModule*
CellModule::module()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellModule::num()
{
  return mNum;
}

// @brief module を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_module(ElbModule* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiModule);
  if ( cell ) {
    cell->add_module(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellModule));
    TagHashCell* cell = new (p) CellModule(obj);
    put_cell(parent, vpiModule, cell);
  }
}

// @brief module のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_module_list(const VlNamedObj* parent,
			  vector<const VlModule*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiModule);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbModule* obj = cell->module(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// primitive array 用のセル
//////////////////////////////////////////////////////////////////////
class CellPrimArray :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellPrimArray(ElbPrimArray* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_primarray(ElbPrimArray* obj);

  /// @brief primitive array の先頭を得る．
  virtual
  ElbPrimArray*
  primarray();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbPrimArray* mTop;

  // 末尾の要素
  ElbPrimArray* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellPrimArray::CellPrimArray(ElbPrimArray* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellPrimArray::add_primarray(ElbPrimArray* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief primitive array の先頭を得る．
ElbPrimArray*
CellPrimArray::primarray()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellPrimArray::num()
{
  return mNum;
}

// @brief primitive array を追加する．
// @param[in] parent 親のスコープ
// @param[in] obj 登録する要素
void
TagHash::add_primarray(ElbPrimArray* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiPrimitiveArray);
  if ( cell ) {
    cell->add_primarray(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellPrimArray));
    TagHashCell* cell = new (p) CellPrimArray(obj);
    put_cell(parent, vpiPrimitiveArray, cell);
  }
}

// @brief primitive array のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_primarray_list(const VlNamedObj* parent,
			     vector<const VlPrimArray*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiPrimitiveArray);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbPrimArray* obj = cell->primarray(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// primitive 用のセル
//////////////////////////////////////////////////////////////////////
class CellPrimitive :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellPrimitive(ElbPrimitive* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_primitive(ElbPrimitive* obj);

  /// @brief primitive の先頭を得る．
  virtual
  ElbPrimitive*
  primitive();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbPrimitive* mTop;

  // 末尾の要素
  ElbPrimitive* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellPrimitive::CellPrimitive(ElbPrimitive* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellPrimitive::add_primitive(ElbPrimitive* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief primitive の先頭を得る．
ElbPrimitive*
CellPrimitive::primitive()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellPrimitive::num()
{
  return mNum;
}

// @brief primitive を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_primitive(ElbPrimitive* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiPrimitive);
  if ( cell ) {
    cell->add_primitive(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellPrimitive));
    TagHashCell* cell = new (p) CellPrimitive(obj);
    put_cell(parent, vpiPrimitive, cell);
  }
}

// @brief primitive のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_primitive_list(const VlNamedObj* parent,
			     vector<const VlPrimitive*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiPrimitive);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbPrimitive* obj = cell->primitive(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// タスク用のセル
//////////////////////////////////////////////////////////////////////
class CellTask :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellTask(ElbTask* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_task(ElbTask* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbTask*
  task();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbTask* mTop;

  // 末尾の要素
  ElbTask* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellTask::CellTask(ElbTask* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellTask::add_task(ElbTask* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbTask*
CellTask::task()
{
  return mTop;
}
  
// @brief 要素数の取得
ymuint32
CellTask::num()
{
  return mNum;
}

// @brief タスクを追加する．
// @param[in] obj 登録する要素
void
TagHash::add_task(ElbTask* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiTask);
  if ( cell ) {
    cell->add_task(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellTask));
    TagHashCell* cell = new (p) CellTask(obj);
    put_cell(parent, vpiTask, cell);
  }
}

// @brief タスクのリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_task_list(const VlNamedObj* parent,
			vector<const VlTask*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiTask);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbTask* obj = cell->task(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// 関数用のセル
//////////////////////////////////////////////////////////////////////
class CellFunction :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellFunction(ElbFunction* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_function(ElbFunction* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbFunction*
  function();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbFunction* mTop;

  // 末尾の要素
  ElbFunction* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellFunction::CellFunction(ElbFunction* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellFunction::add_function(ElbFunction* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbFunction*
CellFunction::function()
{
  return mTop;
}
  
// @brief 要素数の取得
ymuint32
CellFunction::num()
{
  return mNum;
}

// @brief function を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_function(ElbFunction* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    cell->add_function(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellFunction));
    TagHashCell* cell = new (p) CellFunction(obj);
    put_cell(parent, vpiFunction, cell);
  }
}

// @brief function のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_function_list(const VlNamedObj* parent,
			    vector<const VlFunction*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiFunction);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbFunction* obj = cell->function(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// continuous assignment 用のセル
//////////////////////////////////////////////////////////////////////
class CellContAssign :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellContAssign(ElbContAssign* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_contassign(ElbContAssign* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbContAssign*
  contassign();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbContAssign* mTop;

  // 末尾の要素
  ElbContAssign* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellContAssign::CellContAssign(ElbContAssign* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellContAssign::add_contassign(ElbContAssign* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbContAssign*
CellContAssign::contassign()
{
  return mTop;
}
  
// @brief 要素数の取得
ymuint32
CellContAssign::num()
{
  return mNum;
}

// @brief continuous assignment を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_contassign(ElbContAssign* obj)
{
  const VlNamedObj* parent = obj->module();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiContAssign);
  if ( cell ) {
    cell->add_contassign(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellContAssign));
    TagHashCell* cell = new (p) CellContAssign(obj);
    put_cell(parent, vpiContAssign, cell);
  }
}

// @brief continuous assignment のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_contassign_list(const VlNamedObj* parent,
			      vector<const VlContAssign*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiContAssign);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbContAssign* obj = cell->contassign(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// process 用のセル
//////////////////////////////////////////////////////////////////////
class CellProcess :
  public TagHashCell
{
public:

  /// @brief コンストラクタ
  CellProcess(ElbProcess* obj);
  
  /// @brief 要素の追加
  virtual
  void
  add_process(ElbProcess* obj);

  /// @brief 要素の先頭を得る．
  virtual
  ElbProcess*
  process();
  
  /// @brief 要素数の取得
  virtual
  ymuint32
  num();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 先頭の要素
  ElbProcess* mTop;

  // 末尾の要素
  ElbProcess* mTail;

  // 要素数
  ymuint32 mNum;
  
};

// @brief コンストラクタ
CellProcess::CellProcess(ElbProcess* obj) :
  mTop(obj),
  mTail(obj),
  mNum(1)
{
}
  
// @brief 要素の追加
void
CellProcess::add_process(ElbProcess* obj)
{
  mTail->mNext = obj;
  mTail = obj;
  ++ mNum;
}

// @brief 要素の先頭を得る．
ElbProcess*
CellProcess::process()
{
  return mTop;
}

// @brief 要素数の取得
ymuint32
CellProcess::num()
{
  return mNum;
}

// @brief process を追加する．
// @param[in] obj 登録する要素
void
TagHash::add_process(ElbProcess* obj)
{
  const VlNamedObj* parent = obj->parent();
  
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiProcess);
  if ( cell ) {
    cell->add_process(obj);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(CellProcess));
    TagHashCell* cell = new (p) CellProcess(obj);
    put_cell(parent, vpiProcess, cell);
  }
}

// @brief process のリストを取り出す．
// @param[in] parent 親のスコープ
// @param[out] obj_list 結果を格納するリスト
// @retval true 該当する要素が1つ以上あった．
// @retval false 該当する要素がなかった．
bool
TagHash::find_process_list(const VlNamedObj* parent,
			   vector<const VlProcess*>& obj_list) const
{
  // 該当の Cell が存在するか調べる．
  TagHashCell* cell = find_cell(parent, vpiProcess);
  if ( cell ) {
    obj_list.clear();
    obj_list.reserve(cell->num());
    for (ElbProcess* obj = cell->process(); obj; obj = obj->mNext) {
      obj_list.push_back(obj);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_VERILOG

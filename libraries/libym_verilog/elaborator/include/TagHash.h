#ifndef LIBYM_VERILOG_ELB_TAGHASH_H
#define LIBYM_VERILOG_ELB_TAGHASH_H

/// @file libym_verilog/elaborator/include/TagHash.h
/// @brief TagHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TagHash.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_utils/Alloc.h>
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class TagHashCell TagHashCell.h "TagHashCell.h"
/// @brief TagHash 用のセル
//////////////////////////////////////////////////////////////////////
class TagHashCell
{
  friend class TagHash;
  
private:
  
  /// @brief  internal scope を追加する．
  virtual
  void
  add_internalscope(ElbScopeHandle* obj);
  
  /// @brief  internal scope の先頭を得る．
  virtual
  ElbScopeHandle*
  internalscope();
  
  /// @brief  宣言要素を追加する．
  virtual
  void
  add_decl(ElbDeclBase* obj);
  
  /// @brief  宣言要素の先頭を得る．
  virtual
  ElbDeclBase*
  decl();
  
  /// @brief parameter 宣言を追加する．
  virtual
  void
  add_parameter(ElbParameter* obj);
  
  /// @brief parameter 宣言の先頭を得る．
  virtual
  ElbParameter*
  parameter();
  
  /// @brief  defparam を追加する．
  virtual
  void
  add_defparam(ElbDefParam* obj);
  
  /// @brief  defparam の先頭を得る．
  virtual
  ElbDefParam*
  defparam();
  
  /// @brief  param assign を追加する．
  virtual
  void
  add_paramassign(ElbParamAssign* obj);
  
  /// @brief  param assign の先頭を得る．
  virtual
  ElbParamAssign*
  paramassign();
  
  /// @brief module array を追加する．
  virtual
  void
  add_modulearray(ElbModuleArray* obj);
  
  /// @brief module array の先頭を得る．
  virtual
  ElbModuleArray*
  modulearray();
  
  /// @brief  module を追加する．
  virtual
  void
  add_module(ElbModule* obj);
  
  /// @brief  module の先頭を得る．
  virtual
  ElbModule*
  module();
  
  /// @brief  primitive array を追加する．
  virtual
  void
  add_primarray(ElbPrimArray* obj);
  
  /// @brief  primitive array の先頭を得る．
  virtual
  ElbPrimArray*
  primarray();
  
  /// @brief  primitive を追加する．
  virtual
  void
  add_primitive(ElbPrimitive* obj);
  
  /// @brief  primitive の先頭を得る．
  virtual
  ElbPrimitive*
  primitive();
  
  /// @brief タスクを追加する．
  virtual
  void
  add_task(ElbTask* obj);

  /// @brief タスクの先頭を得る．
  virtual
  ElbTask*
  task();

  /// @brief 関数を追加する．
  virtual
  void
  add_function(ElbFunction* obj);

  /// @brief 関数の先頭を得る．
  virtual
  ElbFunction*
  function();
  
  /// @brief continuous assignment を追加する．
  virtual
  void
  add_contassign(ElbContAssign* obj);
  
  /// @brief  continuous assignment の先頭を得る．
  virtual
  ElbContAssign*
  contassign();
  
  /// @brief  process を追加する．
  virtual
  void
  add_process(ElbProcess* process);
  
  /// @brief  process の先頭を得る．
  virtual
  ElbProcess*
  process();
  
  /// @brief  要素数を得る．
  virtual
  ymuint32
  num() = 0;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;
    
  // タグの値
  int mTag;

  // ハッシュ上の次の要素を指すポインタ
  TagHashCell* mLink;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TagHash TagHash.h "TagHash.h"
/// @brief 各スコープの構成要素リストを格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class TagHash
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のオブジェクト
  TagHash(AllocBase& alloc);

  /// @brief デストラクタ
  ~TagHash();


public:
  
  /// @brief 内容を空にする
  void
  clear();
  
  /// @brief internal scope を追加する．
  /// @param[in] scope 登録する要素
  void
  add_internalscope(ElbScopeHandle* scope);

  /// @brief internal scope のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] scope_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_internalscope_list(const VlNamedObj* parent,
			  vector<const VlScope*>& scope_list) const;

  /// @brief 宣言要素を追加する．
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @param[in] decl 登録する要素
  void
  add_decl(int tag,
	   ElbDeclBase* decl);
  
  /// @brief タグから該当する宣言要素のリストを返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @param[out] decl_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  /// @note scope というスコープ内の tag というタグを持つ要素を
  /// decl_list に入れる．
  bool
  find_decl_list(const VlNamedObj* parent,
		 int tag,
		 vector<const VlDecl*>& decl_list) const;

  /// @brief parameter 宣言要素を追加する．
  /// @param[in] param 登録する要素
  void
  add_parameter(ElbParameter* param);
  
  /// @brief parameter 宣言のリストを返す．
  /// @param[in] parent 親のスコープ
  /// @param[out] param_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  /// @note scope というスコープ内の parameter 宣言を
  /// param_list に入れる．
  bool
  find_param_list(const VlNamedObj* parent,
		  vector<const VlDecl*>& param_list) const;

  /// @brief defparam を追加する．
  /// @param[in] defparam 登録する要素
  void
  add_defparam(ElbDefParam* defparam);

  /// @brief defparam のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] defparam_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_defparam_list(const VlNamedObj* parent,
		     vector<const VlDefParam*>& defparam_list) const;

  /// @brief param assign を追加する．
  /// @param[in] paramassign 登録する要素
  void
  add_paramassign(ElbParamAssign* paramassign);

  /// @brief param assign のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] paramassign_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_paramassign_list(const VlNamedObj* parent,
			vector<const VlParamAssign*>& paramassign_list) const;

  /// @brief module array を追加する．
  /// @param[in] modulearray 登録する要素
  void
  add_modulearray(ElbModuleArray* modulearray);

  /// @brief module array のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] modulearray_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_modulearray_list(const VlNamedObj* parent,
			vector<const VlModuleArray*>& modulearray_list) const;

  /// @brief module を追加する．
  /// @param[in] module 登録する要素
  void
  add_module(ElbModule* module);

  /// @brief module のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] module_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_module_list(const VlNamedObj* parent,
		   vector<const VlModule*>& module_list) const;

  /// @brief primitive array を追加する．
  /// @param[in] primarray 登録する要素
  void
  add_primarray(ElbPrimArray* primarray);

  /// @brief primitive array のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] primarray_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_primarray_list(const VlNamedObj* parent,
		      vector<const VlPrimArray*>& primarray_list) const;

  /// @brief primitive を追加する．
  /// @param[in] primitive 登録する要素
  void
  add_primitive(ElbPrimitive* primitive);

  /// @brief primitive のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] primitive_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_primitive_list(const VlNamedObj* parent,
		      vector<const VlPrimitive*>& primitive_list) const;
  
  /// @brief タスクを追加する．
  /// @param[in] task 追加する要素
  void
  add_task(ElbTask* task);

  /// @brief タスクのリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] task_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_task_list(const VlNamedObj* parent,
		 vector<const VlTask*>& task_list) const;
  
  /// @brief 関数を追加する．
  /// @param[in] func 追加する要素
  void
  add_function(ElbFunction* func);

  /// @brief 関数のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] func_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_function_list(const VlNamedObj* parent,
		     vector<const VlFunction*>& func_list) const;

  /// @brief continuous assignment を追加する．
  /// @param[in] contassign 登録する要素
  void
  add_contassign(ElbContAssign* contassign);

  /// @brief continuous assignment のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] contassign_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_contassign_list(const VlNamedObj* parent,
		       vector<const VlContAssign*>& contassign_list) const;

  /// @brief process を追加する．
  /// @param[in] process 登録する要素
  void
  add_process(ElbProcess* process);

  /// @brief process のリストを取り出す．
  /// @param[in] parent 親のスコープ
  /// @param[out] process_list 結果を格納するリスト
  /// @retval true 該当する要素が1つ以上あった．
  /// @retval false 該当する要素がなかった．
  bool
  find_process_list(const VlNamedObj* parent,
		    vector<const VlProcess*>& process_list) const;

  /// @brief このオブジェクトが使用しているメモリ量を返す．
  size_t
  allocated_size() const;

  
private:

  /// @brief Cell を登録する．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  /// @param[in] cell 対象の Cell
  void
  put_cell(const VlNamedObj* parent,
	   int tag,
	   TagHashCell* cell);
  
  /// @brief タグから該当する Cell を探す．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  TagHashCell*
  find_cell(const VlNamedObj* parent,
	    int tag) const;

  /// @brief テーブルの領域を確保する．
  /// @param[in] size 必要なサイズ
  void
  alloc_table(ymuint32 size);
  
  /// @brief ハッシュ値を計算する．
  /// @param[in] parent 親のスコープ
  /// @param[in] tag 要素の型を表すタグ (vpi_user.h 参照)
  ymuint32
  hash_func(const VlNamedObj* parent,
	    int tag) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // Cell の確保用アロケータ
  AllocBase& mAlloc;
  
  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  TagHashCell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_TAGHASH_H

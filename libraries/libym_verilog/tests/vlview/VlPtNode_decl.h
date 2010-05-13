#ifndef VLPTNODE_DECL_H
#define VLPTNODE_DECL_H

/// @file libym_verilog/tests/vlview/VlPtNode_decl.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_decl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VlPtNode.h"
#include <ym_verilog/verilog.h>
#include <ym_verilog/pt/PtP.h>
#include <ym_verilog/pt/PtArray.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class IOHeadListNode VlPtNode_decl.h
/// @brief IO宣言ヘッダリストを表すノード
//////////////////////////////////////////////////////////////////////
class IOHeadListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] iohead_array IO宣言ヘッダの配列
  IOHeadListNode(const PtIOHeadArray& iohead_array);

  /// @brief デストラクタ
  virtual
  ~IOHeadListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IO宣言ヘッダリスト
  PtIOHeadArray mIOHeadArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class IOHeadNode VlPtNode_decl.h
/// @brief IO宣言ヘッダを表すノード
//////////////////////////////////////////////////////////////////////
class IOHeadNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] iohead IO宣言ヘッダ
  IOHeadNode(const PtIOHead* iohead);

  /// @brief デストラクタ
  virtual
  ~IOHeadNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IO宣言ヘッダ
  const PtIOHead* mIOHead;
  
};


//////////////////////////////////////////////////////////////////////
/// @class IOItemListNode VlPtNode_decl.h
/// @brief IO宣言要素のリストを表すノード
//////////////////////////////////////////////////////////////////////
class IOItemListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] iohead IO宣言ヘッダ
  IOItemListNode(const PtIOHead* iohead);

  /// @brief デストラクタ
  virtual
  ~IOItemListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IO宣言ヘッダ
  const PtIOHead* mIOHead;
  
};


//////////////////////////////////////////////////////////////////////
/// @class IOItemNode VlPtNode_decl.h
/// @brief IO宣言要素を表すノード
//////////////////////////////////////////////////////////////////////
class IOItemNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] ioitem IO宣言要素
  IOItemNode(const PtIOItem* ioitem);

  /// @brief デストラクタ
  virtual
  ~IOItemNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IO宣言要素
  const PtIOItem* mIOItem;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DeclHeadListNode VlPtNode_decl.h
/// @brief 宣言ヘッダリストを表すノード
//////////////////////////////////////////////////////////////////////
class DeclHeadListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] declhead_array 宣言ヘッダの配列
  DeclHeadListNode(const QString& label,
		   const PtDeclHeadArray& declhead_array);

  /// @brief デストラクタ
  virtual
  ~DeclHeadListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QString mLabel;
  
  // 宣言ヘッダリスト
  PtDeclHeadArray mDeclHeadArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DeclHeadNode VlPtNode_decl.h
/// @brief 宣言ヘッダを表すノード
//////////////////////////////////////////////////////////////////////
class DeclHeadNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] declhead 宣言ヘッダ
  DeclHeadNode(const PtDeclHead* declhead);

  /// @brief デストラクタ
  virtual
  ~DeclHeadNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言ヘッダ
  const PtDeclHead* mDeclHead;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DeclItemListNode VlPtNode_decl.h
/// @brief Decl宣言要素のリストを表すノード
//////////////////////////////////////////////////////////////////////
class DeclItemListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] declhead IO宣言ヘッダ
  DeclItemListNode(const PtDeclHead* iohead);

  /// @brief デストラクタ
  virtual
  ~DeclItemListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言ヘッダ
  const PtDeclHead* mDeclHead;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DeclItemNode VlPtNode_decl.h
/// @brief 宣言要素を表すノード
//////////////////////////////////////////////////////////////////////
class DeclItemNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] declitem IO宣言要素
  DeclItemNode(const PtDeclItem* declitem);

  /// @brief デストラクタ
  virtual
  ~DeclItemNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言要素
  const PtDeclItem* mDeclItem;
  
};

//////////////////////////////////////////////////////////////////////
/// @class RangeListNode VlPtNode_misc.h
/// @brief 配列の範囲のリストを表すノード
//////////////////////////////////////////////////////////////////////
class RangeListNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] declitem 親の宣言要素
  RangeListNode(const PtDeclItem* declitem);

  /// @brief デストラクタ
  ~RangeListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言要素
  const PtDeclItem* mDeclItem;
  
};


//////////////////////////////////////////////////////////////////////
/// @class RangeNode VlPtNode_misc.h
/// @brief 配列の範囲を表すノード
//////////////////////////////////////////////////////////////////////
class RangeNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] range 範囲
  RangeNode(const PtRange* range);

  /// @brief デストラクタ
  ~RangeNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const;
    
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const PtRange* mRange;
  
};

END_NAMESPACE_YM_VERILOG

#endif // VLPTNODE_DECL_H

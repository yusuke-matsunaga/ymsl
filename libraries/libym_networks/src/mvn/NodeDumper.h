#ifndef NODEDUMPER_H
#define NODEDUMPER_H

/// @file NodeDumper.h
/// @brief NodeDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/mvn.h"

BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class NodeDumper NodeDumper.h
/// @brief MvnNode の内容を Verilog-HDL の形式で出力するためのクラス
//////////////////////////////////////////////////////////////////////
class NodeDumper
{
public:

  /// @brief デストラクタ
  virtual
  ~NodeDumper() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class UnaryOpDumper NodeDumper.h
/// @brief 単項演算子用のクラス
//////////////////////////////////////////////////////////////////////
class UnaryOpDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] opr_str 演算子を表す文字列
  UnaryOpDumper(const char* opr_str);

  /// @brief デストラクタ
  virtual
  ~UnaryOpDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子を表す文字列
  const char* mOprStr;

};


//////////////////////////////////////////////////////////////////////
/// @class BinaryOpDumper NodeDumper.h
/// @brief 二項演算子用のクラス
//////////////////////////////////////////////////////////////////////
class BinaryOpDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] opr_str 演算子を表す文字列
  /// @param[in] need_paren 括弧をつける時に true にするフラグ
  BinaryOpDumper(const char* opr_str,
		 bool need_paren = false);

  /// @brief デストラクタ
  virtual
  ~BinaryOpDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子を表す文字列
  const char* mOprStr;

  // 括弧を制御するフラグ
  bool mNeedParen;

};


//////////////////////////////////////////////////////////////////////
/// @class TernaryOpDumper NodeDumper.h
/// @brief さん項演算子用のクラス
//////////////////////////////////////////////////////////////////////
class TernaryOpDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] opr_str1, opr_str2 演算子を表す文字列
  TernaryOpDumper(const char* opr_str1,
		  const char* opr_str2);

  /// @brief デストラクタ
  virtual
  ~TernaryOpDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子を表す文字列その1
  const char* mOprStr1;

  // 演算子を表す文字列その2
  const char* mOprStr2;

};


//////////////////////////////////////////////////////////////////////
/// @class NaryOpDumper NodeDumper.h
/// @brief 多項演算子用のクラス
//////////////////////////////////////////////////////////////////////
class NaryOpDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] opr_str 演算子を表す文字列
  NaryOpDumper(const char* opr_str);

  /// @brief デストラクタ
  virtual
  ~NaryOpDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子を表す文字列
  const char* mOprStr;

};


//////////////////////////////////////////////////////////////////////
/// @class ConcatDumper NodeDumper.h
/// @brief 連結演算子用のクラス
//////////////////////////////////////////////////////////////////////
class ConcatDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  ConcatDumper();

  /// @brief デストラクタ
  virtual
  ~ConcatDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class CaseEqDumper NodeDumper.h
/// @brief CaseEq演算子用のクラス
//////////////////////////////////////////////////////////////////////
class CaseEqDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  CaseEqDumper();

  /// @brief デストラクタ
  virtual
  ~CaseEqDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class ConstBitSelectDumper NodeDumper.h
/// @brief ConstBitSelect演算子用のクラス
//////////////////////////////////////////////////////////////////////
class ConstBitSelectDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  ConstBitSelectDumper();

  /// @brief デストラクタ
  virtual
  ~ConstBitSelectDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class ConstPartSelectDumper NodeDumper.h
/// @brief ConstPartSelect演算子用のクラス
//////////////////////////////////////////////////////////////////////
class ConstPartSelectDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  ConstPartSelectDumper();

  /// @brief デストラクタ
  virtual
  ~ConstPartSelectDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class BitSelectDumper NodeDumper.h
/// @brief BitSelect演算子用のクラス
//////////////////////////////////////////////////////////////////////
class BitSelectDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  BitSelectDumper();

  /// @brief デストラクタ
  virtual
  ~BitSelectDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class PartSelectDumper NodeDumper.h
/// @brief PartSelect演算子用のクラス
//////////////////////////////////////////////////////////////////////
class PartSelectDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  PartSelectDumper();

  /// @brief デストラクタ
  virtual
  ~PartSelectDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);

};


//////////////////////////////////////////////////////////////////////
/// @class ConstDumper NodeDumper.h
/// @brief Const演算子用のクラス
//////////////////////////////////////////////////////////////////////
class ConstDumper :
  public NodeDumper
{
public:

  /// @brief コンストラクタ
  ConstDumper();

  /// @brief デストラクタ
  virtual
  ~ConstDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // NodeDumper の継承クラス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  virtual
  void
  operator()(ostream& s,
	     const MvnNode* node);

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // NODEDUMPER_H

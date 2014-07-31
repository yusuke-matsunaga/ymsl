#ifndef BLIFNODEIMPL_H
#define BLIFNODEIMPL_H

/// @file BlifNodeImpl.h
/// @brief BlifNodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BlifNode.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifNodeImpl BlifNodeImpl.h "BlifNodeImpl.h"
/// @brief BlifNode の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class BlifNodeImpl :
  public BlifNode
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  /// @param[in] name ノード名
  BlifNodeImpl(ymuint32 id,
	       const char* name);

  /// @brief デストラクタ
  virtual
  ~BlifNodeImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 全タイプ共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID を返す．
  virtual
  ymuint32
  id() const;

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理タイプ/ゲートタイプに共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン数を得る．
  virtual
  ymuint32
  fanin_num() const;

  /// @brief ファンインのノードIDを返す．
  /// @param[in] pos 入力位置 ( 0 <= pos < fanin_num() )
  virtual
  ymuint32
  fanin_id(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理タイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーのキューブ数を得る．
  virtual
  ymuint32
  cube_num() const;

  /// @brief 入力キューブのパタンを得る．
  /// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
  /// @param[in] i_pos 入力位置 ( 0 <= i_pos < fanin_num() )
  /// @note 意味のあるパタンは '0' '1' '-'
  virtual
  char
  cube_pat(ymuint32 c_pos,
	   ymuint32 i_pos) const;

  /// @brief 出力キューブを表すパタンを得る．
  /// @note 意味のあるパタンは '0' '1'
  virtual
  char
  opat() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ゲートタイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを返す．
  virtual
  const Cell*
  cell() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ラッチタイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードのID番号を返す．
  virtual
  ymuint32
  inode_id() const;

  /// @brief リセット値を返す．
  virtual
  char
  reset_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  // ここのメモリ領域はすべて BlifNetworkImpl::mAlloc が管理する．
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @class BlifInputNode BlifNodeImpl.h "BlifNodeImpl.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BlifInputNode :
  public BlifNodeImpl
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  /// @param[in] name ノード名
  BlifInputNode(ymuint32 id,
		const char* name);

  /// @brief デストラクタ
  virtual
  ~BlifInputNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 全タイプ共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief 内容を blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  write_blif(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class BlifNodeImpl2 BlifNodeImpl.h "BlifNodeImpl.h"
/// @brief 可変ファンインを持つノードに共通の親クラス
//////////////////////////////////////////////////////////////////////
class BlifNodeImpl2 :
  public BlifNodeImpl
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name ノード名
  /// @param[in] ni ファンイン数
  /// @param[in] fanins ファンインのID番号の配列
  BlifNodeImpl2(ymuint32 id,
		const char* name,
		ymuint32 ni,
		const ymuint32* fanins);

  /// @brief デストラクタ
  virtual
  ~BlifNodeImpl2();


public:
  //////////////////////////////////////////////////////////////////////
  // 論理タイプ/ゲートタイプに共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン数を得る．
  virtual
  ymuint32
  fanin_num() const;

  /// @brief ファンインのノードIDを返す．
  /// @param[in] pos 入力位置 ( 0 <= pos < fanin_num() )
  virtual
  ymuint32
  fanin_id(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  // ここのメモリ領域はすべて BlifNetworkImpl::mAlloc が管理する．
  //////////////////////////////////////////////////////////////////////

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインのIDの配列
  const ymuint32* mFanins;

};


//////////////////////////////////////////////////////////////////////
/// @class BlifLogicNode BlifNodeImpl.h "BlifNodeImpl.h"
/// @brief 論理ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BlifLogicNode :
  public BlifNodeImpl2
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name ノード名
  /// @param[in] ni ファンイン数
  /// @param[in] fanins ファンインのID番号の配列
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 入力キューブのパタンをすべてつなげた文字列
  /// @param[in] opat 出力キューブのパタン
  BlifLogicNode(ymuint32 id,
		const char* name,
		ymuint32 ni,
		const ymuint32* fanins,
		ymuint32 nc,
		const char* cover_pat,
		char opat);

  /// @brief デストラクタ
  virtual
  ~BlifLogicNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 全タイプ共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief 内容を blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  write_blif(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理タイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーのキューブ数を得る．
  virtual
  ymuint32
  cube_num() const;

  /// @brief 入力キューブのパタンを得る．
  /// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
  /// @param[in] i_pos 入力位置 ( 0 <= i_pos < fanin_num() )
  /// @note 意味のあるパタンは '0' '1' '-'
  virtual
  char
  cube_pat(ymuint32 c_pos,
	   ymuint32 i_pos) const;

  /// @brief 出力キューブを表すパタンを得る．
  /// @note 意味のあるパタンは '0' '1'
  virtual
  char
  opat() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブ数
  ymuint32 mCubeNum;

  // 入力キューブを表す文字列
  const char* mCoverPat;

  // 出力のパタン
  char mOpat;

};


//////////////////////////////////////////////////////////////////////
/// @class BlifGateNode BlifNodeImpl.h "BlifNodeImpl.h"
/// @brief ゲートノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BlifGateNode :
  public BlifNodeImpl2
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name ノード名
  /// @param[in] ni ファンイン数
  /// @param[in] fanins ファンインのID番号の配列
  /// @param[in] cell セルへのポインタ
  BlifGateNode(ymuint32 id,
	       const char* name,
	       ymuint32 ni,
	       const ymuint32* fanins,
	       const Cell* cell);

  /// @brief デストラクタ
  virtual
  ~BlifGateNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 全タイプ共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief 内容を blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  write_blif(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ゲートタイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを返す．
  virtual
  const Cell*
  cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  const Cell* mCell;

};


//////////////////////////////////////////////////////////////////////
/// @class BlifLatchNode BlifNodeImpl.h "BlifNodeImpl.h"
/// @brief ラッチノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BlifLatchNode :
  public BlifNodeImpl
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  /// @param[in] name ノード名
  /// @param[in] inode_id 入力ノードのID番号
  /// @param[in] rval リセット値
  BlifLatchNode(ymuint32 id,
		const char* name,
		ymuint32 inode_id,
		char rval);

  /// @brief デストラクタ
  virtual
  ~BlifLatchNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 全タイプ共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief 内容を blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  write_blif(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ラッチタイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードのID番号を返す．
  virtual
  ymuint32
  inode_id() const;

  /// @brief リセット値を返す．
  virtual
  char
  reset_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ノードのID
  ymuint32 mInodeId;

  // リセット値
  char mResetVal;

};

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFNODEIMPL_H

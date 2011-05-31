#ifndef LIBYM_MVN_VERILOG_READERIMPL_H
#define LIBYM_MVN_VERILOG_READERIMPL_H

/// @file libym_networks/verilog/ReaderImpl.h
/// @brief ReaderImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn_nsdef.h"
#include "ym_networks/MvnVlMap.h"
#include "ym_verilog/VlMgr.h"
#include "ym_verilog/vl/VlFwd.h"
#include "DeclHash.h"
#include "DeclMap.h"
#include "Driver.h"
#include "Env.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ReaderImpl ReaderImpl.h "ReaderImpl.h"
/// @brief 合成可能な Verilog 記述を読み込んで MVN に設定するクラス
//////////////////////////////////////////////////////////////////////
class ReaderImpl
{
public:

  /// @brief コンストラクタ
  ReaderImpl();

  /// @brief デストラクタ
  ~ReaderImpl();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief verilog 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] searchpath サーチパス
  /// @param[in] watcher_list 行番号ウォッチャーのリスト
  /// @retval true 正常に読み込めた．
  /// @retval false 読込中にエラーが起こった．
  bool
  read(const string& filename,
       const SearchPathList& searchpath = SearchPathList(),
       const list<VlLineWatcher*> watcher_list = list<VlLineWatcher*>());

  /// @brief 今まで読み込んだ情報からネットワークを生成する．
  /// @param[in] mgr ネットワーク生成用のマネージャ
  /// @param[out] node_map MvnNode と宣言要素の対応付けを保持する配列
  /// @retval true 正常に処理を行った．
  /// @retval false 生成中にエラーが起こった．
  bool
  gen_network(MvnMgr& mgr,
	      MvnVlMap& node_map);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module を生成する．
  /// @param[in] vl_module 対象のモジュール
  MvnModule*
  gen_module(const VlModule* vl_module);

  /// @brief モジュール内部の要素を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] vl_module 対象のモジュール
  void
  gen_moduleitem(MvnModule* module,
		 const VlModule* vl_module);

  /// @brief 宣言要素を生成する．
  /// @param[in] module モジュール
  /// @param[in] vl_scope 対象のスコープ
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  /// @note 内部に下位のスコープを含む場合には再帰する．
  bool
  gen_decl(MvnModule* module,
	   const VlNamedObj* vl_scope);

  /// @brief 要素を生成する．
  /// @param[in] module モジュール
  /// @param[in] vl_scope 対象のスコープ
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  /// @note 内部に下位のスコープを含む場合には再帰する．
  bool
  gen_item(MvnModule* module,
	   const VlNamedObj* vl_scope);

  /// @brief ポート参照式の実体化を行う．
  /// @param[in] expr 対象の式
  /// @param[out] node 対応するノードを格納する変数
  /// @param[out] msb ビット指定位置か範囲指定の MSB を格納する変数
  /// @param[out] lsb 範囲指定の LSB を格納する変数
  /// @retval 0 単純な形式だった．
  /// @retval 1 ビット指定形式だった．
  /// @retval 2 範囲指定形式だった．
  int
  gen_portref(const VlExpr* expr,
	      MvnNode*& node,
	      ymuint& msb,
	      ymuint& lsb);

  /// @brief プロセス文を生成する．
  /// @param[in] vl_process プロセス文
  bool
  gen_process(MvnModule* parent_module,
	      const VlProcess* process);

  /// @brief モジュールインスタンスの生成を行う．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] vl_module モジュール
  void
  gen_moduleinst(MvnModule* parent_module,
		 const VlModule* vl_module);

  /// @brief プリミティブインスタンスの生成を行う．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] prim プリミティブ
  void
  gen_priminst(MvnModule* parent_module,
	       const VlPrimitive* prim);

  /// @brief 継続的代入文の生成を行う．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  void
  gen_cont_assign(MvnModule* parent_module,
		  const VlExpr* lhs,
		  const VlExpr* rhs);

  /// @brief ステートメントの中身を生成する(combinational always用)．
  /// @param[in] module 親のモジュール
  /// @param[in] stmt 本体のステートメント
  /// @param[in] env 環境
  bool
  gen_stmt1(MvnModule* module,
	    const VlStmt* stmt,
	    ProcEnv& env);

  /// @brief ステートメントの中身を生成する(sequential always用)．
  /// @param[in] module 親のモジュール
  /// @param[in] stmt 本体のステートメント
  /// @param[in] env 環境
  bool
  gen_stmt2(MvnModule* module,
	    const VlStmt* stmt,
	    ProcEnv& env);

  /// @brief 代入文を生成する．
  /// @param[in] module 親のモジュール
  /// @param[in] stmt 本体のステートメント
  /// @param[in] env 環境
  void
  gen_assign(MvnModule* module,
	     const VlStmt* stmt,
	     ProcEnv& env);

  /// @brief 環境をマージする．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] env 対象の環境
  /// @param[in] cond 条件を表すノード
  /// @param[in] then_env 条件が成り立ったときに通るパスの環境
  /// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
  void
  merge_env1(MvnModule* parent_module,
	     ProcEnv& env,
	     MvnNode* cond,
	     const ProcEnv& then_env,
	     const ProcEnv& else_env);

  /// @brief 環境をマージする．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] env 対象の環境
  /// @param[in] cond 条件を表すノード
  /// @param[in] then_env 条件が成り立ったときに通るパスの環境
  /// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
  void
  merge_env2(MvnModule* parent_module,
	     ProcEnv& env,
	     MvnNode* cond,
	     const ProcEnv& then_env,
	     const ProcEnv& else_env);

  /// @brief 代入条件をマージする．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] cond 切り替え条件
  /// @param[in] then_cond cond が成り立ったときの代入条件
  /// @param[in] else_cond cond が成り立たなかったときの代入条件
  /// @note 基本的には ITE(cond, then_cond, else_cond) だが，NULL の場合がある．
  MvnNode*
  merge_cond(MvnModule* parent_module,
	     MvnNode* cond,
	     MvnNode* then_cond,
	     MvnNode* else_cond);

  /// @brief 式に対応したノードの木を作る．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] expr 式
  /// @param[in] env 環境
  MvnNode*
  gen_expr(MvnModule* parent_module,
	   const VlExpr* expr,
	   const Env& env);

  /// @brief 演算に対応したノードの木を作る．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] op_type 演算の種類
  /// @param[in] operand_array オペランドに対応するノードの配列
  /// @param[in] out_bw 出力のビット幅
  MvnNode*
  gen_opr(MvnModule* parent_module,
	  nsYm::nsVerilog::tVpiOpType op_type,
	  const vector<MvnNode*>& operand_array,
	  ymuint out_bw);

  /// @brief 宣言要素への参照に対応するノードを作る．
  /// @param[in] expr 式
  /// @param[in] env 環境
  MvnNode*
  gen_primary(const VlExpr* expr,
	      const Env& env);

  /// @brief 右辺式に対応するノードを返す．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] node 右辺式のノード
  /// @param[in] offset オフセット
  /// @param[in] bit_width ビット幅
  /// @note node から [offset: offset + bit_width - 1] の選択するノードを返す．
  /// @note 全範囲を選択する場合には node を返す．
  /// @note 範囲が合わなかったら NULL を返す．
  MvnNode*
  gen_rhs(MvnModule* parent_module,
	  MvnNode* node,
	  ymuint offset,
	  ymuint bit_width);

  /// @brief 左辺式に接続する．
  /// @param[in] dst_node 左辺に対応するノード
  /// @param[in] expr 左辺式
  /// @param[in] src_node 右辺に対応するノード
  void
  connect_lhs(MvnNode* dst_node,
	      const VlExpr* expr,
	      MvnNode* src_node);

  /// @brief 宣言要素に対応するノードを登録する．
  /// @param[in] decl 宣言要素
  /// @param[in] node 登録するノード
  void
  reg_node(const VlDecl* decl,
	   MvnNode* node);

  /// @brief 入出力宣言要素に対応するノードを登録する．
  /// @param[in] decl 宣言要素
  /// @param[in] node 登録するノード
  void
  reg_ionode(const VlDecl* decl,
	     MvnNode* node);

  /// @brief 宣言要素に対応するノードを登録する．
  /// @param[in] decl 宣言要素(配列型)
  /// @param[in] offset オフセット
  /// @param[in] node 登録するノード
  void
  reg_node(const VlDeclArray* decl,
	   ymuint offset,
	   MvnNode* node);

  /// @brief ドライバーを登録する．
  /// @param[in] node 左辺に対応するノード
  /// @param[in] driver ドライバー
  void
  reg_driver(MvnNode* node,
	     const Driver& driver);

  /// @brief ドライバーリストを取り出す．
  /// @param[in] node 対応するノード
  /// @note なければ空のリストを作る．
  vector<Driver>&
  driver_list(MvnNode* node);

  /// @brief 複数のドライバがある時にエラーメッセージを出力する．
  void
  error_drivers(MvnNode* node,
		const Driver& driver1,
		const Driver& driver2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Verilog を扱うマネージャ
  VlMgr mVlMgr;

  // MvNetwork を扱うマネージャ
  MvnMgr* mMvnMgr;

  // VlIODecl と MvnNode の対応付けをとるハッシュ表
  DeclMap mIODeclMap;

  // REG 型オブジェクトに ID 番号を割り当てるためのハッシュ表
  DeclHash mDeclHash;

  // トップレベルの環境
  Env mGlobalEnv;

  // MvnNode の ID番号をキーとして VlDecl/VlDeclArray の情報を保持する配列
  MvnVlMap mNodeMap;

  // VlDecl のドライバーのリスト
  vector<vector<Driver> > mDriverList;

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_READERIMPL_H
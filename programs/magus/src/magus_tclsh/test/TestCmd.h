#ifndef MAGUS_TEST_TESTCMD_H
#define MAGUS_TEST_TESTCMD_H

/// @file src/test/TestCmd.h
/// @brief TestCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TestCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "BaseCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// Network を読んで出力するテストコマンド
//////////////////////////////////////////////////////////////////////
class TestCmd :
  public BaseCmd
{
public:

  /// @brief コンストラクタ
  TestCmd(NetMgr* mgr);
  

protected:

  /// @brief コマンド処理関数
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_TEST_TESTCMD_H

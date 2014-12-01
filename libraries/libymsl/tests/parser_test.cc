
/// @file parser_test.cc
/// @brief parser_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "../src/YmslParser.h"
#include "../src/RsrvWordDic.h"
#include "../src/YmslAst.h"
#include "YmUtils/FileIDO.h"
#include "YmUtils/StreamIDO.h"
#include "YmUtils/StringIDO.h"
#include "YmUtils/MsgHandler.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_YMSL


int
parser_test1(IDO& ido)
{
  StreamMsgHandler handler(&cout);
  MsgMgr::reg_handler(&handler);

  YmslParser parser;

  parser.read(ido);

  parser.root()->print(cout);

  return 0;
}

int
parser_test(int argc,
	    char** argv)
{
  if ( argc == 1 ) {
    const char* str =
      "var a:int\n"
      "a = 1\n"
      "F = a + b\n"
      "F = a\n"
      "F = a + b / c * d % e + (f > g) & (h <= k)\n"
      "//comment\n";

    StringIDO ido(str);
    return parser_test1(ido);
  }
  else {
    for (int i = 1; i < argc; ++ i) {
      FileIDO ido;
      if ( !ido.open(argv[i]) ) {
	cerr << argv[i] << ": no such file" << endl;
	return -1;
      }
      int stat = parser_test1(ido);
      if ( stat != 0 ) {
	return stat;
      }
    }
  }
  return 0;
}

END_NAMESPACE_YM_YMSL


int
main(int argc,
     char** argv)
{
  return nsYm::nsYmsl::parser_test(argc, argv);
}
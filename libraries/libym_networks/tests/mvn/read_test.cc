
/// @file read_test.cc
/// @brief MvnNode を生成するテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnVerilogReader.h"
#include "ym_networks/MvnVlMap.h"
#include "ym_networks/MvnDumper.h"
#include "ym_networks/MvnVerilogWriter.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  list<string> filename_list;
  for (ymuint i = 1; i < argc; ++ i) {
    filename_list.push_back(argv[i]);
  }

#if !defined(YM_DEBUG)
  try {
#endif
    MsgHandler* mh = new StreamMsgHandler(&cerr);
    mh->set_mask(kMaskAll);
    mh->delete_mask(kMsgInfo);
    mh->delete_mask(kMsgDebug);
    MsgMgr::reg_handler(mh);

    MvnVerilogReader reader;

    for (list<string>::const_iterator p = filename_list.begin();
	 p != filename_list.end(); ++ p) {
      const string& name = *p;
      cerr << "Reading " << name;
      cerr.flush();
      bool stat = reader.read(name);
      cerr << " end" << endl;
      if ( !stat ) {
	return 1;
      }
    }
    cerr << "Generating MvnNetwork" << endl;
    MvnMgr mgr;
    MvnVlMap node_map;
    bool stat = reader.gen_network(mgr, node_map);
    cerr << " End" << endl;
    if ( !stat ) {
      cerr << "error occured" << endl;
      return 2;
    }

    MvnDumper dump;
    dump(cout, mgr);

    MvnVerilogWriter vl_writer;

    vl_writer(cout, mgr, node_map);

#if !defined(YM_DEBUG)
  }
  catch ( AssertError x) {
    cout << x << endl;
  }
#endif
  return 0;
}


/// @file libym_networks/tests/change_test.cc
/// @brief BdnMgr::change_logic() のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNodeHandle.h"
#include "ym_networks/BdnDumper.h"


BEGIN_NAMESPACE_YM_BDN

void
change_test()
{
  BdnMgr network;
  BdnDumper dump;

  network.set_name("change_test");

  BdnPort* port_a = network.new_port("a", 1);
  BdnPort* port_b = network.new_port("b", 1);
  BdnPort* port_c = network.new_port("c", 1);
  BdnPort* port_d = network.new_port("d", 1);

  BdnNode* a = network.new_port_input(port_a, 0);
  BdnNode* b = network.new_port_input(port_b, 0);
  BdnNode* c = network.new_port_input(port_c, 0);
  BdnNode* d = network.new_port_output(port_d, 0);

  // a & b
  BdnNodeHandle h1 = network.new_and(BdnNodeHandle(a, false),
				     BdnNodeHandle(b, false));

  // a & c
  BdnNodeHandle h2 = network.new_and(BdnNodeHandle(a, false),
				     BdnNodeHandle(c, false));

  // (a & b) | (a & c)
  BdnNodeHandle h3 = network.new_or(h1, h2);

  network.change_output_fanin(d, h3);

  cout << "===Before change_logic===" << endl;
  dump(cout, network);

  // (b | c)
  BdnNodeHandle h4 = network.new_or(BdnNodeHandle(b, false),
				    BdnNodeHandle(c, false));

  network.change_and(h3.node(),
		     BdnNodeHandle(a, false),
		     h4);

  cout << "===After change_logic===" << endl;
  dump(cout, network);

  // 論理を反転
  network.change_nand(h3.node(),
		      BdnNodeHandle(a, false),
		      h4);

  cout << "===After change_logic(2)===" << endl;
  dump(cout, network);

}

END_NAMESPACE_YM_BDN


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsBdn::change_test;

  change_test();

  return 0;
}
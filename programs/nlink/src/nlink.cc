
/// @file nlink.cc
/// @brief nlink の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "NlProblem.h"
#include "NlSolver2.h"


BEGIN_NAMESPACE_YM_NLINK

void
nlink(const string& filename)
{
  NlProblem problem = read_problem(filename);

  print_problem(cout, problem);

  NlSolver2 solver;

  solver.solve(problem);
}

END_NAMESPACE_YM_NLINK


int
main(int argc,
     const char** argv)
{
  using nsYm::nsNlink::nlink;
  using namespace std;

  for (int i = 1; i < argc; ++ i) {
    nlink(argv[i]);
  }

  return 0;
}

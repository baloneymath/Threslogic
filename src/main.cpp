/***************************************************
* Author: Hao Chen (NTU 2017)                      *
***************************************************/

using namespace std;
#include "src/common/util.h"
#include "src/common/arghandler.h"
#include "src/common/paramhandler.h"

ParamHandler gParam;
int main(int argc, char** argv) {
  ArgHandler gArg(argc, argv, gParam);
  util::showSysInfo();
  util::TimeUsage timer;
  srand(time(NULL));

  Network ntk(gParam.inputFileName);
  util::showMemoryUsage();
  return 0;
}
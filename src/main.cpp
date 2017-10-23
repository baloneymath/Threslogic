/***************************************************
* Author: Hao Chen (NTU 2017)                      *
***************************************************/
#include <omp.h>
using namespace std;
#include "src/common/util.h"
#include "src/common/arghandler.h"
#include "src/common/paramhandler.h"
#include "src/logic/cirNetwork.h"
#include "src/simulation/simBnn.h"

ParamHandler gParam;
int main(int argc, char** argv) {
  ArgHandler gArg(argc, argv, gParam);
  util::showSysInfo();
  omp_set_num_threads(12);
  util::TimeUsage timer;
  srand(time(NULL));

  CirNetwork ntk(gParam.inputFileName);
  SimBnn sim(ntk);
  sim.readMNIST_test(gParam.MNIST_test);
  sim.simulateMNIST();

  util::showMemoryUsage(1);
  return 0;
}
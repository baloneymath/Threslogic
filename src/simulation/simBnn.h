#ifndef SIM_BNN_H
#define SIM_BNN_H

#include <vector>
#include "src/common/util.h"
#include "src/logic/cirNetwork.h"

class SimBnn {
  public:
    SimBnn(CirNetwork& ntk)
      : _ntk(ntk) {}
    ~SimBnn() {}

    void readMNIST_test(const string& filename);
    void simulateMNIST();

  private:
    CirNetwork& _ntk;
    vector<vector<float>> _train_X, _test_X;
    vector<int> _train_y, _test_y;
};

#endif // SIM_BNN_H
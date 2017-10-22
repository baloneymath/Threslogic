#ifndef CIR_THRESGATE_H
#define CIR_THRESGATE_H

#include <vector>
#include "cirGate.h"
#include "src/common/util.h"

class CirThresGate : public CirGate {
  public:
    CirThresGate(unsigned Id = 0, float val = 0)
      : CirGate(Id), _threshold(val) {}

    // get
    float threshold() const { return _threshold; }
    vector<int>& weights() { return _weights; }

    int computeOutput(const vector<float>& inputs) {
      assert(inputs.size() == _weights.size());
      float accum = 0;
      for (unsigned i = 0; i < inputs.size(); ++i) {
        accum += inputs[i] * _weights[i];
      }
      int val = accum >= _threshold? 1 : -1;
      setSimVal(val);
      return val;
    }

    // set
    void addWeight(int i) { _weights.push_back(i); }

  private:
    float _threshold;
    vector<int> _weights;
};

#endif // CIR_THRESGATE_H
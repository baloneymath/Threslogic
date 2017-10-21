#ifndef THRESGATE_H
#define THRESGATE_H

#include <vector>
#include "src/common/util.h"

class ThresGate {
  public:
    ThresGate(unsigned Id = 0, float val = 0)
      : _Id(Id), _threshold(val) {}
    ~ThresGate() {}

    // get
    unsigned  Id()        const { return _Id; }
    float     threshold() const { return _threshold; }
    vector<int>&      weights()   { return _weights; }
    vector<unsigned>& faninsId()  { return _faninsId; }
    vector<unsigned>& fanoutsId() { return _fanoutsId; }

    int computeOutput(const vector<float>& inputs) {
      assert(inputs.size() == _weights.size());
      float accum = 0;
      for (unsigned i = 0; i < inputs.size(); ++i) {
        accum += inputs[i] * _weights[i];
      }
      return accum >= _threshold ? 1 : -1;
    }

    // set
    void addFanin(int weight, unsigned faninId) {
      _weights.push_back(weight);
      _faninsId.push_back(faninId);
    }
    void addFanout(unsigned fanoutId) {
      _fanoutsId.push_back(fanoutId);
    }

  private:
    unsigned _Id;
    float _threshold;
    vector<int> _weights;
    vector<unsigned> _faninsId;
    vector<unsigned> _fanoutsId;
};

#endif
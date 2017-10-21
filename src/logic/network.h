#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "src/common/util.h"
#include "thresgate.h"

#define Thres_NtkForEachGate(i) \
  for (i = 0; i < numThresgate(); ++i)

class Network {
  public:
    Network(const string& filename = "") {
      if (filename != "") parse(filename);
    }
    ~Network() {}

    // get
    string name() const { return _name; }
    unsigned numThresgate() const { return _thresgates.size(); }
    ThresGate& thresgate(unsigned i) { return _thresgates[i]; }

    void parse(const string& filename);
    
  private:
    string _name;
    vector<ThresGate> _thresgates;
};


#endif
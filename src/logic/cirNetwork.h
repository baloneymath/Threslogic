#ifndef CIR_NETWORK_H
#define CIR_NETWORK_H

#include <vector>
#include "src/common/util.h"
#include "cirGate.h"
#include "cirThresGate.h"


class CirNetwork {
  public:
    CirNetwork(const string& filename = "") {
      if (filename != "") parse(filename);
    }
    ~CirNetwork() {}

    // get
    string name() const { return _name; }
    unsigned numGates() const { return _gates.size(); }
    unsigned numPiGates() const {return _piGates.size(); }
    unsigned numPoGates() const {return _poGates.size(); }
    unsigned numThresgates() const { return _thresgates.size(); }
  
    void parse(const string& filename);
    
  private:
    string _name;
    vector<CirGate>       _gates;
    vector<CirPiGate>     _piGates;
    vector<CirPoGate>     _poGates;
    vector<CirThresGate>  _thresgates;
};


#endif // CIR_NETWORK_H
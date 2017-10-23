#ifndef CIR_NETWORK_H
#define CIR_NETWORK_H

#include <vector>
#include "src/common/util.h"
#include "cirGate.h"


class CirNetwork {
  public:
    CirNetwork(const string& filename = "") {
      if (filename != "") parse(filename);
    }
    ~CirNetwork() {
      for (unsigned i = 0; i < _gates.size(); ++i)
        delete _gates[i];
    }

    // get
    string name() const { return _name; }
    unsigned numGates() const { return _gates.size(); }
    unsigned numPiGates() const {return _piGates.size(); }
    unsigned numPoGates() const {return _poGates.size(); }
    unsigned numThresgates() const { return _thresGates.size(); }
    CirGate*      gate(unsigned i)      { return _gates[i]; }
    CirPiGate*    piGate(unsigned i)    { return _piGates[i]; }
    CirPoGate*    poGate(unsigned i)    { return _poGates[i]; }
    CirThresGate* thresGate(unsigned i) { return _thresGates[i]; }
  
    void parse(const string& filename);
    
  private:
    string _name;
    vector<CirGate*>       _gates;
    vector<CirPiGate*>     _piGates;
    vector<CirPoGate*>     _poGates;
    vector<CirThresGate*>  _thresGates;
};


#endif // CIR_NETWORK_H
#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <vector>
#include "src/common/util.h"

enum GateType {
  BASE_GATE,
  CONST1_GATE,
  PI_GATE,
  PO_GATE,
  THRESHOLD_GATE
};

class CirGate {
  friend class CirNetwork;
  public:
    CirGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : _Id(Id), _name(n), _simVal(0) {}
    virtual ~CirGate() {}
    
    // get
    unsigned Id() const { return _Id; }
    unsigned faninId(unsigned i)  const { return _fanins[i]; }
    unsigned fanoutId(unsigned i) const { return _fanouts[i]; }
    vector<unsigned>& fanins()      { return _fanins; }
    vector<unsigned>& fanouts()     { return _fanouts; }
    virtual GateType  getType()     { return BASE_GATE; }
    virtual string    getTypeStr()  { return ""; }
  
  protected:
    // set
    void setId(unsigned i) { _Id = i; }
    void setName(const string& n) { _name = n; }
    void addFanin(unsigned i) { _fanins.push_back(i); }
    void addFanout(unsigned i) { _fanouts.push_back(i); }
    void setSimVal(int i) { _simVal = i; }
  
  private:
    unsigned _Id;
    string _name;
    vector<unsigned> _fanins;
    vector<unsigned> _fanouts;
    int _simVal;
};

class CirConst1Gate : public CirGate {
  public:
    CirConst1Gate() : CirGate(0, "CONST1") {}
    GateType getType() { return CONST1_GATE; }
    string getTypeStr() { return "CONST1"; }
};

class CirPiGate : public CirGate {
  public:
    CirPiGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : CirGate(Id, n) {}
    GateType getType() { return PI_GATE; }
    string getTypeStr() { return "PI"; }
};

class CirPoGate : public CirGate {
  public:
    CirPoGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : CirGate(Id, n) {}
    GateType getType() { return PO_GATE; }
    string getTypeStr() { return "PO"; }
};

#endif // CIR_GATE_H
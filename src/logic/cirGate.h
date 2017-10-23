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
      : _Id(Id), _name(n), _type(BASE_GATE), _simVal(0) {}
    virtual ~CirGate() {}
    
    // get
    string name() const {return _name; }
    unsigned Id() const { return _Id; }
    unsigned faninId(unsigned i)  const { return _fanins[i]; }
    unsigned fanoutId(unsigned i) const { return _fanouts[i]; }
    unsigned numFanins()          const { return _fanins.size(); }
    unsigned numFanouts()         const { return _fanouts.size(); }
    vector<unsigned>& fanins()      { return _fanins; }
    vector<unsigned>& fanouts()     { return _fanouts; }
    GateType getType()    { return _type; }
    string getTypeStr() {
      switch (_type) {
        case BASE_GATE: return "BASE";
        case CONST1_GATE: return "CONST1";
        case PI_GATE: return "PI";
        case PO_GATE: return "PO";
        case THRESHOLD_GATE: return "THRESHOLD";
        default: assert(false);
      }
    }
    virtual float threshold() { return 0; }
    virtual int computeOutput(const vector<float>& inputs) = 0;
  
  protected:
    // set
    void setId(unsigned i) { _Id = i; }
    void setName(const string& n) { _name = n; }
    void setType(const GateType& t) { _type = t; }
    void addFanin(unsigned i) { _fanins.push_back(i); }
    void addFanout(unsigned i) { _fanouts.push_back(i); }
    void setSimVal(int i) { _simVal = i; }
  
  private:
    unsigned _Id;
    string _name;
    vector<unsigned> _fanins;
    vector<unsigned> _fanouts;
    GateType _type;
    int _simVal;
};

class CirConst1Gate : public CirGate {
  friend class CirNetwork;
  public:
    CirConst1Gate() : CirGate(0, "CONST1") { setType(CONST1_GATE); }
    int computeOutput(const vector<float>& inputs) {return 0;}
};

class CirPiGate : public CirGate {
  friend class CirNetwork;
  public:
    CirPiGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : CirGate(Id, n) { setType(PI_GATE); }
      int computeOutput(const vector<float>& inputs) {return 0;}
};

class CirPoGate : public CirGate {
  friend class CirNetwork;
  public:
    CirPoGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : CirGate(Id, n) { setType(PO_GATE); }
      int computeOutput(const vector<float>& inputs) {return 0;}
};

#endif // CIR_GATE_H
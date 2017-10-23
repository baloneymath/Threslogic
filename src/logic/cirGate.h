#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <vector>
#include "src/common/util.h"

enum GateType {
  BASE_GATE,
  CONST1_GATE,
  PI_GATE,
  PO_GATE,
  THRES_GATE
};

class CirGate {
  friend class CirNetwork;
  public:
    CirGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : _Id(Id), _name(n), _simVal(0) {}
    virtual ~CirGate() {}
    
    // get
    string name() const {return _name; }
    unsigned Id() const { return _Id; }
    CirGate* fanin(unsigned i)  const { return _fanins[i]; }
    CirGate* fanout(unsigned i) const { return _fanouts[i]; }
    unsigned numFanins() const  { return _fanins.size(); }
    unsigned numFanouts() const { return _fanouts.size(); }
    vector<CirGate*>& fanins()  { return _fanins; }
    vector<CirGate*>& fanouts() { return _fanouts; }
    
    virtual bool        isThres()          const { return false; }
    virtual GateType    getType()          const { return BASE_GATE; }
    virtual string      getTypeStr()       const { return "BASE"; }
    virtual float       threshold()        const { return 999999; }
    virtual int         weight(unsigned i) const { ++i; return 0; }
    virtual unsigned    numWeights()       const { return 0; }
    virtual vector<int> weights()          const { return vector<int>(); }
  
  protected:
    // set
    void setId(unsigned i) { _Id = i; }
    void setName(const string& n) { _name = n; }
    void addFanin(CirGate* g) { _fanins.push_back(g); }
    void addFanout(CirGate* g) { _fanouts.push_back(g); }
    void setSimVal(int i) { _simVal = i; }
  
  private:
    unsigned _Id;
    string _name;
    vector<CirGate*> _fanins;
    vector<CirGate*> _fanouts;
    int _simVal;
};

class CirConst1Gate : public CirGate {
  friend class CirNetwork;
  public:
    CirConst1Gate() : CirGate(0, "CONST1") {}
  bool isThres() const { return false; }
  GateType getType() const { return CONST1_GATE; }
  string getTypeStr() const { return "CONST1"; }
};

class CirPiGate : public CirGate {
  friend class CirNetwork;
  public:
    CirPiGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : CirGate(Id, n) {}
    bool isThres() const { return false; }
    GateType getType() const { return PI_GATE; }
    string getTypeStr() const { return "PI"; }
};

class CirPoGate : public CirGate {
  friend class CirNetwork;
  public:
    CirPoGate(unsigned Id = MAX_UNSIGNED, const string& n = "")
      : CirGate(Id, n) {}
    bool isThres() const { return false; }
    GateType getType() const { return PO_GATE; }
    string getTypeStr() const { return "PO"; }
};

class CirThresGate : public CirGate {
  friend class CirNetwork;
  public:
    CirThresGate(unsigned Id = 0, const string& n = "", float val = 0)
      : CirGate(Id, n), _threshold(val) {}
    bool isThres() const { return true; }
    GateType getType() const { return THRES_GATE; }
    string getTypeStr() const { return "THRES"; }
  
    float threshold() const { return _threshold; }
    int weight(unsigned i) const { return _weights[i]; }
    unsigned numWeights() const { return _weights.size(); }
    vector<int> weights() const { return _weights; }

  protected:
    void addWeight(int i) { _weights.push_back(i); }
    void setThreshold(float val) { _threshold = val; }
  
  private:
    float _threshold;
    vector<int> _weights;
};

#endif // CIR_GATE_H
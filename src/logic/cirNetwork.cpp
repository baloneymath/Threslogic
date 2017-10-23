#include <map>
using namespace std;
#include "cirNetwork.h"

void CirNetwork::parse(const string& filename) {
  char buf[100000];
  vector<string> tokens;

  FILE* fin = fopen(filename.c_str(), "r");
  unsigned gate_cnt = 1; // const1 initial
  unsigned piGate_cnt = 0;
  unsigned poGate_cnt = 0;
  unsigned thresGate_cnt = 0;
  map<string, unsigned> name2gateId;
  while (fgets(buf, sizeof(buf), fin)) {
    if (buf[0] == '#') continue;
    buf[strcspn(buf, "\n\r")] = 0;
    util::splitString(buf, ' ', tokens);
    if (tokens.size() > 0) {
      if (tokens[0] == ".model") _name = tokens[1];
      else if (tokens[0] == ".inputs") {
        for (unsigned i = 1; i < tokens.size(); ++i)
          name2gateId[tokens[i]] = gate_cnt++;
        piGate_cnt = tokens.size() - 1;
      }
      else if (tokens[0] == ".outputs") {
        for (unsigned i = 1; i < tokens.size(); ++i)
          name2gateId[tokens[i]] = gate_cnt++;
        poGate_cnt = tokens.size() - 1;
      }
      else if (tokens[0] == ".thres") {
        name2gateId[tokens.back()] = gate_cnt++;
        ++thresGate_cnt;
        fgets(buf, sizeof(buf), fin);
      }
      else if (tokens[0] == ".po") continue;
      else {
        fprintf(stderr, "!!! Detect undefined format in file !!!\n\n");
        assert(false);
      }
    }
  }
  fprintf(stderr, "Total Gates: %u + 1 (Const Gate)\n", gate_cnt - 1);
  fprintf(stderr, "  -Pi    : %u\n", piGate_cnt);
  fprintf(stderr, "  -Po    : %u\n", poGate_cnt);
  fprintf(stderr, "  -Thres : %u\n", thresGate_cnt);
  fseek(fin, 0, SEEK_SET);
  fprintf(stderr, "Connecting Network....\n");
  _gates.resize(gate_cnt);
  _piGates.reserve(piGate_cnt);
  _poGates.reserve(poGate_cnt);
  _thresgates.reserve(thresGate_cnt);
  _gates[0] = CirConst1Gate();
  unsigned cnt = 1;
  while (fgets(buf, sizeof(buf), fin)) {
    if (buf[0] == '#') continue;
    buf[strcspn(buf, "\n\r")] = 0;
    util::splitString(buf, ' ', tokens);
    if (tokens.size() > 0) {
      if (tokens[0] == ".inputs") {
        for (unsigned i = 1; i < tokens.size(); ++i) {
          CirPiGate g(cnt, tokens[i]);
          _gates[cnt++] = g;
          _piGates.push_back(g);
        }
      }
      else if (tokens[0] == ".outputs") {
        for (unsigned i = 1; i < tokens.size(); ++i) {
          CirPoGate g(cnt, tokens[i]);
          _gates[cnt++] = g;
          _poGates.push_back(g);
        }
      }
      else if (tokens[0] == ".thres") {
        CirThresGate g(cnt, tokens.back());
        for (unsigned i = 1; i < tokens.size() - 1; ++i) {
          unsigned faninId = name2gateId[tokens[i]];
          g.addFanin(faninId);
          _gates[faninId].addFanout(g.Id());
        }
        fgets(buf, sizeof(buf), fin);
        buf[strcspn(buf, "\n\r")] = 0;
        util::splitString(buf, ' ', tokens);
        for (unsigned i = 0; i < tokens.size() - 1; ++i)
          g.addWeight(stoi(tokens[i]));
        g.setThreshold(stof(tokens.back()));
        _gates[cnt++] = g;
        _thresgates.push_back(g);
      }
      else if (tokens[0] == ".po") {
        CirGate& g = _gates[name2gateId[tokens.back()]];
        unsigned faninId = name2gateId[tokens[1]];
        g.addFanin(faninId);
        _gates[faninId].addFanout(g.Id());
      }
    }
  }
  CirGate& g = _gates[4500];
  vector<float> v(784, 1);
  cerr << g.computeOutput(v) << endl;
  //for (unsigned i = 0; i < g.numFanins(); ++i) {
  //  cerr << _gates[g.faninId(i)].name() << ' ';
  //}
  //cerr << endl;
  fclose(fin);
}
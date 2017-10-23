using namespace std;
#include "cirNetwork.h"

void CirNetwork::parse(const string& filename) {
  char buf[100000];
  vector<string> tokens;

  FILE* fin = fopen(filename.c_str(), "r");
  unsigned gate_cnt = 0;
  unsigned piGate_cnt = 0;
  unsigned poGate_cnt = 0;
  unsigned thresGate_cnt = 0;
  while (fgets(buf, sizeof(buf), fin)) {
    if (buf[0] == '#') continue;
    buf[strcspn(buf, "\n\r")] = 0;
    util::splitString(buf, " ", tokens);
    if (tokens.size() > 0) {
      if (tokens[0] == ".model") _name = tokens[1];
      else if (tokens[0] == ".inputs") piGate_cnt = tokens.size() - 1;
      else if (tokens[0] == ".outputs") poGate_cnt = tokens.size() - 1;
      else if (tokens[0] == ".thres") {
        ++thresGate_cnt;
        fgets(buf, sizeof(buf), fin);
      }
      else {
        fprintf(stderr, "!!! Detect undefined format in file !!!\n\n");
        assert(false);
      }
    }
  }
  gate_cnt = piGate_cnt + poGate_cnt + thresGate_cnt;
  fprintf(stderr, "Total Gates: %u\n", gate_cnt);
  fprintf(stderr, "  -Pi    : %u\n", piGate_cnt);
  fprintf(stderr, "  -Po    : %u\n", poGate_cnt);
  fprintf(stderr, "  -Thres : %u\n", thresGate_cnt);
  fseek(fin, 0, SEEK_SET);
  _gates.resize(gate_cnt);
  _piGates.reserve(piGate_cnt);
  _poGates.reserve(poGate_cnt);
  _thresgates.reserve(thresGate_cnt);
  unsigned cnt = 1;
  while (fgets(buf, sizeof(buf), fin)) {
    if (buf[0] == '#') continue;
    buf[strcspn(buf, "\n\r")] = 0;
    util::splitString(buf, " ", tokens);
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
        fgets(buf, sizeof(buf), fin);
      }
    }
  }
  fclose(fin);
}
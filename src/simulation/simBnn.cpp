using namespace std;
#include "simBnn.h"

void SimBnn::readMNIST_test(const string& filename) {
  if (filename == "") return;
  char buf[100000];
  vector<string> tokens;

  FILE* fin = fopen(filename.c_str(), "r");
  while (fgets(buf, sizeof(buf), fin)) {
    util::splitString(buf, ' ', tokens);
    vector<float> x;
    for (unsigned i = 0; i < tokens.size() - 1; ++i)
      x.push_back(2 * stof(tokens[i]) - 1);
    _test_X.push_back(x);
    _test_y.push_back(stoi(tokens.back()));
  }
  fclose(fin);
}

void SimBnn::simulateMNIST() {
  auto getOutput1 = [&] (CirThresGate* g, const vector<float> v) -> int {
    float accum = 0;
    for (unsigned i = 0; i < g->numFanins(); ++i) {
      accum += v[i] * g->weight(i);
    }
    return accum >= g->threshold() ? 1 : -1;
  };
  auto getOutput2 = [&] (CirThresGate* g, const vector<int> v) -> int {
    int accum = 0;
    for (unsigned i = 0; i < g->numFanins(); ++i) {
      accum += v[i] * g->weight(i);
    }
    return accum >= g->threshold() ? 1 : -1;
  };
  if (_test_X.size() == 0) return;
  int err = 0;
  #pragma omp parallel for
  for (unsigned i = 0; i < _test_X.size(); ++i) {
    vector<int> outs, inputs;
    // First layer
    for (unsigned j = 0; j < 4096; ++j) {
      CirThresGate* g = _ntk.thresGate(j);
      outs.push_back(getOutput1(g, _test_X[i]));
    }
    // Second Layer
    inputs = outs; outs.clear();
    for (unsigned j = 4096; j < 8192; ++j) {
      CirThresGate* g = _ntk.thresGate(j);
      outs.push_back(getOutput2(g, inputs));
    }
    // Third Layer
    inputs = outs; outs.clear();
    for (unsigned j = 8192; j < 12288; ++j) {
      CirThresGate* g = _ntk.thresGate(j);
      outs.push_back(getOutput2(g, inputs));
    }
    // Output Layer
    inputs = outs; outs.clear();
    for (unsigned j = 12288; j < 12298; ++j) {
      CirThresGate* g = _ntk.thresGate(j);
      outs.push_back(getOutput2(g, inputs));
    }
    int idx = -1;
    for (unsigned k = 0; k < outs.size(); ++k) {
      if (outs[k] == 1) {
        idx = k;
        break;
      }
    }
    if (idx != _test_y[i]) ++err;
    fprintf(stderr, "Input: %u,\tResult: %d, Ans: %d, err: %d\n", i + 1, idx, _test_y[i], err);
  }
  fprintf(stderr, "Test error rate: %f%%\n", 100 * (double)err / _test_y.size());
  fprintf(stderr, "Transform error rate: %f%%\n", 100 * (double)(err - 93) / _test_y.size());
}
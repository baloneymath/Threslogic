#ifndef PARAMHANDLER_H
#define PARAMHANDLER_H

#include <string>

struct ParamHandler {
  ParamHandler()
    : inputFileName(""), outputFileName("") {}
  ~ParamHandler() {}

  string inputFileName;
  string outputFileName;
};

#endif
#ifndef ARGHANDLER_H
#define ARGHANDLER_H

#include "paramhandler.h"

struct ArgHandler {
  ArgHandler(int argc, char** argv, ParamHandler& gParam);
  ~ArgHandler() {}
};

#endif
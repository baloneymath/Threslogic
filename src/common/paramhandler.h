#ifndef PARAMHANDLER_H
#define PARAMHANDLER_H

#include <string>

struct ParamHandler {
  ParamHandler()
    : inputFileName(""), outputFileName(""),
      MNIST_train(""), MNIST_test("") {}
  ~ParamHandler() {}

  string inputFileName;
  string outputFileName;
  string MNIST_train, MNIST_test;
};

#endif // PARAMHANDLER_H
using namespace std;
#include "util.h"
#include "arghandler.h"

extern ParamHandler gParam;

inline void dumpUsage() {
  fprintf(stderr, "Usage: ./thres_opt -in <input> [-out <output>]\n\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -h, --help: show this help messege\n");
  fprintf(stderr, "  -in: input file\n");
  fprintf(stderr, "  -out: output result file\n");
  exit(0);
}
ArgHandler::ArgHandler(int argc, char **argv, ParamHandler& gParam) {
  int i = 0;
  if (argc < 2) dumpUsage();
  while(i < argc) {
    if (strlen(argv[i]) <= 1) {
      ++i;
      continue;
    }
    else if (strcmp(argv[i] + 1, "h") == 0) dumpUsage();
    else if (strcmp(argv[i] + 2, "help") == 0) dumpUsage();
    else if (strcmp(argv[i] + 1, "in") == 0) {
      if (++i >= argc) dumpUsage();
      gParam.inputFileName = argv[i];
    }
    else if (strcmp(argv[i] + 1, "out") == 0) {
      if (++i >= argc) dumpUsage();
      gParam.outputFileName = argv[i];
    }
    ++i;
  }
  if (!util::existFile(gParam.inputFileName)) {
    fprintf(stderr, "Error: No file name \"%s\"\n",
            util::getFileName(gParam.inputFileName).c_str());
    dumpUsage();
  }
}
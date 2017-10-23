#ifndef UTIL_H
#define UTIL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <vector>

#define LL long long
constexpr unsigned MAX_UNSIGNED =  numeric_limits<unsigned>::max();
constexpr int      MAX_INT      =  numeric_limits<int>::max();
constexpr int      MIN_INT      =  numeric_limits<int>::lowest();
constexpr size_t   MAX_SIZE_T   =  numeric_limits<size_t>::max();
constexpr double   MAX_DOUBLE   =  numeric_limits<double>::max();
constexpr double   MIN_DOUBLE   =  numeric_limits<double>::lowest();
constexpr LL       MAX_LL       =  numeric_limits<LL>::max();
constexpr LL       MIN_LL       =  numeric_limits<LL>::lowest();
constexpr double   EPSILON      =  1e-8;

namespace util {

// show sys info
inline void showSysInfo() {
#ifdef __linux__
  int systemret = 0;
  fprintf(stderr, "\n");
  fprintf(stderr, "=================== SYSTEM INFORMATION ==================\n");
  systemret += system(">&2 echo 'User:       '`whoami`@`hostname`");
  systemret += system(">&2 echo 'Date:       '`date`");
  systemret += system(">&2 echo 'System:     '`uname -a`");
  fprintf(stderr, "=========================================================\n");
  assert(!systemret);
#endif
}
// filename
inline bool existFile (const string& filename) {
  struct stat buffer;   
  return stat(filename.c_str(), &buffer) == 0; 
}
inline string getFileDirName(const string& filePathName) {
  string retStr = filePathName;
  string::size_type pos = retStr.rfind("/");
  if (pos != string::npos)
    retStr = retStr.substr(0, pos);
  return retStr;
}
inline string getFileName(const string& filePathName) {
  string retStr = filePathName;
  string::size_type pos = retStr.rfind("/");
  if (pos != string::npos)
    retStr = retStr.substr(pos + 1);
  return retStr;
}
// cpu memory usage
constexpr double TIME_SCALE   = 1000000.0;
constexpr double MEMORY_SCALE = 1024.0;
class TimeUsage {
  public:
    TimeUsage() { start(FULL); start(PARTIAL); }
    struct TimeState {
      TimeState(double r = 0, double u = 0, double s = 0)
        : rTime(r), uTime(u), sTime(s) {}
      double rTime, uTime, sTime; //real, user, system
    };
    enum TimeType { FULL, PARTIAL };
    void start(TimeType type) { 
      (type == FULL) ? checkUsage(_tStart) : checkUsage(_pStart); 
    }
    void showUsage(const char* comment, TimeType type) {
      TimeState curSt;
      checkUsage(curSt);
      TimeState dur = (type == FULL) ? diff(_tStart, curSt) : diff(_pStart, curSt);
      if (type == FULL)
        fprintf(stderr, "---------- %s total time usage -----------\n", comment);
      else
        fprintf(stderr, "---------- %s period time usage -----------\n", comment);
      fprintf(stderr, "Real: %fs; User: %fs; System: %fs\n\n", dur.rTime, dur.uTime, dur.sTime);
    }
  private:
    TimeState diff(TimeState &start, TimeState &end) {
      return TimeState(end.rTime - start.rTime, 
                       end.uTime - start.uTime,
                       end.sTime - start.sTime);
    }
    void checkUsage(TimeState &st) const {
      rusage tUsg;
      getrusage(RUSAGE_SELF, &tUsg);
      timeval tReal;
      gettimeofday(&tReal, NULL);
      st.uTime = tUsg.ru_utime.tv_sec + tUsg.ru_utime.tv_usec / TIME_SCALE;
      st.sTime = tUsg.ru_stime.tv_sec + tUsg.ru_stime.tv_usec / TIME_SCALE;
      st.rTime = tReal.tv_sec + tReal.tv_usec / TIME_SCALE;
    }
    TimeState _tStart, _pStart; //total, period
};
// memory
inline double getPeakMemoryUsage() {
#ifdef __linux__
  FILE* fmem = fopen("/proc/self/status", "r");
  char membuf[128];
  double vmPeak = 0;
  while (fgets(membuf, 128, fmem)) {
    char* ch;
    if ((ch = strstr(membuf, "VmPeak:"))) {
      vmPeak = atol(ch + 7);
      continue;
    }
  }
  fclose(fmem);
  return vmPeak;
#else
  return -1;
#endif
}
inline double getCurrMemoryUsage() {
#ifdef __linux__
  FILE* fmem = fopen("/proc/self/status", "r");
  char membuf[128];
  double vmSize = 0;
  while (fgets(membuf, 128, fmem)) {
    char* ch;
    if ((ch = strstr(membuf, "VmSize:"))) {
      vmSize = atol(ch + 7);
      break;
    }
  }
  fclose(fmem);
  return vmSize;
#else
  return -1;
#endif
}
inline void showMemoryUsage(bool flag) {
  if (flag) fprintf(stderr, "Peak Memory Usage: %f MB\n", getPeakMemoryUsage() / MEMORY_SCALE);
  else      fprintf(stderr, "Curr Memory Usage: %f MB\n", getCurrMemoryUsage() / MEMORY_SCALE);
}

// string
inline void splitString(const char* str, const string& delims, vector<string>& tokens) {
  tokens.clear();
  string s(str), token;
  size_t cur, prev = 0;
  cur = s.find_first_of(delims);
  while (cur != std::string::npos) {
    token = s.substr(prev, cur - prev);
    if (token != "") tokens.push_back(token);
    prev = cur + 1;
    cur = s.find_first_of(delims, prev);
  }
  token = s.substr(prev, cur - prev);
  if (token != "") tokens.push_back(token);
}

inline void splitString(const string& s, const string& delims, vector<string>& tokens) {
  tokens.clear();
  string token;
  size_t cur, prev = 0;
  cur = s.find_first_of(delims);
  while (cur != std::string::npos) {
    token = s.substr(prev, cur - prev);
    if (token != "") tokens.push_back(token);
    prev = cur + 1;
    cur = s.find_first_of(delims, prev);
  }
  token = s.substr(prev, cur - prev);
  if (token != "") tokens.push_back(token);
}
}

#endif // UTIL_H
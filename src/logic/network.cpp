using namespace std;
#include "network.h"

inline void split(const char* str, const char c, vector<string>& tokens) {
  tokens.clear();
  do {
    const char* begin = str;
    while (*str != c && *str) ++str;
    tokens.push_back(string(begin, str));
  } while (*str++);
}

void Network::parse(const string& filename) {
	_name = filename;
	
}
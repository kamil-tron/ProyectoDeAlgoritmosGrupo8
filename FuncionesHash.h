#pragma once

#include <string>
using namespace std;

inline int hashInt(const int& k, int m) {
    return k % m;
}

inline int hashString(const string& s, int m) {
    unsigned long h = 0;
    for (char c : s) h = 31 * h + c;
    return static_cast<int>(h % m);
}

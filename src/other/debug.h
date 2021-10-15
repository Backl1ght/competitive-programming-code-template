#include <bits/stdc++.h>
using namespace std;

template <typename T>
inline void _dbg(const char* format, T value) {
  cerr << format << '=' << value << endl;
}

template <typename First, typename... Rest>
inline void _dbg(const char* format, First f, Rest... r) {
  while (*format != ',')
    cerr << *format++;
  cerr << '=' << f << ", ";
  _dbg(format + 1, r...);
}

template <typename T>
ostream& operator<<(ostream& os, vector<T> V) {
  os << "[ ";
  for (auto v : V)
    os << v << ",";
  return os << " ]";
}

template <typename T>
ostream& operator<<(ostream& os, set<T> V) {
  os << "[ ";
  for (auto v : V)
    os << v << ",";
  return os << " ]";
}

template <typename T>
ostream& operator<<(ostream& os, multiset<T> V) {
  os << "[ ";
  for (auto v : V)
    os << v << ",";
  return os << " ]";
}

template <typename T1, typename T2>
ostream& operator<<(ostream& os, map<T1, T2> V) {
  os << "[ ";
  for (auto v : V)
    os << v << ",";
  return os << " ]";
}

template <typename L, typename R>
ostream& operator<<(ostream& os, pair<L, R> P) {
  return os << "(" << P.first << "," << P.second << ")";
}

#define debug(...)                 \
  cerr << "\033[31m";              \
  _dbg(#__VA_ARGS__, __VA_ARGS__); \
  cerr << "\033[0m";
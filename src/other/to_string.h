#include <bits/stdc++.h>

template <typename T,
          typename = typename std::
              enable_if<std::is_integral<T>::value or std::is_floating_point<T>::value, void>::type>
std::string to_string(T a) {
  return std::to_string(a);
}

std::string to_string(const std::string& s) {
  return '"' + s + '"';
}

template <typename A, typename B>
std::string to_string(std::pair<A, B> p) {
  return "(" + to_string(p.first) + "," + to_string(p.second) + ")";
}

template <typename A, typename B, typename C>
std::string to_string(std::tuple<A, B, C> p) {
  return "(" + to_string(std::get<0>(p)) + ", " + to_string(std::get<1>(p)) + ", " +
         to_string(std::get<2>(p)) + ")";
}

template <typename A, typename B, typename C, typename D>
std::string to_string(std::tuple<A, B, C, D> p) {
  return "(" + to_string(std::get<0>(p)) + ", " + to_string(std::get<1>(p)) + ", " +
         to_string(std::get<2>(p)) + to_string(std::get<3>(p)) + ")";
}

template <typename A, size_t size>
std::string to_string(std::array<A, size> arr) {
  std::string res = "[";
  for (const A& a : arr)
    res += std::to_string(a) + ",";
  res += "]";
  return res;
}

template <size_t N>
std::string to_string(std::bitset<N> v) {
  std::string res = "";
  for (size_t i = 0; i < N; i++)
    res += static_cast<char>('0' + v[i]);
  return res;
}

template <typename T>
std::string to_string(std::vector<T> V) {
  std::string res = "[";
  for (const T& v : V)
    res += to_string(v) + ",";
  res.pop_back();
  res += "]";
  return res;
}

template <typename T>
std::string to_string(std::vector<std::vector<std::vector<T>>> V) {
  std::string res = "[\n";
  for (const std::vector<std::vector<T>>& v : V)
    res += "  " + to_string(v) + "\n";
  res += "]";
  return res;
}

template <typename T>
std::string to_string(std::set<T> S) {
  std::string res = "{";
  for (const T& s : S)
    res += to_string(s) + ",";
  res += "}";
  return res;
}

template <typename T, typename Comp>
std::string to_string(std::multiset<T, Comp> S) {
  std::string res = "{";
  for (const T& s : S)
    res += to_string(s) + ",";
  res += "}";
  return res;
}

template <typename A, typename B>
std::string to_string(std::map<A, B> M) {
  std::string res = "{";
  for (const std::pair<A, B>& m : M)
    res += to_string(m) + ",";
  res += "}";
  return res;
}

template <typename A, typename B>
std::string to_string(std::multimap<A, B> M) {
  std::string res = "{";
  for (const std::pair<A, B>& m : M)
    res += to_string(m) + ",";
  res += "}";
  return res;
}
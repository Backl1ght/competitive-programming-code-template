#include <bits/stdc++.h>

template <typename T>
std::string serialize(T t);

namespace my {

std::string to_string(const std::string& s) {
  return '"' + s + '"';
}

std::string to_string(__int128_t x) {
  if (x == 0)
    return "0";

  std::string s;
  bool is_negative = false;
  if (x < 0) {
    is_negative = true;
    x = -x;
  }
  while (x) {
    s.push_back('0' + x % 10);
    x /= 10;
  }
  std::reverse(s.begin(), s.end());
  if (is_negative)
    s = "-" + s;

  return s;
}

template <typename A, typename B>
std::string to_string(const std::pair<A, B>& p) {
  return "(" + serialize(p.first) + "," + serialize(p.second) + ")";
}

template <typename A, typename B, typename C>
std::string to_string(const std::tuple<A, B, C>& p) {
  return "(" + serialize(std::get<0>(p)) + ", " + serialize(std::get<1>(p)) + ", " +
         serialize(std::get<2>(p)) + ")";
}

template <typename A, typename B, typename C, typename D>
std::string to_string(const std::tuple<A, B, C, D>& p) {
  return "(" + serialize(std::get<0>(p)) + ", " + serialize(std::get<1>(p)) + ", " +
         serialize(std::get<2>(p)) + serialize(std::get<3>(p)) + ")";
}

template <typename A, size_t size>
std::string to_string(const std::array<A, size>& arr) {
  std::string res = "[";
  for (const A& a : arr)
    res += serialize(a) + ",";
  res += "]";
  return res;
}

template <size_t N>
std::string to_string(const std::bitset<N>& v) {
  std::string res = "";
  for (size_t i = 0; i < N; i++)
    res += static_cast<char>('0' + v[i]);
  return res;
}

template <typename T>
std::string to_string(const std::vector<T>& V) {
  std::string res = "[";
  for (const T& v : V)
    res += serialize(v) + ",";
  res += "]";
  return res;
}

template <typename T>
std::string to_string(const std::vector<std::vector<T>>& V) {
  std::string res = "\n[\n";
  for (const std::vector<T>& v : V)
    res += "  " + to_string(v) + "\n";
  res += "]";
  return res;
}

template <typename T>
std::string to_string(const std::vector<std::vector<std::vector<T>>>& V) {
  std::string res = "\n[\n";
  for (const std::vector<std::vector<T>>& v : V)
    res += "  " + to_string(v) + "\n";
  res += "]";
  return res;
}

template <typename T>
std::string to_string(const std::set<T>& S) {
  std::string res = "{";
  for (const T& s : S)
    res += serialize(s) + ",";
  res += "}";
  return res;
}

template <typename T, typename Comp>
std::string to_string(const std::multiset<T, Comp>& S) {
  std::string res = "{";
  for (const T& s : S)
    res += serialize(s) + ",";
  res += "}";
  return res;
}

template <typename A, typename B>
std::string to_string(const std::map<A, B>& M) {
  std::string res = "{";
  for (const std::pair<const A, B>& m : M)
    res += to_string(m) + ",";
  res += "}";
  return res;
}

template <typename A, typename B>
std::string to_string(const std::multimap<A, B>& M) {
  std::string res = "{";
  for (const std::pair<const A, B>& m : M)
    res += to_string(m) + ",";
  res += "}";
  return res;
}
}  // namespace my

template <typename T, typename = std::string>
struct has_std_to_string : std::false_type {};

template <typename T>
struct has_std_to_string<T, decltype(std::to_string(std::declval<T>()))> : std::true_type {};

template <typename T, typename = std::string>
struct has_my_to_string : std::false_type {};

template <typename T>
struct has_my_to_string<T, decltype(my::to_string(std::declval<T>()))> : std::true_type {};

template <typename T, typename = std::string>
struct has_self_to_string : std::false_type {};

template <typename T>
struct has_self_to_string<T, decltype(std::declval<T>().to_string())> : std::true_type {};

template <typename T>
std::string serialize(T t) {
  if constexpr (has_my_to_string<T>::value) {
    return my::to_string(t);
  }
  if constexpr (has_self_to_string<T>::value) {
    return t.to_string();
  }
  if constexpr (has_std_to_string<T>::value) {
    return std::to_string(t);
  }
  return "no matched to_string";
}

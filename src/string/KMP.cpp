namespace String {

template <typename String>
std::vector<int> KMP(const String& s) {
  int n = s.size();
  std::vector<int> p(n);
  p[0] = 0;
  for (int i = 1, j = 0; i < n; ++i) {
    while (j > 0 && s[i] != s[j])
      j = p[j - 1];
    if (s[i] == s[j])
      ++j;
    p[i] = j;
  }
  return p;
}

template <typename String>
std::vector<int> Match(const String& s, const String& t) {
  int n = s.size();
  auto p = KMP<String>(s);

  int m = t.size();
  std::vector<int> r;
  for (int i = 0, j = 0; i < m; ++i) {
    while (j == n || (j > 0 && t[i] != s[j]))
      j = p[j - 1];
    if (s[j] == t[i])
      ++j;

    if (j == n) {
      r.push_back(i);
    }
  }

  return r;
}

// TODO(backlight): period related.

}  // namespace String
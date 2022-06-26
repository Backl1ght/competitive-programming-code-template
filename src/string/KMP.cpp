std::vector<int> kmp(const std::string& s) {
  int n = s.size();
  std::vector<int> p(n);

  p[0] = 0;
  for (int i = 1; i < n; ++i) {
    int j = p[i - 1];
    while (j > 0 && s[j] != s[i])
      j = p[j - 1];
    if (s[i] == s[j])
      ++j;
    p[i] = j;
  }

  return p;
}
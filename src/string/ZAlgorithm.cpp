namespace ZAlgorithm {
// 1-based

// z_i = LCP(s, s[i..n])
void getZ(char* s, int n, int* z) {
  z[1] = n;
  for (int i = 2, l = 0, r = 0; i <= n; ++i) {
    if (i <= r)
      z[i] = min(r - i + 1, z[i - l + 1]);
    else
      z[i] = 0;
    while (i + z[i] <= n && s[z[i] + 1] == s[i + z[i]])
      ++z[i];
    if (i + z[i] - 1 > r)
      l = i, r = i + z[i] - 1;
  }
}

// p_i = LCP(s, t[i...m])
void EXKMP(char* s, int n, int* z, char* t, int m, int* p) {
  getZ(s, n, z);
  for (int i = 1, l = 0, r = 0; i <= m; ++i) {
    if (i <= r)
      p[i] = min(r - i + 1, z[i - l + 1]);
    else
      p[i] = 0;
    while (i + p[i] <= m && s[p[i] + 1] == t[i + p[i]])
      ++p[i];
    if (i + p[i] - 1 > r)
      l = i, r = i + p[i] - 1;
  }
}
}  // namespace ZAlgorithm
namespace KMP {
// pi_i = s[0...i]最长border
void getPi(char* s, int n, int* pi) {
  pi[0] = 0;
  for (int i = 1; i < n; ++i) {
    int j = pi[i - 1];
    while (j > 0 && s[j] != s[i])
      j = pi[j - 1];
    if (s[i] == s[j])
      ++j;
    pi[i] = j;
  }
}

vector<int> getAllMatchPosition(char* s, int n, int* pi, char* t, int m) {
  s[n] = '#';
  s[n + 1] = 0;
  ++n;
  KMP::getPi(s, n, pi);

  vector<int> ans;

  int p = 0;
  for (int i = 0; i < m; ++i) {
    while (p > 0 && t[i] != s[p])
      p = pi[p - 1];
    if (t[i] == s[p]) {
      ++p;
      if (p == n - 1) {
        ans.push_back(i + 2 - n);
      }
    }
  }

  return ans;
}

int getPeriod(int n, int* pi) {
  return n - pi[n - 1];
}
}  // namespace KMP
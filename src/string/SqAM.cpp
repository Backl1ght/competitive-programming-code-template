/**
 * 识别一个串的子序列, O(n^2)
 * 用法类似后缀自动机
*/
struct SqAM {
  int next[N << 1][26], pre[N << 1], lst[26];
  int root, tot;
  void init() {
    root = tot = 1;
    for (int i = 0; i < 26; i++)
      lst[i] = 1;
  }

  void extend(int c) {
    int p = lst[c], np = ++tot;
    pre[np] = p;
    for (int i = 0; i < 26; i++)
      for (int j = lst[i]; j && !next[j][c]; j = pre[j])
        next[j][c] = np;
    lst[c] = np;
  }
};
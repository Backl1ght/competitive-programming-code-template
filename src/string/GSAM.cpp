namespace GSAM {
using T = char;

inline int idx(T c) {
  return c - 'a';
}

const int __N = N << 1;
const int __M = 26;

int tot, next[__N][__M];
int len[__N], fail[__N];

inline void init() {
  tot = 0;
  fail[0] = -1;
  len[0] = 0;
  memset(next[0], 0, sizeof(next[0]));
}

inline int newnode() {
  ++tot;
  fail[tot] = 0;
  len[tot] = 0;
  memset(next[tot], 0, sizeof(next[tot]));
  return tot;
}

void insertTrie(const T* s, int n) {
  int p = 0, c;
  for (int i = 0; i < n; ++i) {
    c = idx(s[i]);
    if (!next[p][c])
      next[p][c] = newnode();
    p = next[p][c];
  }
}

int extendSAM(int last, int c) {
  int cur = next[last][c];
  if (len[cur])
    return cur;
  len[cur] = len[last] + 1;

  int p = fail[last];
  while (p != -1) {
    if (!next[p][c])
      next[p][c] = cur;
    else
      break;
    p = fail[p];
  }

  if (p == -1) {
    fail[cur] = 0;
    return cur;
  }

  int q = next[p][c];
  if (len[p] + 1 == len[q]) {
    fail[cur] = q;
    return cur;
  }

  int clone = newnode();
  for (int i = 0; i < __M; ++i)
    next[clone][i] = len[next[q][i]] ? next[q][i] : 0;

  len[clone] = len[p] + 1;
  while (p != -1 && next[p][c] == q) {
    next[p][c] = clone;
    p = fail[p];
  }
  fail[clone] = fail[q];
  fail[cur] = clone;
  fail[q] = clone;
  return cur;
}

void build() {
  queue<pair<int, int>> q;
  for (int i = 0; i < __M; ++i)
    if (next[0][i])
      q.push(make_pair(0, i));

  while (!q.empty()) {
    pair<int, int> u = q.front();
    q.pop();
    int last = extendSAM(u.first, u.second);
    for (int i = 0; i < __M; ++i)
      if (next[last][i])
        q.push(make_pair(last, i));
  }
}

// 多模式串--本质不同子串数
ll count() {
  ll res = 0;
  for (int i = 1; i <= tot; ++i)
    res += len[i] - len[fail[i]];
  return res;
}
}  // namespace GSAM
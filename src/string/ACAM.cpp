namespace ACAM {
const int __N = 3e5 + 5;
const int __M = 26;
int tot, tr[__N][__M], fail[__N], last[__N];
int f[__N], e[__N];

int eid[__N];
multiset<int> st[__N];

inline int idx(const char& c) {
  return c - 'a';
}

inline void init() {
  tot = 0;
  memset(tr[0], 0, sizeof(tr[0]));
  f[0] = e[0] = 0;
}

inline int newnode() {
  ++tot;
  memset(tr[tot], 0, sizeof(tr[tot]));
  f[tot] = e[tot] = 0;
  return tot;
}

void insert(char* s, int n, int id) {
  int p = 0, c;
  for (int i = 0; i < n; ++i) {
    c = idx(s[i]);
    if (!tr[p][c])
      tr[p][c] = newnode();
    p = tr[p][c];
    ++f[p];
  }
  ++e[p];

  eid[id] = p;
  st[p].insert(0);
}

// 字典图优化
// void getfail() {
//     queue<int> q;
//     for (int i = 0; i < __M; ++i) if (tr[0][i]) fail[tr[0][i]] = 0, q.push(tr[0][i]);
//     while(!q.empty()) {
//         int p = q.front(); q.pop();
//         for (int c = 0; c < __M; ++c) {
//             int nxt = tr[p][c];
//             if (nxt) fail[nxt] = tr[fail[p]][c], q.push(nxt);
//             else nxt = tr[fail[p]][c];
//         }
//     }
// }

// int query(char* t) {
//     int n = strlen(t), p = 0, res = 0;
//     for (int i = 0; i < n; ++i) {
//         p = tr[p][t[i] - 'a'];
//         for (int j = p; j && e[j] != -1; j = fail[j]) res += e[j], e[j] = -1;
//     }
//     return res;
// }

// 跳fail链
void getfail() {
  queue<int> q;
  fail[0] = 0;
  for (int c = 0; c < __M; ++c)
    if (tr[0][c])
      fail[tr[0][c]] = last[tr[0][c]] = 0, q.push(tr[0][c]);
  while (!q.empty()) {
    int p = q.front();
    q.pop();
    for (int c = 0; c < __M; ++c) {
      int u = tr[p][c];
      if (u) {
        q.push(u);
        int v = fail[p];
        while (v && !tr[v][c])
          v = fail[v];
        fail[u] = tr[v][c];
        last[u] = e[fail[u]] ? fail[u] : last[fail[u]];
      }
    }
  }
}

int queryMax(char* t, int n) {
  int p = 0, res = -1, c;
  for (int i = 0; i < n; ++i) {
    c = idx(t[i]);
    while (p && !tr[p][c])
      p = fail[p];
    p = tr[p][c];
    for (int j = p; j; j = last[j])
      if (e[j])
        updMax(res, (*st[j].rbegin()));
  }
  return res;
}
}  // namespace ACAM
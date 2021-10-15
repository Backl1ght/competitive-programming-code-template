// https://loj.ac/p/6021
// https://oi-wiki.org/graph/mst/#kruskal_1
#include <bits/stdc++.h>
using namespace std;
#ifdef BACKLIGHT
#include "debug.h"
#else
#define debug(...)
#endif

const int __BUFFER_SIZE__ = 1 << 20;
bool NEOF = 1;
int __top;
char __buf[__BUFFER_SIZE__], *__p1 = __buf, *__p2 = __buf, __stk[996];
inline char nc() {
  if (!NEOF)
    return EOF;
  if (__p1 == __p2) {
    __p1 = __buf;
    __p2 = __buf + fread(__buf, 1, __BUFFER_SIZE__, stdin);
    if (__p1 == __p2) {
      NEOF = 0;
      return EOF;
    }
  }
  return *__p1++;
}

template <typename T>
inline bool rd(T& x) {
  char c = nc();
  bool f = 0;
  x = 0;
  while (!isdigit(c))
    c == '-' && (f = 1), c = nc();
  while (isdigit(c))
    x = x * 10 + (c ^ 48), c = nc();
  if (f)
    x = -x;
  return NEOF;
}

typedef unsigned long long ull;
ull myRand(ull& k1, ull& k2) {
  ull k3 = k1, k4 = k2;
  k1 = k4;
  k3 ^= (k3 << 23);
  k2 = k3 ^ k4 ^ (k3 >> 17) ^ (k4 >> 26);
  return k2 + k4;
}
pair<int, int> myRanq(ull& k1, ull& k2, int MAXN) {
  int x = myRand(k1, k2) % MAXN + 1, y = myRand(k1, k2) % MAXN + 1;
  if (x > y)
    return make_pair(y, x);
  else
    return make_pair(x, y);
}

struct LCAGraph {
  int n, m, dfs_clock;
  vector<vector<int>> G;
  vector<int> dfn, lg, dep;
  vector<vector<int>> st;
  LCAGraph(int _n = 0)
      : n(_n), m(n + n - 1), G(n), dfn(n), lg(m + 1), dep(n), st(m) {
    lg[1] = 0;
    for (int i = 2; i <= m; ++i)
      lg[i] = lg[i >> 1] + 1;
  }
  void addedge(int u, int v) { G[u].push_back(v); }
  void dfs(int u, int fa) {
    dfn[u] = dfs_clock;
    dep[u] = dep[fa] + 1;
    st[dfs_clock][0] = u;
    ++dfs_clock;
    for (int v : G[u]) {
      if (v == fa)
        continue;
      dfs(v, u);
      st[dfs_clock][0] = u;
      ++dfs_clock;
    }
  }
  void build(int rt) {
    dfs_clock = 0;
    int g = lg[m];
    for (int i = 0; i < m; ++i)
      st[i].resize(g + 1);
    dfs(rt, rt);
    for (int j = 1; j <= g; ++j) {
      for (int i = 0; i + (1 << (j - 1)) < m; ++i) {
        if (dep[st[i][j - 1]] < dep[st[i + (1 << (j - 1))][j - 1]])
          st[i][j] = st[i][j - 1];
        else
          st[i][j] = st[i + (1 << (j - 1))][j - 1];
      }
    }
  }
  int query(int u, int v) {
    int l = dfn[u], r = dfn[v];
    if (l > r)
      swap(l, r);
    int g = lg[r - l + 1];
    if (dep[st[l][g]] < dep[st[r - (1 << g) + 1][g]])
      return st[l][g];
    else
      return st[r - (1 << g) + 1][g];
  }
};

template <typename T>
struct ExKruskal {
  struct Edge {
    int u, v;
    T w;
    Edge() {}
    Edge(int _u, int _v, T _w)
        : u(_u), v(_v), w(_w) {}
    bool operator<(const Edge& e) const { return w < e.w; }
  };
  int n, m;
  vector<Edge> E;
  vector<T> W;
  vector<int> f;
  LCAGraph G;

  int find(int x) { return x == f[x] ? x : f[x] = find(f[x]); }

  ExKruskal(int _n = 0)
      : n(_n), W(n + n - 1), f(n + n - 1), G(n + n - 1) {
    iota(f.begin(), f.end(), 0);
  }

  void addedge(int u, int v, T w) { E.emplace_back(u, v, w); }

  void build() {
    sort(E.begin(), E.end());
    int id = n - 1, cnt = 0;
    for (auto& [u, v, w] : E) {
      u = find(u);
      v = find(v);
      if (u != v) {
        ++id;
        G.addedge(id, u);
        G.addedge(id, v);
        f[u] = f[v] = id;
        W[id] = w;
        ++cnt;
        if (cnt == n - 1)
          break;
      }
    }
    G.build(id);
  }

  int query(int u, int v) {
    int lca = G.query(u, v);
    return W[lca];
  }
};

int n, m, q;
ull a1, a2;
void solve(int Case) {
  rd(n), rd(m);
  ExKruskal<int> ek(n);

  int u, v, w;
  for (int i = 1; i <= m; ++i) {
    rd(u), rd(v), rd(w);
    --u;
    --v;
    ek.addedge(u, v, w);
  }

  ek.build();

  rd(q), rd(a1), rd(a2);

  int ans = 0;
  for (int i = 1; i <= q; ++i) {
    pair<int, int> p = myRanq(a1, a2, n);
    --p.first;
    --p.second;
    ans ^= ek.query(p.first, p.second);
  }
  printf("%d\n", ans);
}

int main() {
#ifdef BACKLIGHT
  freopen("a.in", "r", stdin);
#endif
  int T = 1;
  // cin >> T;
  for (int t = 1; t <= T; ++t)
    solve(t);
  return 0;
}
// 「APIO2018」铁人两项(https://loj.ac/p/2587)
// 给定一张简单无向图，问有多少对三元组 <s, c, f>（s, c, f互不相同）使得存在一条简单路径从 s 出发，经过 c 到达 f 。
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 2e5 + 5;

int n, m;
int w[N];
vector<int> G[N], F[N];

int cc, scc;
int dfc, dfn[N], low[N];
int top, stk[N];
void tarjan(int u) {
  ++cc;
  dfn[u] = low[u] = ++dfc;
  stk[++top] = u;
  for (int v : G[u]) {
    if (!dfn[v]) {
      tarjan(v);
      low[u] = min(low[u], low[v]);
      if (low[v] == dfn[u]) {
        ++scc;
        int np = n + scc;
        w[np] = 0;
        for (int x = 0; x != v; --top) {
          x = stk[top];
          F[np].push_back(x);
          F[x].push_back(np);
          ++w[np];
        }
        F[np].push_back(u);
        F[u].push_back(np);
        ++w[np];
      }
    } else
      low[u] = min(low[u], dfn[v]);
  }
}

ll ans;
int sz[N];
void dfs(int u, int fa) {
  sz[u] = (u <= n);
  for (int v : F[u])
    if (v != fa) {
      dfs(v, u);
      ans += 2ll * w[u] * sz[u] * sz[v];
      sz[u] += sz[v];
    }
  ans += 2ll * w[u] * sz[u] * (cc - sz[u]);
}

void buildBlockForest() {
  for (int i = 1; i <= n; ++i)
    if (!dfn[i]) {
      cc = 0;
      tarjan(i);
      --top;
      dfs(i, i);
    }
}

void solve(int Case) {
  scanf("%d %d", &n, &m);
  fill(w + 1, w + 1 + n, -1);
  int u, v;
  for (int i = 1; i <= m; ++i) {
    scanf("%d %d", &u, &v);
    G[u].push_back(v);
    G[v].push_back(u);
  }
  buildBlockForest();
  printf("%lld\n", ans);
}

int main() {
  int T = 1;
  // scanf("%d", &T);
  for (int i = 1; i <= T; ++i)
    solve(i);
  return 0;
}
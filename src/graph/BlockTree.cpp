// 树分块：uv之间路径上不同的颜色数（强制在线）
#include <bits/stdc++.h>
using namespace std;

const int N = 4e4 + 5;

int n, m, a[N];
int nt, t[N];

int tot, head[N];
struct edge {
  int v, nxt;
} e[N << 1];
void init(int n) {
  tot = 0;
  for (int i = 1; i <= n; ++i)
    head[i] = 0;
}
void add(int u, int v) {
  ++tot;
  e[tot] = (edge){v, head[u]};
  head[u] = tot;
}
#define fore(i, u) for (int i = head[u]; i; i = e[i].nxt)

int sz[N], son[N], f[N], h[N], top[N];

void dfs1(int u, int fa) {
  f[u] = fa;
  h[u] = h[fa] + 1;
  sz[u] = 1;
  son[u] = 0;
  fore(i, u) {
    int v = e[i].v;
    if (v == fa)
      continue;
    dfs1(v, u);
    sz[u] += sz[v];
    if (sz[v] > sz[son[u]])
      son[u] = v;
  }
}

void dfs2(int u, int fa, int k) {
  top[u] = k;
  if (son[u])
    dfs2(son[u], u, k);
  fore(i, u) {
    int v = e[i].v;
    if (v == fa || v == son[u])
      continue;
    dfs2(v, u, v);
  }
}

int lca(int u, int v) {
  while (top[u] != top[v]) {
    if (h[top[u]] < h[top[v]])
      swap(u, v);
    u = f[top[u]];
  }
  if (h[u] > h[v])
    swap(u, v);
  return u;
}

int dep[N], max_dep[N], pa[N];
int key_cnt, keyid[N];

const int COLORCNT = 4e4 + 2;
const int KEYCNT = 101;
const int gap = 400;

bitset<COLORCNT> c[KEYCNT][KEYCNT];

int stk[N], tp;

void dfs_key(int u, int fa) {
  dep[u] = dep[fa] + 1;
  max_dep[u] = dep[u];
  fore(i, u) {
    int v = e[i].v;
    if (v == fa)
      continue;
    dfs_key(v, u);
    if (max_dep[v] > max_dep[u])
      max_dep[u] = max_dep[v];
  }
  if (max_dep[u] - dep[u] >= gap) {
    keyid[u] = ++key_cnt;
    max_dep[u] = dep[u];
  }
}

void dfs_bitset(int u) {
  if (keyid[u] && u != stk[tp]) {
    for (int x = u; x != stk[tp]; x = f[x])
      c[keyid[stk[tp]]][keyid[u]].set(a[x]);

    for (int i = 1; i < tp; ++i) {
      c[keyid[stk[i]]][keyid[u]] = c[keyid[stk[i]]][keyid[stk[tp]]];
      c[keyid[stk[i]]][keyid[u]] |= c[keyid[stk[tp]]][keyid[u]];
    }
    pa[u] = stk[tp];
    stk[++tp] = u;
  }
  for (int i = head[u]; i; i = e[i].nxt) {
    if (e[i].v != f[u])
      dfs_bitset(e[i].v);
  }
  if (keyid[u])
    --tp;
}

void build_block_tree() {
  key_cnt = 0;
  dfs_key(1, 1);
  if (!keyid[1])
    keyid[1] = ++key_cnt;

  tp = 1;
  stk[1] = 1;
  dfs_bitset(1);
}

bitset<COLORCNT> res;

int query(int u, int v) {
  res.reset();
  int uv = lca(u, v);

  // step 1: jump to nearest key node
  while (u != uv && !keyid[u]) {
    res.set(a[u]);
    u = f[u];
  }
  while (v != uv && !keyid[v]) {
    res.set(a[v]);
    v = f[v];
  }

  // step 2: jump to lowest key node
  int pu = u;
  while (dep[pa[pu]] >= dep[uv])
    pu = pa[pu];
  if (pu != u) {
    res |= c[keyid[pu]][keyid[u]];
    u = pu;
  }

  int pv = v;
  while (dep[pa[pv]] >= dep[uv])
    pv = pa[pv];
  if (pv != v) {
    res |= c[keyid[pv]][keyid[v]];
    v = pv;
  }

  // step 3: jump to lca
  while (u != uv) {
    res.set(a[u]);
    u = f[u];
  }
  while (v != uv) {
    res.set(a[v]);
    v = f[v];
  }

  // step 4: set lca
  res.set(a[uv]);

  return res.count();
}

void solve(int Case) {
  scanf("%d %d", &n, &m);
  for (int i = 1; i <= n; ++i) {
    scanf("%d", &a[i]);
    t[i] = a[i];
  }

  sort(t + 1, t + 1 + n);
  nt = unique(t + 1, t + 1 + n) - (t + 1);

  for (int i = 1; i <= n; ++i)
    a[i] = lower_bound(t + 1, t + 1 + nt, a[i]) - t;

  init(n);
  int u, v;
  for (int i = 1; i <= n - 1; ++i) {
    scanf("%d %d", &u, &v);
    add(u, v);
    add(v, u);
  }

  dfs1(1, 1);
  dfs2(1, 1, 1);

  build_block_tree();

  int lastans = 0;
  for (int i = 1; i <= m; ++i) {
    scanf("%d %d", &u, &v);
    u ^= lastans;
    lastans = query(u, v);
    printf("%d\n", lastans);
  }
}

int main() {
  int T = 1;
  // scanf( "%d", &T );
  for (int _ = 1; _ <= T; _++)
    solve(_);
  return 0;
}
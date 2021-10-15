struct Edge {
  int v, w;
  Edge() {}
  Edge(int _v, int _w)
      : v(_v), w(_w) {}
};

vector<Edge> G[N];
inline void addedge(int u, int v, int w) {
  G[u].push_back(Edge(v, w));
  G[v].push_back(Edge(u, w));
}

bool vis[N];
int sz[N], max_sz[N];
void dfs_size(int u, int fa) {
  sz[u] = 1;
  max_sz[u] = 0;
  for (const Edge& e : G[u]) {
    int v = e.v;
    if (v == fa || vis[v])
      continue;
    dfs_size(v, u);
    sz[u] += sz[v];
    max_sz[u] = max(max_sz[u], sz[v]);
  }
}

int Max, rt;
void dfs_root(int r, int u, int fa) {
  max_sz[u] = max(max_sz[u], sz[r] - sz[u]);
  if (Max > max_sz[u])
    Max = max_sz[u], rt = u;
  for (const Edge& e : G[u]) {
    int v = e.v;
    if (v == fa || vis[v])
      continue;
    dfs_root(r, v, u);
  }
}

int dcnt, dis[N];
void dfs_dis(int u, int fa, int d) {
  dis[++dcnt] = d;
  for (const Edge& e : G[u]) {
    int v = e.v, w = e.w;
    if (v == fa || vis[v])
      continue;
    dfs_dis(v, u, d + w);
  }
}

int ans[K];
void calc(int u, int w, int delta) {
  dcnt = 0;
  dfs_dis(u, -1, w);
  for (int i = 1; i <= dcnt; ++i) {
    for (int j = i + 1; j <= dcnt; ++j) {
      ans[dis[i] + dis[j]] += delta;
    }
  }
}

int n, m;
void DFS(int u) {
  Max = n;
  dfs_size(u, -1);
  dfs_root(u, u, -1);
  vis[rt] = 1;
  calc(rt, 0, 1);
  for (const Edge& e : G[rt]) {
    int v = e.v, w = e.w;
    if (vis[v])
      continue;
    calc(v, w, -1);
    DFS(v);
  }
}

void solve() {
  read(n, m);

  int u, v, w;
  FOR(i, 2, n) {
    read(u, v, w);
    addedge(u, v, w);
  }

  DFS(1);

  int k;
  FOR(i, 1, m) {
    read(k);
    puts(ans[k] ? "AYE" : "NAY");
  }
}
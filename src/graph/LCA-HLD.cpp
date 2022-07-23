class LCA {
 private:
  int n_;
  const std::vector<std::vector<int>>& g_;
  std::vector<int> f, sz, son, dep, top;
 
  void dfs1(int u, int fa) {
    f[u] = fa;
    sz[u] = 1;
    son[u] = -1;
    for (int v : g_[u]) {
      if (v == fa)
        continue;
      dep[v] = dep[u] + 1;
      dfs1(v, u);
      sz[u] += sz[v];
      if (son[u] == -1 || sz[v] > sz[son[u]])
        son[u] = v;
    }
  }
 
  void dfs2(int u, int fa, int tp) {
    top[u] = tp;
    if (son[u] != -1)
      dfs2(son[u], u, tp);
    for (int v : g_[u]) {
      if (v == fa || v == son[u])
        continue;
      dfs2(v, u, v);
    }
  }
 
 public:
  LCA(const std::vector<std::vector<int>>& g) : n_(g.size()), g_(g) {
    dep.resize(n_);
    f.resize(n_);
    sz.resize(n_);
    son.resize(n_);
    top.resize(n_);
 
    dep[0] = 0;
    dfs1(0, 0);
    dfs2(0, 0, 0);
  }
 
  int lca(int u, int v) {
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]])
        std::swap(u, v);
      u = f[top[u]];
    }
    if (dep[u] > dep[v])
      std::swap(u, v);
    return u;
  }
};
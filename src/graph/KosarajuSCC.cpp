std::vector<std::vector<int>> KosarajuSCC(const std::vector<std::vector<int>>& g) {
  int n = g.size();
  std::vector<std::vector<int>> r(n);
  for (int u = 0; u < n; ++u) {
    for (int v : g[u]) {
      r[v].push_back(u);
    }
  }

  int dfs_clock = 0;
  std::vector<bool> vis(n, false);
  std::vector<int> dfn(n, -1);
  std::function<void(int)> dfs1 = [&](int u) {
    vis[u] = true;
    for (const int& v : g[u]) {
      if (!vis[v])
        dfs1(v);
    }
    dfn[dfs_clock++] = u;
  };
  for (int i = 0; i < n; ++i) {
    if (!vis[i])
      dfs1(i);
  }

  int scc_count = 0;
  std::vector<std::vector<int>> sccs;
  std::vector<int> belong(n, -1);
  std::function<void(int)> dfs2 = [&](int u) {
    belong[u] = scc_count - 1;
    sccs[scc_count - 1].push_back(u);
    for (const int& v : r[u]) {
      if (belong[v] == -1)
        dfs2(v);
    }
  };
  for (int i = n - 1; i >= 0; --i) {
    if (belong[dfn[i]] == -1) {
      ++scc_count;
      sccs.push_back({});
      dfs2(dfn[i]);
    }
  }

  return sccs;
}
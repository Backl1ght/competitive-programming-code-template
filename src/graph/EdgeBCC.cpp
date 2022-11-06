std::vector<std::vector<int>> EdgeBCC(
    int n,
    const std::vector<std::pair<int, int>>& E) {
  int m = E.size();

  std::vector<std::vector<std::pair<int, int>>> g(n);
  for (int i = 0; i < m; ++i) {
    auto [u, v] = E[i];
    g[u].push_back({v, 2 * i});
    g[v].push_back({u, 2 * i + 1});
  }

  std::vector<bool> is_bridge(m, false);
  std::vector<std::vector<int>> bccs;
  std::vector<int> belong(n, -1);

  int dfs_clock = 0;
  std::vector<int> dfn(n, -1), low(n);
  std::function<void(int, int)> tarjan = [&](int u, int fa) {
    dfn[u] = low[u] = dfs_clock++;
    for (auto [v, edge_id] : g[u]) {
      if (v == fa)
        continue;

      if (dfn[v] == -1) {
        tarjan(v, u);

        low[u] = std::min(low[u], low[v]);

        if (dfn[u] < low[v]) {
          is_bridge[edge_id / 2] = true;
        }
      } else if (dfn[v] < dfn[u]) {
        low[u] = std::min(low[u], dfn[v]);
      }
    }
  };
  for (int i = 0; i < n; ++i) {
    if (dfn[i] == -1) {
      tarjan(i, i);
    }
  }

  std::function<void(int)> blood_fill = [&](int u) {
    belong[u] = bccs.size() - 1;
    bccs.back().push_back(u);
    for (auto [v, edge_id] : g[u]) {
      if (is_bridge[edge_id / 2])
        continue;
      if (belong[v] != -1)
        continue;
      blood_fill(v);
    }
  };
  for (int i = 0; i < n; ++i) {
    if (belong[i] == -1) {
      bccs.push_back({});
      blood_fill(i);
    }
  }

  return bccs;
}
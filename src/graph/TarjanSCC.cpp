std::vector<std::vector<int>> TarjanSCC(const std::vector<std::vector<int>>& g) {
  int n = g.size();

  int dfs_clock = 0;
  std::vector<int> dfn(n, -1), low(n);
  std::vector<bool> in_stack(n, false);
  std::stack<int> stk;

  int scc_count = 0;
  std::vector<int> belong(n, -1);
  std::vector<std::vector<int>> sccs;

  std::function<void(int)> tarjan = [&](int u) {
    dfn[u] = low[u] = dfs_clock++;
    in_stack[u] = true;
    stk.push(u);

    for (int v : g[u]) {
      if (dfn[v] == -1) {
        tarjan(v);
        low[u] = std::min(low[u], low[v]);
      } else if (in_stack[v]) {
        low[u] = std::min(low[u], dfn[v]);
      }
    }

    if (dfn[u] == low[u]) {
      ++scc_count;
      sccs.push_back({});

      int x;
      do {
        x = stk.top();
        stk.pop();

        in_stack[x] = false;
        belong[x] = scc_count - 1;
        sccs[scc_count - 1].push_back(x);
      } while (u != x);
    }
  };
  for (int i = 0; i < n; ++i) {
    if (dfn[i] == -1) {
      tarjan(i);
    }
  }

  return sccs;
}
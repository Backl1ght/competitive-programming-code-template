std::vector<std::vector<int>> PointBCC(const std::vector<std::vector<int>>& g) {
  int n = g.size();

  std::vector<std::vector<int>> bccs;
  std::vector<int> belong(n, -1);
  std::vector<bool> is_cut(n, false);

  int dfs_clock = 0;
  std::vector<int> dfn(n, -1), low(n);
  std::stack<int> stack;
  std::function<void(int, int)> tarjan = [&](int u, int fa) {
    dfn[u] = low[u] = dfs_clock++;
    stack.push(u);

    int son = 0;
    for (int v : g[u]) {
      if (v == fa)
        continue;

      if (dfn[v] == -1) {
        tarjan(v, u);

        low[u] = std::min(low[u], low[v]);

        if (dfn[u] <= low[v]) {
          ++son;
          if (u != fa || son > 1)
            is_cut[u] = true;
          bccs.push_back({});
          int top;
          do {
            top = stack.top();
            stack.pop();
            bccs.back().push_back(top);
            belong[top] = bccs.size() - 1;
          } while (top != v);
          bccs.back().push_back(u);
          belong[u] = bccs.size() - 1;
        }
      } else if (dfn[v] < dfn[u]) {
        low[u] = std::min(low[u], dfn[v]);
      }
    }

    if (u == fa && son == 0) {
      bccs.push_back({u});
      belong[u] = bccs.size() - 1;
    }
  };
  for (int i = 0; i < n; ++i) {
    if (dfn[i] == -1) {
      while (!stack.empty())
        stack.pop();
      tarjan(i, i);
    }
  }

  return bccs;
}
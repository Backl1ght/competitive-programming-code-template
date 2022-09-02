class TwoSAT {
 private:
 public:
  TwoSAT(int n) : n_(n), g_(2 * n_), r_(2 * n_) {}

  int ID(int x, int y) {
    ASSERT(0 <= x && x < n_);
    ASSERT(y == 0 || y == 1);

    return 2 * x + y;
  }

  void Either(int x, int y) {
    g_[x ^ 1].push_back(y);
    r_[y].push_back(x ^ 1);

    g_[y ^ 1].push_back(x);
    r_[x].push_back(y ^ 1);
  }

  void Implies(int x, int y) { Either(x ^ 1, y); }

  void Must(int x) { Either(x, x); }

  std::pair<bool, std::vector<int>> Solve() {
    int dfs_clock = 0;
    std::vector<bool> vis(2 * n_, false);
    std::vector<int> dfn(2 * n_, -1);
    std::function<void(int)> dfs1 = [&](int u) {
      vis[u] = true;
      for (int v : g_[u]) {
        if (vis[v] == false) {
          dfs1(v);
        }
      }
      dfn[dfs_clock++] = u;
    };
    for (int i = 0; i < 2 * n_; ++i) {
      if (!vis[i]) {
        dfs1(i);
      }
    }

    int c = 0;
    std::vector<int> color(2 * n_, -1);
    std::function<void(int)> dfs2 = [&](int u) {
      color[u] = c;
      for (int v : r_[u]) {
        if (color[v] == -1) {
          dfs2(v);
        }
      }
    };
    for (int i = 2 * n_ - 1; i >= 0; --i) {
      int u = dfn[i];
      if (color[u] == -1) {
        dfs2(u);
        c++;
      }
    }

    for (int i = 0; i < n_; ++i) {
      if (color[2 * i] == color[2 * i + 1]) {
        return {false, {}};
      }
    }

    std::vector<int> values(n_, false);
    for (int i = 0; i < n_; ++i) {
      values[i] = color[2 * i] < color[2 * i + 1];
    }

    return {true, values};
  }

 private:
  int n_;
  std::vector<std::vector<int>> g_, r_;
};
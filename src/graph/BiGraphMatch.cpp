// Hopcroft Karp, O(\sqrt{V}E)
struct bigraph {
  int dfn;

  vector<vector<int>> G;

  int nl, nr;
  vector<int> ml, mr;
  vector<int> ll, lr;
  vector<int> vis;

  bigraph(int _nl, int _nr) {
    nl = _nl;
    nr = _nr;
    G = vector<vector<int>>(nl + 1);
  }

  void addarc(int u, int v) {
    G[u].push_back(v);
  }

  void addedge(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }

  bool bfs() {
    queue<int> q;
    bool res = false;

    for (int i = 1; i <= nl; ++i) {
      if (ml[i])
        ll[i] = 0;
      else
        ll[i] = 1, q.push(i);
    }

    for (int i = 1; i <= nr; ++i)
      lr[i] = 0;

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int v : G[u]) {
        if (lr[v] == 0) {
          lr[v] = ll[u] + 1;
          if (mr[v]) {
            ll[mr[v]] = lr[v] + 1;
            q.push(mr[v]);
          } else
            res = true;
        }
      }
    }

    return res;
  };

  bool dfs(int u) {
    for (int v : G[u]) {
      if (lr[v] == ll[u] + 1 && vis[v] != dfn) {
        vis[v] = dfn;
        if (mr[v] == 0 || dfs(mr[v])) {
          mr[v] = u;
          ml[u] = v;
          return true;
        }
      }
    }
    return false;
  };

  int HK() {
    ml = vector<int>(nl + 1);
    mr = vector<int>(nr + 1);
    ll = vector<int>(nl + 1);
    lr = vector<int>(nr + 1);
    vis = vector<int>(nr + 1);

    int res = 0;
    while (bfs()) {
      ++dfn;
      for (int i = 1; i <= nl; ++i)
        if (!ml[i])
          res += dfs(i);
    }
    return res;
  }
};

/**
 * 最小覆盖数 = 最大匹配数
 * 最大独立集 = 顶点数 - 二分图匹配数
 * DAG 最小路径覆盖数 = 结点数 - 拆点后二分图最大匹配数
 */
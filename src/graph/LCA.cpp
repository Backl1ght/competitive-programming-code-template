namespace Backlight {

template <typename T>
struct Wraph {
  struct Edge {
    int u, v;
    T w;
    Edge() {}
    Edge(int _u, int _v, T _w)
        : u(_u), v(_v), w(_w) {}
  };

  int V;
  vector<vector<Edge>> G;

  Wraph()
      : V(0) {}
  Wraph(int _V)
      : V(_V), G(_V + 1) {}

  inline void addarc(int u, int v, T w = 1) {
    assert(1 <= u && u <= V);
    assert(1 <= v && v <= V);
    G[u].push_back(Edge(u, v, w));
  }

  inline void addedge(int u, int v, T w = 1) {
    addarc(u, v, w);
    addarc(v, u, w);
  }

  /**************************************************/
  vector<int> dep;
  vector<T> dis;
  vector<vector<int>> par;
  int rt, LG;
  void dfs(int u, int fa, int d1, int d2) {
    dep[u] = d1;
    dis[u] = d2;
    if (u == rt) {
      for (int i = 0; i < LG; ++i)
        par[u][i] = rt;
    } else {
      par[u][0] = fa;
      for (int i = 1; i < LG; ++i) {
        par[u][i] = par[par[u][i - 1]][i - 1];
      }
    }

    for (Edge& e : G[u]) {
      int v = e.v;
      T w = e.w;
      if (v == fa)
        continue;
      dfs(v, u, d1 + 1, d2 + w);
    }
  }

  inline void build_lca(int _rt) {
    rt = _rt;
    LG = __lg(V + 1) + 1;
    dep = vector<int>(V + 1);
    dis = vector<T>(V + 1);
    par = vector<vector<int>>(V + 1, vector<int>(LG));
    dfs(rt, rt, 0, 0);
  }

  inline int jump(int u, int d) {
    for (int j = LG - 1; j >= 0; --j) {
      if ((1 << j) & d)
        u = par[u][j];
    }
    return u;
  }

  int lca(int u, int v) {
    if (dep[u] < dep[v])
      swap(u, v);
    u = jump(u, dep[u] - dep[v]);
    if (u == v)
      return u;
    for (int i = LG - 1; i >= 0; --i) {
      if (par[u][i] != par[v][i]) {
        u = par[u][i];
        v = par[v][i];
      }
    }
    return par[u][0];
  }
};

};  // namespace Backlight
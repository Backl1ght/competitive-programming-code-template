namespace Backlight {

template <typename T>
struct Wraph {
  struct Edge {
    int u, v;
    T w;
    Edge() {}
    Edge(int _u, int _v, T _w)
        : u(_u), v(_v), w(_w) {}
    bool operator<(const Edge& e) {
      return w < e.w;
    }
  };

  int V;
  vector<vector<Edge>> G;
  vector<Edge> E;

  Wraph()
      : V(0) {}
  Wraph(int _V)
      : V(_V), G(_V + 1) {}

  inline void addarc(int u, int v, T w) {
    assert(1 <= u && u <= V);
    assert(1 <= v && v <= V);
    G[u].push_back(Edge(u, v, w));
    E.push_back(Edge(u, v, w));
  }

  inline void addedge(int u, int v, T w) {
    addarc(u, v, w);
    addarc(v, u, w);
  }

  /**************************************************/
  T kruskal() {
    vector<int> fa(V + 1);
    for (int i = 1; i <= V; ++i)
      fa[i] = i;

    auto find = [&fa](auto self, int x) {
      if (x == fa[x])
        return x;
      fa[x] = self(self, fa[x]);
      return fa[x];
    };

    auto merge = [&fa, find](int x, int y) {
      x = find(find, x);
      y = find(find, y);
      if (x == y)
        return false;
      fa[x] = y;
      return true;
    };

    T cost = 0;
    int cnt = 0;
    sort(E.begin(), E.end());
    for (int i = 0; i < (int)E.size(); ++i) {
      Edge e = E[i];
      if (merge(e.u, e.v)) {
        cost = e.w;
        ++cnt;
        if (cnt == V - 1)
          break;
      }
    }
    return cost;
  }
};

}  // namespace Backlight
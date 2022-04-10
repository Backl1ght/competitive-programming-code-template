namespace Backlight {

template <typename Cap, typename Cost>
struct mcmf_graph {
  static const Cap INF = numeric_limits<Cap>::max();

  struct Edge {
    int v, nxt;
    Cap cap, flow;
    Cost cost;
    Edge() {}
    Edge(int _v, int _nxt, Cap _cap, Cost _cost)
        : v(_v), nxt(_nxt), cap(_cap), flow(0), cost(_cost) {}
  };

  int V, E;
  vector<int> h;
  vector<Edge> e;

  mcmf_graph()
      : V(0) {}
  mcmf_graph(int _V)
      : V(_V), h(_V + 1, -1) {}

  inline void addarc(int u, int v, Cap cap, Cost cost) {
    assert(1 <= u && u <= V);
    assert(1 <= v && v <= V);
    e.push_back(Edge(v, h[u], cap, cost));
    h[u] = e.size() - 1;
  }

  inline void addedge(int u, int v, Cap cap, Cost cost) {
    addarc(u, v, cap, cost);
    addarc(v, u, 0, -cost);
  }

  pair<Cap, Cost> mcmf(int s, int t) {
    assert(1 <= s && s <= V);
    assert(1 <= t && t <= V);
    assert(s != t);

    Cap flow = 0;
    Cost cost = 0;

    vector<int> pe(V + 1);
    vector<bool> inq(V + 1);
    vector<Cost> dis(V + 1);
    vector<Cap> incf(V + 1);

    auto spfa = [&]() {
      fill(dis.begin(), dis.end(), INF);
      queue<int> q;
      q.push(s);
      dis[s] = 0;
      incf[s] = INF;
      incf[t] = 0;
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (int i = h[u]; i != -1; i = e[i].nxt) {
          int v = e[i].v;
          Cap _cap = e[i].cap;
          Cost _cost = e[i].cost;
          if (_cap == 0 || dis[v] <= dis[u] + _cost)
            continue;
          dis[v] = dis[u] + _cost;
          incf[v] = min(_cap, incf[u]);
          pe[v] = i;
          if (!inq[v])
            q.push(v), inq[v] = true;
        }
      }
      return incf[t];
    };

    auto update = [&]() {
      flow += incf[t];
      for (int i = t; i != s; i = e[pe[i] ^ 1].v) {
        e[pe[i]].cap -= incf[t];
        e[pe[i] ^ 1].cap += incf[t];
        cost += incf[t] * e[pe[i]].cost;
      }
    };

    while (spfa())
      update();

    return make_pair(flow, cost);
  }
};

}  // namespace Backlight
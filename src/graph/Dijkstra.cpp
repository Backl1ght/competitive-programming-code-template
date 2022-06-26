template <typename DistanceType>
struct Edge {
  int v_;
  DistanceType w_;
  Edge() {}
  Edge(int v, DistanceType w) : v_(v), w_(w) {}
};

template <typename DistanceType>
std::vector<DistanceType> Dijkstra(const std::vector<std::vector<Edge<DistanceType>>>& g, int s) {
  using Node = std::pair<DistanceType, int>;
  const DistanceType INF = std::numeric_limits<DistanceType>::max();
  const int n = g.size();

  std::vector<DistanceType> dis(n, INF);
  std::vector<bool> vis(n, false);

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
  dis[s] = 0;
  q.push(Node(dis[s], s));
  while (!q.empty()) {
    auto [c, u] = q.top();
    q.pop();

    if (vis[u])
      continue;
    vis[u] = true;

    for (auto [v, w] : g[u]) {
      if (dis[v] > c + w) {
        dis[v] = c + w;
        q.push(Node(dis[v], v));
      }
    }
  }
  return dis;
}
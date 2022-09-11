template <typename DistanceType,
          typename Comp = std::greater<>,
          typename Edge = std::pair<DistanceType, int>,
          typename Node = std::pair<DistanceType, int>>
std::vector<DistanceType> Dijkstra(const std::vector<std::vector<Edge>>& g, int s) {
  const DistanceType INF = std::numeric_limits<DistanceType>::max();
  const int n = g.size();
  const Comp comp;

  std::vector<DistanceType> dis(n, INF);
  std::vector<bool> vis(n, false);

  std::priority_queue<Node, std::vector<Node>, Comp> q;
  dis[s] = 0;
  q.push(Node(dis[s], s));
  while (!q.empty()) {
    auto [c, u] = q.top();
    q.pop();

    if (vis[u])
      continue;
    vis[u] = true;

    for (auto [w, v] : g[u]) {
      if (comp(dis[v], c + w)) {
        dis[v] = c + w;
        q.push(Node(dis[v], v));
      }
    }
  }

  return dis;
}
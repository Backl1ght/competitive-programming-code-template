// Problem: P3381 【模板】最小费用最大流
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P3381
// Memory Limit: 128 MB
// Time Limit: 1000 ms
//
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>

#define CPPIO std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
#define freep(p) p ? delete p, p = nullptr, void(1) : void(0)

#ifdef BACKLIGHT
#include "debug.h"
#else
#define logd(...) ;
#endif

using i64 = int64_t;
using u64 = uint64_t;

void solve_case(int Case);

int main(int argc, char* argv[]) {
  CPPIO;
  int T = 1;
  // std::cin >> T;
  for (int t = 1; t <= T; ++t) {
    solve_case(t);
  }
  return 0;
}

template <typename CapacityType, typename CostType>
class MinCostMaxFlowGraph {
 private:
  struct Edge {
    int from, to;
    CapacityType capacity;
    CostType cost;
    Edge() {}
    Edge(int _from, int _to, CapacityType _capacity, CostType _cost)
        : from(_from), to(_to), capacity(_capacity), cost(_cost) {}
  };
  int n_;
  int m_;
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> adjacent_;

 public:
  MinCostMaxFlowGraph(int n) : n_(n), m_(0), edges_(0), adjacent_(n) {}

  void addedge(int u, int v, CapacityType capacity, CostType cost) {
    assert(0 <= u and u < n_);
    assert(0 <= v and v < n_);

    edges_.push_back(Edge(u, v, capacity, cost));
    adjacent_[u].push_back(m_);
    ++m_;

    edges_.push_back(Edge(v, u, 0, -cost));
    adjacent_[v].push_back(m_);
    ++m_;
  }

  std::pair<CapacityType, CostType> EK(int src, int dst) {
    const static CapacityType INF = std::numeric_limits<CapacityType>::max();

    std::vector<CostType> h(n_);
    std::vector<bool> inq(n_);
    std::function<void()> spfa = [&]() -> void {
      std::fill(h.begin(), h.end(), INF);

      std::queue<int> q;
      q.push(src);
      inq[src] = true;
      h[src] = 0;
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;

        for (int edge_id : adjacent_[u]) {
          auto [from, to, capacity, cost] = edges_[edge_id];
          if (capacity <= 0 || h[to] <= h[from] + cost)
            continue;
          h[to] = h[from] + cost;
          if (!inq[to]) {
            q.push(to);
            inq[to] = true;
          }
        }
      }
    };

    std::vector<CostType> d(n_);
    std::vector<bool> vis(n_);
    std::vector<int> prev(n_);
    std::function<bool()> dijkstra = [&]() -> bool {
      std::fill(d.begin(), d.end(), INF);
      std::fill(vis.begin(), vis.end(), false);

      std::priority_queue<std::pair<CostType, int>> q;
      d[src] = 0;
      q.push(std::make_pair(-d[src], src));

      while (!q.empty()) {
        auto [_, u] = q.top();
        q.pop();

        if (vis[u])
          continue;
        vis[u] = true;

        for (int edge_id : adjacent_[u]) {
          auto [from, to, capacity, cost] = edges_[edge_id];
          CostType new_cost = cost + h[from] - h[to];

          if (vis[to] || capacity <= 0 || d[to] <= d[from] + new_cost)
            continue;

          d[to] = d[from] + new_cost;
          prev[to] = edge_id;
          q.push(std::make_pair(-d[to], to));
        }
      }

      return d[dst] != INF;
    };

    spfa();

    CapacityType max_flow = 0;
    CostType min_cost = 0;
    while (dijkstra()) {
      CapacityType augment = INF;
      for (int p = dst; p != src; p = edges_[prev[p] ^ 1].to) {
        augment = std::min(augment, edges_[prev[p]].capacity);
      }

      max_flow += augment;
      for (int p = dst; p != src; p = edges_[prev[p] ^ 1].to) {
        min_cost += edges_[prev[p]].cost * augment;
        edges_[prev[p]].capacity -= augment;
        edges_[prev[p] ^ 1].capacity += augment;
      }

      for (int i = 0; i < n_; ++i)
        h[i] += d[i];
    }

    return {min_cost, max_flow};
  }
};

void solve_case(int Case) {
  int n, m, s, t;
  std::cin >> n >> m >> s >> t;
  --s, --t;

  MinCostMaxFlowGraph<int, int> g(n);
  for (int i = 0, u, v, cap, cost; i < m; ++i) {
    std::cin >> u >> v >> cap >> cost;
    --u, --v;
    g.addedge(u, v, cap, cost);
  }

  auto [min_cost, max_flow] = g.EK(s, t);

  std::cout << max_flow << " " << min_cost << "\n";
}
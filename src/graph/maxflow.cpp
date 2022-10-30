// Problem: P3376 【模板】网络最大流
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P3376
// Memory Limit: 128 MB
// Time Limit: 2000 ms
//
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>

#define CPPIO \
  std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
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

template <typename CapacityType>
class MaxFlowGraph {
  struct Edge {
    int from, to;
    CapacityType capacity, flow;
    Edge() {}
    Edge(int _from, int _to, CapacityType _capacity, CapacityType _flow)
        : from(_from), to(_to), capacity(_capacity), flow(_flow) {}
  };

  int n_;
  int m_;
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> adjacent_;

 public:
  explicit MaxFlowGraph(int n) : n_(n), m_(0), edges_(0), adjacent_(n) {}

  void AddEdge(int from, int to, CapacityType capacity) {
    assert(0 <= from and from < n_);
    assert(0 <= to and to < n_);

    edges_.emplace_back(from, to, capacity, 0);
    adjacent_[from].push_back(m_);
    ++m_;

    edges_.emplace_back(to, from, 0, 0);
    adjacent_[to].push_back(m_);
    ++m_;
  }

  CapacityType Dinic(int src, int dst) {
    const static CapacityType INF = std::numeric_limits<CapacityType>::max();
    std::vector<int> level(n_);
    std::vector<int> start_index(n_);

    std::function<bool()> bfs = [&]() -> bool {
      std::fill(level.begin(), level.end(), -1);

      std::queue<int> q;
      q.push(src);
      level[src] = 0;

      while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int edge_id : adjacent_[u]) {
          auto [from, to, capacity, flow] = edges_[edge_id];
          CapacityType residual_capacity = capacity - flow;
          if (residual_capacity > 0 && level[to] == -1) {
            level[to] = level[u] + 1;
            if (to == dst)
              break;
            q.push(to);
          }
        }
      }

      return level[dst] != -1;
    };

    std::function<CapacityType(int, CapacityType)> dfs =
        [&](int u, CapacityType max_augment) -> CapacityType {
      if (u == dst)
        return max_augment;

      if (max_augment == 0)
        return 0;

      CapacityType total_augment = 0;
      int i = start_index[u];
      for (; i < (int)adjacent_[u].size(); ++i) {
        int edge_id = adjacent_[u][i];
        auto [from, to, capacity, flow] = edges_[edge_id];
        if (level[to] == level[u] + 1) {
          CapacityType residual_capacity = capacity - flow;
          CapacityType new_augment =
              dfs(to, std::min(max_augment, residual_capacity));
          if (new_augment <= 0)
            continue;

          max_augment -= new_augment;
          total_augment += new_augment;
          edges_[edge_id].flow += new_augment;
          edges_[edge_id ^ 1].flow -= new_augment;

          if (max_augment == 0)
            break;
        }
      }
      start_index[u] = i;

      if (total_augment == 0)
        level[u] = -1;

      return total_augment;
    };

    CapacityType max_flow = 0;
    while (bfs()) {
      std::fill(start_index.begin(), start_index.end(), 0);
      CapacityType new_flow = dfs(src, INF);
      logd(new_flow);
      max_flow += new_flow;
    }

    return max_flow;
  }

  std::vector<bool> Cut(int s) {
    std::vector<bool> visited(n_, false);

    std::function<void(int)> dfs = [&](int u) {
      visited[u] = true;
      for (int edge_id : adjacent_[u]) {
        auto [from, to, capacity, flow] = edges_[edge_id];
        if (visited[to] || capacity == flow)
          continue;

        dfs(to);
      }
    };
    dfs(s);

    return visited;
  }
};

void solve_case(int Case) {
  int n, m, s, t;
  std::cin >> n >> m >> s >> t;
  --s, --t;
  MaxFlowGraph<i64> g(n);
  for (int i = 0, u, v, w; i < m; ++i) {
    std::cin >> u >> v >> w;
    --u, --v;
    g.AddEdge(u, v, w);
  }
  std::cout << g.Dinic(s, t) << "\n";
}

#include <bits/stdc++.h>

#define CPPIO std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
#define freep(p) p ? delete p, p = nullptr, void(1) : void(0)

#if defined(BACKLIGHT) && !defined(NASSERT)
#define ASSERT(x)                                                                         \
  ((x) || (fprintf(stderr, "assertion failed (" __FILE__ ":%d): \"%s\"\n", __LINE__, #x), \
           assert(false), false))
#else
#define ASSERT(x) ;
#endif

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

/**
 * Dynamic Forest Maintained With Euler Tour Tree.
 *
 * As said in reference, link and cut operation of dynamic trees can be transformed into sequence
 * split and sequence merge operation, which can be easily maintained using balanced search trees
 * like Treap.
 *
 * @reference: Dynamic trees as search trees via euler tours, applied to the network simplex
 * algorithm by Robert E. Tarjan. https://link.springer.com/article/10.1007/BF02614369
 */
class DynamicForest {
 private:
  static std::mt19937 rng_;

  struct Node {
    int size_;
    int priority_;

    Node* left_;
    Node* right_;
    Node* parent_;

    int from_;
    int to_;

    std::array<int, 2> node_mask_;
    std::array<int, 2> subtree_mask_;

    Node(int from, int to)
        : size_(1),
          priority_(rng_()),
          left_(nullptr),
          right_(nullptr),
          parent_(nullptr),
          from_(from),
          to_(to),
          node_mask_({0, 0}),
          subtree_mask_({0, 0}) {}

    inline void Maintain() {
      size_ = 1;
      subtree_mask_ = node_mask_;
      if (left_) {
        size_ += left_->size_;
        subtree_mask_[0] += left_->subtree_mask_[0];
        subtree_mask_[1] += left_->subtree_mask_[1];
        left_->parent_ = this;
      }
      if (right_) {
        size_ += right_->size_;
        subtree_mask_[0] += right_->subtree_mask_[0];
        subtree_mask_[1] += right_->subtree_mask_[1];
        right_->parent_ = this;
      }
    }
  };

  static inline int GetSize(Node* p) { return p == nullptr ? 0 : p->size_; }

  static inline Node* FindRoot(Node* p) {
    if (!p)
      return nullptr;

    while (p->parent_ != nullptr)
      p = p->parent_;
    return p;
  }

  static std::string to_string(Node* p) {
    std::stringstream ss;

    ss << "Node [\n";

    std::function<void(Node*)> dfs = [&](Node* p) {
      if (!p)
        return;
      dfs(p->left_);
      ss << "(" << p->from_ << "," << p->to_ << "),";
      dfs(p->right_);
    };
    dfs(p);

    ss << "]\n\n";

    return ss.str();
  }

  inline Node* AllocateNode(int u, int v) {
    Node* p = new Node(u, v);
    return p;
  }

  inline void FreeNode(Node*& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  /*
   * Dynamic Sequence Maintained using Treap.
   */
  class Treap {
   public:
    /**
     * Merge two treap a and b into a single treap, with keys in a less than keys in b.
     *
     * In the other word, concating sequence a and sequence b.
     */
    static Node* Merge(Node* a, Node* b) {
      if (a == nullptr)
        return b;
      if (b == nullptr)
        return a;

      if (a->priority_ < b->priority_) {
        a->right_ = Merge(a->right_, b);
        a->Maintain();
        return a;
      } else {
        b->left_ = Merge(a, b->left_);
        b->Maintain();
        return b;
      }
    }

    /**
     * Get the number of nodes with keys less than or equal to the key of p.
     *
     * In the other word, the the 1-based index of p inside the sequencec containing p.
     */
    static int GetPosition(Node* p) {
      ASSERT(p != nullptr);

      int position = GetSize(p->left_) + 1;
      while (p) {
        if (p->parent_ && p == p->parent_->right_)
          position += GetSize(p->parent_->left_) + 1;
        p = p->parent_;
      }
      return position;
    }

    /*
     * Bottom up split treap p into 2 treaps a and b.
     *   - a: a treap containing nodes with position less than or equal to p.
     *   - b: a treap containing nodes with postion greater than p.
     *
     * In the other word, split sequence containning p into two sequences, the first one contains
     * elements before p and element p, the second one contains elements after p.
     */
    static std::pair<Node*, Node*> SplitUp2(Node* p) {
      ASSERT(p != nullptr);

      Node *a = nullptr, *b = nullptr;
      b = p->right_;
      if (b)
        b->parent_ = nullptr;
      p->right_ = nullptr;

      bool is_p_left_child_of_parent = false;
      bool is_from_left_child = false;
      while (p) {
        Node* parent = p->parent_;

        if (parent) {
          is_p_left_child_of_parent = (parent->left_ == p);
          if (is_p_left_child_of_parent) {
            parent->left_ = nullptr;
          } else {
            parent->right_ = nullptr;
          }
          p->parent_ = nullptr;
        }

        if (!is_from_left_child) {
          a = Merge(p, a);
        } else {
          b = Merge(b, p);
        }

        is_from_left_child = is_p_left_child_of_parent;
        p->Maintain();
        p = parent;
      }
      if (a)
        a->parent_ = nullptr;
      if (b)
        b->parent_ = nullptr;

      return {a, b};
    }

    /*
     * Bottom up split treap p into 3 treaps a, b and c.
     *   - a: a treap containing nodes with key less than p.
     *   - b: a treap containing nodes with key greater than p.
     *   - c: a treap containing nodes with key equal p.
     *
     * In the other word, split sequence containning p into three sequences, the first one contains
     * elements before p, the second one contains element p, the third one contains elements after
     * p.
     */
    static std::tuple<Node*, Node*, Node*> SplitUp3(Node* p) {
      ASSERT(p != nullptr);

      Node* a = p->left_;
      if (a)
        a->parent_ = nullptr;
      p->left_ = nullptr;

      Node* b = p->right_;
      if (b)
        b->parent_ = nullptr;
      p->right_ = nullptr;

      Node* c = p;

      bool is_p_left_child_of_parent = false;
      bool is_from_left_child = false;
      Node* parent = p->parent_;
      if (parent) {
        is_p_left_child_of_parent = (parent->left_ == p);
        if (is_p_left_child_of_parent) {
          parent->left_ = nullptr;
        } else {
          parent->right_ = nullptr;
        }
        p->parent_ = nullptr;
      }
      is_from_left_child = is_p_left_child_of_parent;
      p->Maintain();
      p = parent;

      while (p) {
        Node* parent = p->parent_;

        if (parent) {
          is_p_left_child_of_parent = (parent->left_ == p);
          if (is_p_left_child_of_parent) {
            parent->left_ = nullptr;
          } else {
            parent->right_ = nullptr;
          }
          p->parent_ = nullptr;
        }

        if (!is_from_left_child) {
          a = Merge(p, a);
        } else {
          b = Merge(b, p);
        }

        is_from_left_child = is_p_left_child_of_parent;
        p->Maintain();
        p = parent;
      }
      if (a)
        a->parent_ = nullptr;
      if (b)
        b->parent_ = nullptr;
      if (c)
        c->parent_ = nullptr;

      return {a, c, b};
    }
  };

  template <int mask_index>
  void Mark(Node* p, int mask) {
    ASSERT(p != nullptr);

    p->node_mask_[mask_index] = mask;

    while (p) {
      p->Maintain();
      p = p->parent_;
    }
  }

  template <int mask_index>
  Node* GetOneConnectedMaskedNode(int u) {
    Node* vertex_u = vertices_[u];
    Node* p = FindRoot(vertex_u);
    while (p) {
      if (p->node_mask_[mask_index] == 1)
        return p;

      if (p->left_ && p->left_->subtree_mask_[mask_index] > 0)
        p = p->left_;
      else if (p->right_ && p->right_->subtree_mask_[mask_index] > 0)
        p = p->right_;
      else
        break;
    }

    return nullptr;
  }

 public:
  DynamicForest(int n) : n_(n), vertices_(n_), tree_edges_(n_) {
    ASSERT(n_ > 0);

    // TODO(backlight): Higher level of graph got few vaild nodes, use hashmap to maintain vertices
    // node on higher level graph may get better space efficiency.
    for (int i = 0; i < n_; ++i)
      vertices_[i] = AllocateNode(i, i);
  }

  ~DynamicForest() {
    for (int i = 0; i < n_; ++i) {
      for (auto [_, e] : tree_edges_[i]) {
        FreeNode(e);
      }
    }
    for (int i = 0; i < n_; ++i) {
      FreeNode(vertices_[i]);
    }
  }

  void Insert(int u, int v) {
    ASSERT(not tree_edges_[u].count(v));
    ASSERT(not tree_edges_[v].count(u));

    Node* vertex_u = vertices_[u];
    Node* vertex_v = vertices_[v];

    Node* edge_uv = AllocateNode(u, v);
    Node* edge_vu = AllocateNode(v, u);
    tree_edges_[u][v] = edge_uv;
    tree_edges_[v][u] = edge_vu;

    int position_u = Treap::GetPosition(vertex_u);
    int position_v = Treap::GetPosition(vertex_v);

    auto [L11, L12] = Treap::SplitUp2(vertex_u);
    auto [L21, L22] = Treap::SplitUp2(vertex_v);

    ASSERT(GetSize(L11) == position_u);
    ASSERT(GetSize(L21) == position_v);

    Node* result = nullptr;
    result = Treap::Merge(result, L12);
    result = Treap::Merge(result, L11);
    result = Treap::Merge(result, edge_uv);
    result = Treap::Merge(result, L22);
    result = Treap::Merge(result, L21);
    result = Treap::Merge(result, edge_vu);
  }

  void Delete(int u, int v) {
    ASSERT(tree_edges_[u].count(v));
    ASSERT(tree_edges_[v].count(u));

    Node* edge_uv = tree_edges_[u][v];
    Node* edge_vu = tree_edges_[v][u];
    tree_edges_[u].erase(v);
    tree_edges_[v].erase(u);

    int position_uv = Treap::GetPosition(edge_uv);
    int position_vu = Treap::GetPosition(edge_vu);
    if (position_uv > position_vu) {
      std::swap(edge_uv, edge_vu);
      std::swap(position_uv, position_vu);
    }

    auto [L1, uv, _] = Treap::SplitUp3(edge_uv);
    ASSERT(GetSize(L1) == position_uv - 1);
    ASSERT(GetSize(uv) == 1);

    auto [L2, vu, L3] = Treap::SplitUp3(edge_vu);
    ASSERT(GetSize(L2) == position_vu - position_uv - 1);
    ASSERT(GetSize(vu) == 1);

    L1 = Treap::Merge(L1, L3);

    FreeNode(edge_uv);
    FreeNode(edge_vu);
  }

  int GetComponentSize(int u) {
    Node* vertex_u = vertices_[u];
    Node* root_of_vertex_u = FindRoot(vertex_u);
    return GetSize(root_of_vertex_u);
  }

  bool IsConnected(int u, int v) {
    Node* vertex_u = vertices_[u];
    Node* vertex_v = vertices_[v];
    return FindRoot(vertex_u) == FindRoot(vertex_v);
  }

  std::pair<bool, int> GetOneConnectedMaskedVertex(int u) {
    Node* p = GetOneConnectedMaskedNode<0>(u);
    if (p == nullptr)
      return {false, -1};
    return {true, p->from_};
  }

  std::tuple<bool, int, int> GetOneConnectedMaskedEdge(int u) {
    Node* p = GetOneConnectedMaskedNode<1>(u);
    if (p == nullptr)
      return {false, -1, -1};
    return {true, p->from_, p->to_};
  }

  void MarkVertex(int u, int mask) {
    Node* p = vertices_[u];
    Mark<0>(p, mask);
  }

  void MarkEdge(int u, int v, int mask) {
    Node* uv = tree_edges_[u][v];
    Mark<1>(uv, mask);
    Node* vu = tree_edges_[v][u];
    Mark<1>(vu, mask);
  }

  std::string to_string() const {
    std::stringstream ss;

    ss << "DynamicForest [\n";

    std::function<void(Node*)> dfs = [&](Node* p) {
      if (!p)
        return;
      dfs(p->left_);
      ss << "(" << p->from_ << "," << p->to_ << "),";
      dfs(p->right_);
    };
    for (int i = 0; i < n_; ++i) {
      if (vertices_[i]->parent_ == nullptr) {
        ss << "  Component [";
        dfs(vertices_[i]);
        ss << "]\n";
      }
    }
    for (int i = 0; i < n_; ++i) {
      for (auto [_, j] : tree_edges_[i]) {
        if (j->parent_ == nullptr) {
          ss << "  Component [";
          dfs(j);
          ss << "]\n";
        }
      }
    }

    ss << "]\n\n";

    return ss.str();
  }

 private:
  int n_;
  std::vector<Node*> vertices_;
  std::vector<std::map<int, Node*>> tree_edges_;
};
std::mt19937 DynamicForest::rng_(std::chrono::steady_clock::now().time_since_epoch().count());

/**
 * Solve Fully Dynamic Conectivity Problem in poly-logarithmic time.
 *
 * @reference:
 *   1. Poly-logarithmic deterministic fully-dynamic algorithms forconnectivity, minimum spanning
 * tree, 2-edge, and biconnectivity. by Holm et al.
 *      https://u.cs.biu.ac.il/~rodittl/p723-holm.pdf
 *   2. An Experimental Study of Poly-Logarithmic Fully-Dynamic Connectivity
 * Algorithms. by Iyer et al.
 *      https://people.csail.mit.edu/rahul/papers/dyncon-jea2001.pdf
 *
 * @efficiency
 *  - Insertion or Deletion: O(\log^2 n)
 *  - Query: O(\log n)
 *  - Space: O(m + n \log n)
 *
 * TODO(backlight):
 *   1. As said in the thesis 1, maintain dynamic sequence using multiple-ary
 * balanced search tree can answer query in O(\frac{\log n}{\log \log n}) time.
 *   2. Thesis 2 propose a 2 heuristics:
 *     2.1. Sampling: Pick O(\log n) non-tree edges randomly as replacement candidate first before
 * the enumeration of non-tree edges.
 *     2.2. Truncating Levels: At a high level, the size of tree is really small so that it is no
 * longer worth doing anything sophisticated. Just use brute force.
 */
class HolmDeLichtenbergThorup {
 private:
  inline int GetLevelOfEdge(int u, int v) {
    if (!edge_level_[u].count(v))
      return -1;

    ASSERT(edge_level_[v].count(u));
    ASSERT(edge_level_[u][v] == edge_level_[v][u]);

    return edge_level_[u][v];
  }

  inline bool EdgeExist(int u, int v) { return GetLevelOfEdge(u, v) != -1; }

  inline void InsertTreeEdge(int u, int v) {
    int level = GetLevelOfEdge(u, v);
    ASSERT(level != -1);

    for (int l = 0; l <= level; ++l) {
      ASSERT(not spanning_forests_[l]->IsConnected(u, v));

      spanning_forests_[l]->Insert(u, v);
    }
    spanning_forests_[level]->MarkEdge(u, v, 1);
  }

  inline void DeleteTreeEdge(int u, int v) {
    int level = GetLevelOfEdge(u, v);
    ASSERT(level != -1);

    for (int l = 0; l <= level; ++l) {
      spanning_forests_[l]->Delete(u, v);
    }
  }

  inline bool IsTreeEdge(int u, int v) {
    int level = GetLevelOfEdge(u, v);
    ASSERT(level != -1);

    return not non_tree_edges_[level][u].count(v);
  }

  inline void UpgradeTreeEdge(int u, int v) {
    int level = edge_level_[u][v];
    ASSERT(level != -1);

    ++edge_level_[u][v], ++edge_level_[v][u];

    spanning_forests_[level]->MarkEdge(u, v, 0);
    spanning_forests_[level + 1]->Insert(u, v);
    spanning_forests_[level + 1]->MarkEdge(u, v, 1);
  }

  inline void InsertNonTreeEdge(int u, int v) {
    int level = GetLevelOfEdge(u, v);
    ASSERT(level != -1);

    if (non_tree_edges_[level][u].empty())
      spanning_forests_[level]->MarkVertex(u, 1);
    non_tree_edges_[level][u].insert(v);

    if (non_tree_edges_[level][v].empty())
      spanning_forests_[level]->MarkVertex(v, 1);
    non_tree_edges_[level][v].insert(u);
  }

  inline void DeleteNonTreeEdge(int u, int v) {
    int level = GetLevelOfEdge(u, v);
    ASSERT(level != -1);

    non_tree_edges_[level][u].erase(v);
    if (non_tree_edges_[level][u].empty())
      spanning_forests_[level]->MarkVertex(u, 0);

    non_tree_edges_[level][v].erase(u);
    if (non_tree_edges_[level][v].empty())
      spanning_forests_[level]->MarkVertex(v, 0);
  }

  inline bool IsNonTreeEdge(int u, int v) {
    int level = GetLevelOfEdge(u, v);
    ASSERT(level != -1);

    return non_tree_edges_[level][u].count(v);
  }

  inline void UpgradeNonTreeEdge(int u, int v) {
    DeleteNonTreeEdge(u, v);
    ++edge_level_[u][v], ++edge_level_[v][u];
    InsertNonTreeEdge(u, v);
  }

  void TryReconnect(int u, int v, int level) {
    DynamicForest* forest = spanning_forests_[level];

    // We are going to search edges inside smaller component.
    int size_u = forest->GetComponentSize(u);
    int size_v = forest->GetComponentSize(v);
    if (size_u > size_v) {
      std::swap(u, v);
    }

    // TODO(backlight): Since the deletion of non-tree edges will not effect the structure of
    // forest, there is no need to find the root of u and v each time.

    // To maintain Invariant (i) mentioned in reference 1 section 3.1, we need to upgrade all tree
    // edges at this level, since we may upgrade non-tree edges later. And tree edges at this level
    // must not be a replacement, so upgrade all tree edges at this level directly.
    while (true) {
      auto [found, x, y] = forest->GetOneConnectedMaskedEdge(u);
      if (not found)
        break;

      ASSERT(GetLevelOfEdge(x, y) == level);

      UpgradeTreeEdge(x, y);

      ASSERT(GetLevelOfEdge(x, y) == level + 1);
    }

    // To search non-tree edges inside component which u belong to(in short U), search a vertex
    // inside U firstly and then search the adajacent list of vertex u. Here we need to find a
    // non-tree edge in O(\log n) time to get armotized O(\log^2 n) time complexity of insertion and
    // deletion.
    //
    // Enumerate all vertices inside U maybe too expensive(seems like O(n)?). When inserting
    // non-tree edges, maintain a for each vertex that is this vertex got at least one non-tree
    // edges incident to it. And then for each node in search trees, maintain a mask that is there
    // at least one masked node inside subtree. And if we use search tree with at most O(\log n)
    // height, we can find a valid vertex in O(\log n) time, and then find a non-tree edge in
    // O(\log n) time.
    while (true) {
      auto [found, x] = forest->GetOneConnectedMaskedVertex(u);
      if (not found)
        break;

      auto& adajacent_vertices = non_tree_edges_[level][x];
      while (!adajacent_vertices.empty()) {
        auto it = adajacent_vertices.begin();
        int y = *it;
        adajacent_vertices.erase(it);

        ASSERT(GetLevelOfEdge(x, y) == level);

        if (forest->IsConnected(y, v)) {
          DeleteNonTreeEdge(x, y);
          InsertTreeEdge(x, y);
          return;
        } else {
          UpgradeNonTreeEdge(x, y);

          ASSERT(non_tree_edges_[level][y].count(x) == 0);
          ASSERT(GetLevelOfEdge(x, y) == level + 1);
          ASSERT(GetLevelOfEdge(y, x) == level + 1);
        }
      }
    }

    if (level - 1 >= 0)
      TryReconnect(u, v, level - 1);
  }

 public:
  HolmDeLichtenbergThorup(int n)
      : n_(n),
        logn_(std::__lg(n_) + 1),
        non_tree_edges_(logn_, std::vector<std::set<int>>(n_)),
        edge_level_(n_),
        spanning_forests_(logn_, nullptr) {
    for (int i = 0; i < logn_; ++i) {
      spanning_forests_[i] = new DynamicForest(n_);
    }
  }

  ~HolmDeLichtenbergThorup() {
    for (int i = 0; i < logn_; ++i) {
      delete spanning_forests_[i];
    }
  }

  void Insert(int u, int v) {
    ASSERT(0 <= u and u < n_ and 0 <= v and v < n_);

    if (u == v) {
      // Self loop, do nothing.
      return;
    }

    if (EdgeExist(u, v)) {
      // Edge (u, v) already exist, do nothing.
      return;
    }

    edge_level_[u][v] = edge_level_[v][u] = 0;
    if (IsConnected(u, v)) {
      InsertNonTreeEdge(u, v);
    } else {
      InsertTreeEdge(u, v);
    }
  }

  void Delete(int u, int v) {
    ASSERT(0 <= u and u < n_ and 0 <= v and v < n_);

    if (u == v) {
      // Do nothing.
      return;
    }

    if (!EdgeExist(u, v)) {
      // Edge does not exist, do nothing.
      return;
    }

    /*
     * If (u, v) is non-tree edge, this deletion will not effect connectivity, just remove it from
     * non-tree edge adajacent list. Otherwise, this deletion may effect connectivity, but it is
     * possible to recover connectivity using non-tree edges. For more details please refer to
     * section 3 of reference 1.
     */
    if (IsNonTreeEdge(u, v)) {
      DeleteNonTreeEdge(u, v);
    } else if (IsTreeEdge(u, v)) {
      DeleteTreeEdge(u, v);
      int level = GetLevelOfEdge(u, v);
      TryReconnect(u, v, level);
    }
    edge_level_[u].erase(v);
    edge_level_[v].erase(u);
  }

  bool IsConnected(int u, int v) {
    ASSERT(0 <= u and u < n_ and 0 <= v and v < n_);

    if (u == v) {
      // Do nothing.
      return true;
    }

    return spanning_forests_[0]->IsConnected(u, v);
  }

  std::string to_string() const {
    std::stringstream ss;

    ss << "HolmDeLichtenbergThorup [\n";
    for (int i = 0; i < logn_; ++i) {
      ss << "Level " << i << " " << spanning_forests_[i]->to_string();
    }
    ss << "]\n\n";

    return ss.str();
  }

 private:
  int n_;
  int logn_;
  std::vector<std::vector<std::set<int>>> non_tree_edges_;
  std::vector<std::map<int, int>> edge_level_;
  std::vector<DynamicForest*> spanning_forests_;
};
using DCP = HolmDeLichtenbergThorup;

void solve_case(int Case) {
  int n, m;
  std::cin >> n >> m;

  DCP d(n + 1);
  int lastans = 0;
  for (int i = 0; i < m; ++i) {
    int op, u, v;
    std::cin >> op >> u >> v;
    u ^= lastans;
    v ^= lastans;
    if (op == 0) {
      d.Insert(u, v);
    } else if (op == 1) {
      d.Delete(u, v);
    } else {
      int connected = d.IsConnected(u, v);
      if (connected)
        lastans = u;
      else
        lastans = v;
      std::cout << (connected ? "Y" : "N") << "\n";
    }
  }
}

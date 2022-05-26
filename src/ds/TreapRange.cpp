// Problem: P3391 【模板】文艺平衡树
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P3391
// Memory Limit: 125 MB
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

template <typename ValueType>
class Treap {
 private:
  typedef int32_t size_t;

  struct Node {
    ValueType value;
    uint32_t priority;
    size_t size;
    Node* left;
    Node* right;

    int rev;
  };
  Node* root_;
  std::mt19937 rng_;

 private:
  Node* NewNode(ValueType value) {
    Node* p = new Node;
    p->value = value;
    p->priority = rng_();
    p->size = 1;
    p->left = p->right = nullptr;

    p->rev = 0;

    return p;
  }

  size_t GetSize(Node* p) { return p ? p->size : 0; }

  void PushDown(Node* p) {
    if (!p)
      return;
    if (p->rev) {
      std::swap(p->left, p->right);

      if (p->left)
        p->left->rev ^= 1;

      if (p->right)
        p->right->rev ^= 1;

      p->rev = 0;
    }
  }

  void PushUp(Node* p) {
    if (!p)
      return;
    p->size = GetSize(p->left) + 1 + GetSize(p->right);
  }

  std::pair<Node*, Node*> SplitSize(Node* p, size_t k) {
    if (!p)
      return {nullptr, nullptr};
    std::pair<Node*, Node*> result;

    PushDown(p);
    if (GetSize(p->left) < k) {
      auto right_result = SplitSize(p->right, k - GetSize(p->left) - 1);
      p->right = right_result.first;

      result.first = p;
      result.second = right_result.second;
    } else {
      auto left_result = SplitSize(p->left, k);
      p->left = left_result.second;

      result.first = left_result.first;
      result.second = p;
    }
    PushUp(p);

    return result;
  }

  Node* Merge(Node* a, Node* b) {
    if (a == nullptr)
      return b;
    else if (b == nullptr)
      return a;

    if (a->priority < b->priority) {
      PushDown(a);
      a->right = Merge(a->right, b);
      PushUp(a);
      return a;
    } else {
      PushDown(b);
      b->left = Merge(a, b->left);
      PushUp(b);
      return b;
    }
  }

 public:
  Treap() : root_(nullptr), rng_(std::chrono::steady_clock::now().time_since_epoch().count()) {}

  ~Treap() {
    std::function<void(Node*)> dfs = [&](Node* p) -> void {
      if (!p)
        return;
      dfs(p->left);
      dfs(p->right);
      freep(p);
    };
    dfs(root_);
  }

  void Insert(const size_t& k, ValueType value) {
    auto [a, b] = SplitSize(root_, k - 1);
    a = Merge(a, NewNode(value));
    root_ = Merge(a, b);
  }

  void Delete(const size_t& k) {
    auto [a, b] = SplitValue(root_, k);
    auto [c, d] = SplitValue(a, k - 1);

    freep(d);

    a = Merge(c, d);
    root_ = Merge(a, b);
  }

  void Build(const std::vector<ValueType>& a) {
    assert(root_ == nullptr);

    std::stack<Node*> stk;
    for (int i = 0; i < (int)a.size(); ++i) {
      Node* p = NewNode(a[i]);
      Node* last = nullptr;
      while (!stk.empty() && stk.top()->priority > p->priority) {
        last = stk.top();
        PushUp(last);
        stk.pop();
      }

      p->left = last;
      if (!stk.empty())
        stk.top()->right = p;

      stk.push(p);
    }

    while (!stk.empty()) {
      PushUp(stk.top());
      root_ = stk.top();
      stk.pop();
    }
  }

  void Reverse(int l, int r) {
    auto [a, b] = SplitSize(root_, l - 1);
    auto [c, d] = SplitSize(b, r - l + 1);

    c->rev ^= 1;

    b = Merge(c, d);
    root_ = Merge(a, b);
  }

  std::vector<ValueType> GetPreOrder() {
    std::vector<int> a(root_->size);
    size_t b = 0;
    std::function<void(Node*)> dfs = [&](Node* p) -> void {
      if (!p)
        return;
      PushDown(p);
      dfs(p->left);
      a[b++] = p->value;
      dfs(p->right);
    };
    dfs(root_);
    return a;
  }
};

void solve_case(int Case) {
  int n, m;
  std::cin >> n >> m;

  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    a[i] = i + 1;
  }

  Treap<int> t;
  t.Build(a);
  logd(t.GetPreOrder());

  for (int i = 0, l, r; i < m; ++i) {
    std::cin >> l >> r;
    logd(l, r);
    t.Reverse(l, r);
    logd(t.GetPreOrder());
  }

  std::vector<int> b = t.GetPreOrder();
  for (int i = 0; i < n; ++i)
    std::cout << b[i] << " \n"[i + 1 == n];
}
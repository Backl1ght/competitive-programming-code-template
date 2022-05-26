// Problem: P6136 【模板】普通平衡树（数据加强版）
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P6136
// Memory Limit: 88 MB
// Time Limit: 3000 ms
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
  freopen(argv[1], "r", stdin);
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
    return p;
  }

  void Free(Node*& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  size_t GetSize(Node* p) { return p ? p->size : 0; }

  void PushUp(Node* p) {
    if (!p)
      return;
    p->size = GetSize(p->left) + 1 + GetSize(p->right);
  }

  std::pair<Node*, Node*> SplitValue(Node* p, const ValueType& value) {
    if (!p)
      return {nullptr, nullptr};

    std::pair<Node*, Node*> result;
    if (p->value <= value) {
      auto right_result = SplitValue(p->right, value);
      p->right = right_result.first;

      result.first = p;
      result.second = right_result.second;
    } else {
      auto left_result = SplitValue(p->left, value);
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

    Node* result;
    if (a->priority < b->priority) {
      result = a;
      a->right = Merge(a->right, b);
      PushUp(a);
    } else {
      result = b;
      b->left = Merge(a, b->left);
      PushUp(b);
    }

    return result;
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

  void Insert(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value);
    a = Merge(a, NewNode(value));
    root_ = Merge(a, b);
  }

  void Delete(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value);
    auto [c, d] = SplitValue(a, value - 1);

    assert(d);

    Node* temp = d;
    d = Merge(d->left, d->right);
    Free(temp);

    a = Merge(c, d);
    root_ = Merge(a, b);
  }

  size_t GetRank(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value - 1);
    size_t result = GetSize(a) + 1;
    root_ = Merge(a, b);
    return result;
  }

  ValueType GetKth(size_t k) {
    assert(k <= GetSize(root_));
    Node* p = root_;

    ValueType result;
    while (p) {
      if (k <= GetSize(p->left)) {
        p = p->left;
      } else if (k == GetSize(p->left) + 1) {
        result = p->value;
        break;
      } else {
        k -= GetSize(p->left) + 1;
        p = p->right;
      }
    }
    return result;
  }

  ValueType GetPrev(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value - 1);

    assert(a != nullptr);
    Node* c = a;
    while (c->right)
      c = c->right;
    ValueType result = c->value;

    root_ = Merge(a, b);

    return result;
  }

  ValueType GetNext(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value);

    assert(b != nullptr);
    Node* c = b;
    while (c->left)
      c = c->left;
    ValueType result = c->value;

    root_ = Merge(a, b);

    return result;
  }
};

void solve_case(int Case) {
  int n, q;
  std::cin >> n >> q;

  Treap<int> t;
  for (int i = 1, x; i <= n; ++i) {
    std::cin >> x;
    t.Insert(x);
  }

  int ans = 0;
  for (int i = 1, op, x, last = 0; i <= q; ++i) {
    std::cin >> op >> x;
    x ^= last;
    logd(op, x);
    // std::cerr << t.ToString() << std::endl;
    switch (op) {
      case 1:
        t.Insert(x);
        break;
      case 2:
        t.Delete(x);
        break;
      case 3:
        last = t.GetRank(x);
        ans ^= last;
        logd(last);
        break;
      case 4:
        last = t.GetKth(x);
        ans ^= last;
        logd(last);
        break;
      case 5:
        last = t.GetPrev(x);
        ans ^= last;
        logd(last);
        break;
      case 6:
        last = t.GetNext(x);
        ans ^= last;
        logd(last);
        break;
    }
    // std::cerr << t.ToString() << std::endl;
  }
  std::cout << ans << "\n";
}
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

int main() {
  CPPIO;
  int T = 1;
  // std::cin >> T;
  for (int t = 1; t <= T; ++t) {
    solve_case(t);
  }
  return 0;
}

template <typename ValueType>
class AVLTree {
 private:
  struct Node {
    ValueType value;
    size_t count;
    size_t size;
    size_t height;
    Node* left;
    Node* right;
    Node() {}
    Node(ValueType value) {
      this->value = value;
      this->count = this->size = 1;
      this->height = 1;
      this->left = this->right = nullptr;
    }
  };

  size_t GetSize(Node* p) { return p ? p->size : 0; }

  size_t GetHeight(Node* p) { return p ? p->height : 0; }

  void Maintain(Node* p) {
    if (!p)
      return;
    p->size = GetSize(p->left) + p->count + GetSize(p->right);
    p->height = std::max(GetHeight(p->left), GetHeight(p->right)) + 1;
  }

  void Zig(Node*& p) {
    Node* r = p->right;
    p->right = r->left;
    r->left = p;
    Maintain(p);
    Maintain(r);
    p = r;
  }

  void Zag(Node*& p) {
    Node* l = p->left;
    p->left = l->right;
    l->right = p;
    Maintain(p);
    Maintain(l);
    p = l;
  }

  void KeepBalance(Node*& p) {
    if (!p)
      return;
    if (GetHeight(p->left) - GetHeight(p->right) == -2) {
      if (p->right && GetSize(p->right->left) > GetSize(p->right->right)) {
        Zag(p->right);
      }
      Zig(p);
    } else if (GetHeight(p->left) - GetHeight(p->right) == 2) {
      if (p->left && GetSize(p->left->left) < GetSize(p->left->right)) {
        Zig(p->left);
      }
      Zag(p);
    }
    Maintain(p);
  }

  void InsertInternal(Node*& p, ValueType value) {
    if (!p) {
      p = new Node(value);
      return;
    }
    if (value < p->value) {
      InsertInternal(p->left, value);
    } else if (value == p->value) {
      p->count++;
    } else {
      InsertInternal(p->right, value);
    }
    Maintain(p);
    KeepBalance(p);
  }

  void DeleteInternal(Node*& p, ValueType value) {
    if (!p)
      return;
    if (value < p->value) {
      DeleteInternal(p->left, value);
    } else if (value == p->value) {
      if (p->left && p->right) {
        Node* replace = p->right;
        while (replace->left)
          replace = replace->left;
        p->value = replace->value;
        p->count = replace->count;
        replace->count = 1;
        DeleteInternal(p->right, replace->value);
      } else {
        Node* temp = p;
        if (p->left)
          p = p->left;
        else if (p->right)
          p = p->right;
        else
          p = nullptr;
        freep(temp);
      }
    } else {
      DeleteInternal(p->right, value);
    }
    Maintain(p);
    KeepBalance(p);
  }

  void DeleteOneInternal(Node*& p, ValueType value) {
    if (!p)
      return;
    if (value < p->value) {
      DeleteOneInternal(p->left, value);
    } else if (value == p->value) {
      --p->count;
      if (p->count == 0) {
        if (p->left && p->right) {
          Node* replace = p->right;
          while (replace->left)
            replace = replace->left;
          p->value = replace->value;
          p->count = replace->count;
          replace->count = 1;
          DeleteInternal(p->right, replace->value);
        } else {
          Node* temp = p;
          if (p->left)
            p = p->left;
          else if (p->right)
            p = p->right;
          else
            p = nullptr;
          freep(temp);
        }
      }
    } else {
      DeleteOneInternal(p->right, value);
    }
    Maintain(p);
    KeepBalance(p);
    Maintain(p);
  }

  ValueType GetKthInternal(Node* p, size_t k) {
    if (k <= 0)
      assert(false);
    if (k <= GetSize(p->left))
      return GetKthInternal(p->left, k);
    else if (k <= GetSize(p->left) + p->count)
      return p->value;
    else
      return GetKthInternal(p->right, k - GetSize(p->left) - p->count);
  }

  size_t GetPrevRank(Node* p, ValueType value) {
    if (!p)
      return 0;
    if (value < p->value) {
      return GetPrevRank(p->left, value);
    } else if (value == p->value) {
      return GetSize(p->left);
    } else {
      return GetSize(p->left) + p->count + GetPrevRank(p->right, value);
    }
  }

  size_t GetRankInternal(Node* p, ValueType value) { return GetPrevRank(p, value) + 1; }

  size_t GetNextRank(Node* p, ValueType value) {
    if (!p)
      return 1;
    if (value < p->value) {
      return GetNextRank(p->left, value);
    } else if (value == p->value) {
      return GetSize(p->left) + p->count + 1;
    } else {
      return GetSize(p->left) + p->count + GetNextRank(p->right, value);
    }
  }

 private:
  Node* root_;

 public:
  AVLTree() : root_(nullptr) {}

  ~AVLTree() {
    std::function<void(Node*)> dfs = [&](Node* p) -> void {
      if (!p)
        return;
      dfs(p->left);
      dfs(p->right);
      freep(p);
    };
    dfs(root_);
  }

  void Insert(ValueType value) { InsertInternal(root_, value); }

  void Delete(ValueType value) { DeleteInternal(root_, value); }

  void DeleteOne(ValueType value) { DeleteOneInternal(root_, value); }

  size_t GetRank(ValueType value) { return GetRankInternal(root_, value); }

  ValueType GetKth(size_t k) { return GetKthInternal(root_, k); }

  ValueType GetPrev(ValueType value) {
    size_t prev_rank = GetPrevRank(root_, value);
    return GetKthInternal(root_, prev_rank);
  }

  ValueType GetNext(ValueType value) {
    size_t next_rank = GetNextRank(root_, value);
    return GetKthInternal(root_, next_rank);
  }

  std::string ToString() {
    std::string result;
    std::function<void(Node*)> dfs = [&](Node* p) {
      if (!p)
        return;
      dfs(p->left);
      result += "(" + std::to_string(p->value) + "," + std::to_string(p->count) + "), ";
      dfs(p->right);
    };
    dfs(root_);
    return result;
  }
};

void solve_case(int Case) {
  int n, q;
  std::cin >> n >> q;

  AVLTree<int> t;
  for (int i = 1, x; i <= n; ++i) {
    std::cin >> x;
    t.Insert(x);
  }

  int ans = 0;
  for (int i = 1, op, x, last = 0; i <= q; ++i) {
    // std::cerr << t.ToString() << std::endl;
    std::cin >> op >> x;
    x ^= last;
    switch (op) {
      case 1:
        t.Insert(x);
        break;
      case 2:
        t.DeleteOne(x);
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
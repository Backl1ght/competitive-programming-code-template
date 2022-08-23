// Problem: P6136 【模板】普通平衡树（数据加强版）
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P6136
// Memory Limit: 88 MB
// Time Limit: 3000 ms
//
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>

#define CPPIO std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
#define freep(parent) parent ? delete parent, parent = nullptr, void(1) : void(0)

#ifdef BACKLIGHT
#include "debug.h"
#else
#define logd(...) ;
#define serialize(...) ""
#define ASSERT(x) ;
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

/**
 * Splay Tree(aka Self-Adjusting Binary Search Trees).
 *
 * @reference: Self-adjusting binary search trees, by Sleator and Tarjan.
 *             https://www.cs.cmu.edu/~sleator/papers/self-adjusting.pdf
 */
template <typename ValueType>
class SplayValue {
 public:
  struct Node {
    Node* left_;
    Node* right_;
    Node* parent_;

    int count_;
    int size_;

    ValueType value_;

    Node(const ValueType& value) : value_(value) {
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      size_ = 1;
      count_ = 1;
    }

    inline bool IsLeftSon() { return parent_->left_ == this; }
  };

 private:
  inline Node* AllocateNode(const ValueType& value) {
    Node* p = new Node(value);
    return p;
  }

  inline void FreeNode(Node*& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  inline int GetSize(Node* p) { return p ? p->size_ : 0; }

  inline void Maintain(Node* p) {
    if (p) {
      p->size_ = GetSize(p->left_) + p->count_ + GetSize(p->right_);
    }
  }

  inline void LeftRotate(Node* p) {
    Node* father = p->parent_;
    Node* grandfather = father->parent_;
    Node* left_son_of_p = p->left_;

    p->parent_ = grandfather;
    if (grandfather) {
      if (father->IsLeftSon())
        grandfather->left_ = p;
      else
        grandfather->right_ = p;
    }

    p->left_ = father;
    father->parent_ = p;

    father->right_ = left_son_of_p;
    if (left_son_of_p)
      left_son_of_p->parent_ = father;

    Maintain(father);
    Maintain(p);
  }

  inline void RightRotate(Node* p) {
    Node* father = p->parent_;
    Node* grandfather = father->parent_;
    Node* right_son_of_p = p->right_;

    p->parent_ = grandfather;
    if (grandfather) {
      if (father->IsLeftSon())
        grandfather->left_ = p;
      else
        grandfather->right_ = p;
    }

    p->right_ = father;
    father->parent_ = p;

    father->left_ = right_son_of_p;
    if (right_son_of_p)
      right_son_of_p->parent_ = father;

    Maintain(father);
    Maintain(p);
  }

  inline void Rotate(Node* p) {
    ASSERT(p->parent_ != nullptr);

    Node* father = p->parent_;
    Node* grandfather = father->parent_;

    // Case 1: zig.
    if (grandfather == nullptr) {
      if (p->IsLeftSon())
        RightRotate(p);
      else
        LeftRotate(p);
      return;
    }

    // Case 2: zig-zig.
    if (p->IsLeftSon() == father->IsLeftSon()) {
      if (p->IsLeftSon()) {
        RightRotate(father);
        RightRotate(p);
      } else {
        LeftRotate(father);
        LeftRotate(p);
      }
      return;
    }

    // Case 3: zig-zag.
    if (p->IsLeftSon()) {
      RightRotate(p);
      LeftRotate(p);
    } else {
      LeftRotate(p);
      RightRotate(p);
    }
  }

  void Splay(Node* p) {
    if (p == nullptr)
      return;

    while (p->parent_ != nullptr) {
      Rotate(p);
    }
    root_ = p;
  }

  Node* Find(const ValueType& value) {
    Node* p = root_;
    while (p) {
      if (p->value_ == value) {
        break;
      } else if (value < p->value_) {
        if (p->left_ == nullptr) {
          Splay(p);
          return nullptr;
        }
        p = p->left_;
      } else {
        if (p->right_ == nullptr) {
          Splay(p);
          return nullptr;
        }
        p = p->right_;
      }
    }

    return p;
  }

 public:
  SplayValue() : root_(nullptr) {}

  ~SplayValue() {
    std::function<void(Node*)> dfs = [&](Node* p) {
      if (!p)
        return;
      dfs(p->left_);
      dfs(p->right_);
      FreeNode(p);
    };
    dfs(root_);
  }

  void Insert(const ValueType& value) {
    if (root_ == nullptr) {
      root_ = AllocateNode(value);
      return;
    }

    Node* p = root_;
    while (p) {
      ++p->size_;

      if (p->value_ == value) {
        ++p->count_;
        break;
      } else if (value < p->value_) {
        if (p->left_ == nullptr) {
          p->left_ = AllocateNode(value);
          p->left_->parent_ = p;
          p = p->left_;
          break;
        }
        p = p->left_;
      } else {
        if (p->right_ == nullptr) {
          p->right_ = AllocateNode(value);
          p->right_->parent_ = p;
          p = p->right_;
          break;
        }
        p = p->right_;
      }
    }

    ASSERT(p);

    Splay(p);
  }

  void Delete(const ValueType& value) {
    Node* p = Find(value);
    if (p->value_ != value)
      return;
    Splay(p);

    if (root_->count_ > 1) {
      --root_->count_;
      --root_->size_;
      return;
    }

    if (root_->left_ == nullptr && root_->right_ == nullptr) {
      FreeNode(root_);
      return;
    }

    if (root_->left_ == nullptr || root_->right_ == nullptr) {
      Node* temp = root_;
      root_ = (root_->left_ == nullptr ? root_->right_ : root_->left_);
      FreeNode(temp);
      root_->parent_ = nullptr;
      return;
    }

    Node* temp = root_;
    Node* replacement = root_->left_;
    while (replacement->right_)
      replacement = replacement->right_;
    Splay(replacement);

    ASSERT(temp->left_ == nullptr);
    root_->right_ = temp->right_;
    if (temp->right_)
      temp->right_->parent_ = root_;
    --root_->size_;
    FreeNode(temp);
  }

  int GetRank(const ValueType& value) {
    int rank = 0;

    Node* p = root_;
    while (p) {
      if (value == p->value_) {
        rank += GetSize(p->left_);
        break;
      } else if (value < p->value_) {
        if (p->left_ == nullptr) {
          break;
        }
        p = p->left_;
      } else {
        rank += GetSize(p->left_) + p->count_;

        if (p->right_ == nullptr) {
          break;
        }
        p = p->right_;
      }
    }
    Splay(p);

    return rank + 1;
  }

  const ValueType GetKth(int k) {
    ASSERT(k > 0);
    ASSERT(GetSize(root_) >= k);
    Node* p = root_;
    while (p) {
      if (k <= GetSize(p->left_)) {
        p = p->left_;
      } else if (k <= GetSize(p->left_) + p->count_) {
        break;
      } else {
        k -= GetSize(p->left_) + p->count_;
        p = p->right_;
      }
    }
    if (!p) {
      logd(this->to_string(), k);
    }
    ASSERT(p);

    Splay(p);
    return root_->value_;
  }

  const ValueType GetPrev(const ValueType& value) {
    ValueType result = value;

    Node* p = root_;
    while (p) {
      if (value <= p->value_) {
        if (p->left_ == nullptr) {
          Splay(p);
          break;
        }
        p = p->left_;
      } else {
        if (result == value)
          result = p->value_;
        else
          result = std::max(result, p->value_);

        if (p->right_ == nullptr) {
          Splay(p);
          break;
        }
        p = p->right_;
      }
    }

    return result;
  }

  const ValueType GetNext(const ValueType& value) {
    ValueType result = value;

    Node* p = root_;
    while (p) {
      if (value < p->value_) {
        if (result == value)
          result = p->value_;
        else
          result = std::min(result, p->value_);

        if (p->left_ == nullptr) {
          Splay(p);
          break;
        }
        p = p->left_;
      } else {
        if (p->right_ == nullptr) {
          Splay(p);
          break;
        }
        p = p->right_;
      }
    }

    return result;
  }

  std::string to_string() {
    std::stringstream ss;
    ss << "Splay: [";

    std::function<void(Node*)> dfs = [&](Node* p) {
      if (p == nullptr)
        return;

      dfs(p->left_);
      ss << "(" << serialize(p->value_) << "," << p->count_ << ")"
         << ", ";
      dfs(p->right_);
    };
    dfs(root_);

    ss << "]\n";

    return ss.str();
  }

 private:
  Node* root_;
};

void solve_case(int Case) {
  int n, q;
  std::cin >> n >> q;

  SplayValue<int> t;
  for (int i = 1, x; i <= n; ++i) {
    std::cin >> x;
    t.Insert(x);
  }

  int ans = 0;
  for (int i = 1, op, x, last = 0; i <= q; ++i) {
    std::cin >> op >> x;
    x ^= last;
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
        break;
      case 4:
        last = t.GetKth(x);
        ans ^= last;
        break;
      case 5:
        last = t.GetPrev(x);
        ans ^= last;
        break;
      case 6:
        last = t.GetNext(x);
        ans ^= last;
        break;
    }
  }
  printf("%d\n", ans);
}
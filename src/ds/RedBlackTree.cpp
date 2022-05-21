// Problem: P6136 【模板】普通平衡树（数据加强版）
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P6136
// Memory Limit: 88 MB
// Time Limit: 3000 ms
//
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>

#define CPPIO std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
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
class RedBlackTree {
 private:
  typedef bool ColorType;
  const static ColorType RED = false;
  const static ColorType BLACK = true;

  struct Node {
    ValueType value;
    ColorType color;
    size_t count;
    size_t size;
    Node* left;
    Node* right;
    Node* father;

    Node* GrandFather() {
      if (!father)
        return nullptr;
      return father->father;
    }

    Node* Sibling() {
      if (!father)
        return nullptr;
      if (this == father->left)
        return father->right;
      return father->left;
    }

    Node* Uncle() {
      if (!father)
        return nullptr;
      return father->Sibling();
    }
  };

  Node* NewNode(ValueType value, ColorType color) {
    Node* p = new Node;
    p->value = value;
    p->color = color;
    p->count = 1;
    p->size = 1;
    p->father = nullptr;
    p->left = NIL;
    p->right = NIL;
    return p;
  }

  void Free(Node*& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  size_t GetSize(Node* p) { return p ? p->size : 0; }

 private:
  void Maintain(Node* p) {
    if (!p || p == NIL)
      return;
    p->size = GetSize(p->left) + p->count + GetSize(p->right);
  }

  void Zig(Node* p) {
    if (p->father == nullptr) {
      root_ = p;
      return;
    }
    Node* gp = p->GrandFather();
    Node* fa = p->father;
    Node* y = p->left;

    fa->right = y;

    if (y != NIL)
      y->father = fa;
    p->left = fa;
    fa->father = p;

    if (root_ == fa)
      root_ = p;
    p->father = gp;

    if (gp != nullptr) {
      if (gp->left == fa)
        gp->left = p;
      else
        gp->right = p;
    }
    Maintain(fa);
    Maintain(p);
  }

  void Zag(Node* p) {
    Node* gp = p->GrandFather();
    Node* fa = p->father;
    Node* y = p->right;

    fa->left = y;

    if (y != NIL)
      y->father = fa;
    p->right = fa;
    fa->father = p;

    if (root_ == fa)
      root_ = p;
    p->father = gp;

    if (gp != nullptr) {
      if (gp->left == fa)
        gp->left = p;
      else
        gp->right = p;
    }
    Maintain(fa);
    Maintain(p);
  }

 private:
  Node* root_;
  Node* NIL;

 public:
  RedBlackTree() : root_(nullptr) {
    NIL = new Node;
    NIL->color = BLACK;
  }

  void InsertCase(Node* p) {
    // case 1
    if (p->father == nullptr) {
      p->color = BLACK;
      return;
    }

    // case 2
    if (p->father->color == BLACK) {
      return;
    }

    // case 3
    if (p->Uncle()->color == RED) {
      p->father->color = BLACK;
      p->Uncle()->color = BLACK;
      p->GrandFather()->color = RED;
      InsertCase(p->GrandFather());
    } else {
      // TODO: spilit these into case 4 and case 5
      if (p->father->right == p && p->GrandFather()->left == p->father) {
        Zig(p);
        p->color = BLACK;
        p->father->color = RED;
        Zag(p);
      } else if (p->father->left == p && p->GrandFather()->right == p->father) {
        Zag(p);
        p->color = BLACK;
        p->father->color = RED;
        Zig(p);
      } else if (p->father->left == p && p->GrandFather()->left == p->father) {
        p->father->color = BLACK;
        p->GrandFather()->color = RED;
        Zag(p->father);
      } else if (p->father->right == p && p->GrandFather()->right == p->father) {
        p->father->color = BLACK;
        p->GrandFather()->color = RED;
        Zig(p->father);
      }
    }
  }

  void Insert(ValueType value) {
    if (!root_) {
      root_ = NewNode(value, BLACK);
      return;
    }

    Node* p = root_;
    while (p != NIL) {
      ++p->size;

      if (value < p->value) {
        if (p->left != NIL) {
          p = p->left;
        } else {
          p->left = NewNode(value, RED);
          p->left->father = p;
          InsertCase(p->left);
          break;
        }
      } else if (value == p->value) {
        ++p->count;
        break;
      } else {
        if (p->right != NIL) {
          p = p->right;
        } else {
          p->right = NewNode(value, RED);
          p->right->father = p;
          InsertCase(p->right);
          break;
        }
      }
    }
  }

  void DeleteCase(Node* p) {
    if (p->father == nullptr) {
      p->color = BLACK;
      return;
    }

    // case 2
    if (p->Sibling()->color == RED) {
      p->father->color = RED;
      p->Sibling()->color = BLACK;
      if (p == p->father->left)
        Zig(p->Sibling());
      else
        Zag(p->Sibling());
    }

    if (p->father->color == BLACK && p->Sibling()->color == BLACK &&
        p->Sibling()->left->color == BLACK && p->Sibling()->right->color == BLACK) {
      // case 3
      p->Sibling()->color = RED;
      DeleteCase(p->father);
    } else if (p->father->color == RED && p->Sibling()->color == BLACK &&
               p->Sibling()->left->color == BLACK && p->Sibling()->right->color == BLACK) {
      // case 4
      p->Sibling()->color = RED;
      p->father->color = BLACK;
    } else {
      // case 5
      if (p->Sibling()->color == BLACK) {
        if (p == p->father->left && p->Sibling()->left->color == RED &&
            p->Sibling()->right->color == BLACK) {
          p->Sibling()->color = RED;
          p->Sibling()->left->color = BLACK;
          Zag(p->Sibling()->left);
        } else if (p == p->father->right && p->Sibling()->left->color == BLACK &&
                   p->Sibling()->right->color == RED) {
          p->Sibling()->color = RED;
          p->Sibling()->right->color = BLACK;
          Zig(p->Sibling()->right);
        }
      }

      // case 6
      p->Sibling()->color = p->father->color;
      p->father->color = BLACK;
      if (p == p->father->left) {
        p->Sibling()->right->color = BLACK;
        Zig(p->Sibling());
      } else {
        p->Sibling()->left->color = BLACK;
        Zag(p->Sibling());
      }
    }
  }

  void DeleteOneChild(Node* p) {
    Node* child = p->left == NIL ? p->right : p->left;

    if (p->father == nullptr && child == NIL) {
      Free(p);
      root_ = nullptr;
      return;
    }

    if (p->father == nullptr) {
      Free(p);
      child->father = nullptr;
      root_ = child;
      root_->color = BLACK;
      return;
    }

    if (p->father->left == p) {
      p->father->left = child;
    } else {
      p->father->right = child;
    }
    child->father = p->father;

    if (p->color == BLACK) {
      if (child->color == RED) {
        child->color = BLACK;
      } else
        DeleteCase(child);
    }

    Free(p);
  }

  void Delete(ValueType value) {
    Node* p = root_;
    while (p != NIL) {
      --p->size;

      if (value < p->value) {
        p = p->left;
      } else if (value == p->value) {
        --p->count;
        if (p->count == 0) {
          if (p->left != NIL && p->right != NIL) {
            Node* replace = p->right;
            while (replace->left != NIL) {
              replace = replace->left;
            }

            Node* temp = p->right;
            while (temp->left != NIL) {
              temp->size -= replace->count;
              temp = temp->left;
            }

            p->value = replace->value;
            p->count = replace->count;

            DeleteOneChild(replace);
          } else {
            DeleteOneChild(p);
          }
        }
        break;
      } else {
        p = p->right;
      }
    }
  }

  size_t GetRank(ValueType value) {
    if (!root_)
      return 1;
    size_t rank = 0;
    Node* p = root_;
    while (p != NIL) {
      if (value < p->value) {
        p = p->left;
      } else if (value == p->value) {
        rank += GetSize(p->left);
        break;
      } else {
        rank += GetSize(p->left) + p->count;
        p = p->right;
      }
    }
    rank = rank + 1;
    return rank;
  }

  ValueType GetKth(size_t k) {
    Node* p = root_;
    ValueType result;
    while (p != NIL) {
      if (k <= GetSize(p->left)) {
        p = p->left;
      } else if (k <= GetSize(p->left) + p->count) {
        result = p->value;
        break;
      } else {
        k -= GetSize(p->left) + p->count;
        p = p->right;
      }
    }
    return result;
  }

  ValueType GetPrev(ValueType value) {
    ValueType result = std::numeric_limits<ValueType>::min();
    Node* p = root_;
    while (p != NIL) {
      if (value < p->value) {
        p = p->left;
      } else if (value == p->value) {
        p = p->left;
        if (p != NIL) {
          while (p->right != NIL)
            p = p->right;
          result = p->value;
        }
        break;
      } else {
        result = std::max(result, p->value);
        p = p->right;
      }
    }
    return result;
  }

  ValueType GetNext(ValueType value) {
    ValueType result = std::numeric_limits<ValueType>::max();
    Node* p = root_;
    while (p != NIL) {
      if (value < p->value) {
        result = std::min(result, p->value);
        p = p->left;
      } else if (value == p->value) {
        p = p->right;
        if (p != NIL) {
          while (p->left != NIL)
            p = p->left;
          result = p->value;
        }
        break;
      } else {
        p = p->right;
      }
    }
    return result;
  }

  std::string ToString() {
    std::string result = "";
    std::function<void(Node*)> dfs = [&](Node* p) {
      if (p == NIL)
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

  RedBlackTree<int> t;
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
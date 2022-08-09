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
 * Implementation of B-Tree whose order is 2D - 1.
 *
 * Both insertion and deletion use the strategy that before making operation, if
 * the operation may break the rules of b-tree, adjust the tree firstly so that
 * further operation inside the tree will not break the rules of b-tree, and
 * then finish the operation.
 *
 * TODO(backlight):
 * - It seems like insertion and deletion is ok, finish other method.
 * - The maintainance of size of subtrees can be improved. It is
 * not necessary to re-calculate the size of subtrees each time.
 * - There are servaral memory copy operations, use apis like memcpy or memmove
 * instead of native loop may be more effective.
 * - Increase or decrease the height of tree only when needed maybe more
 * effective.
 */
template <typename ValueType, int D>
class BTree {
 public:
  using ElementType = std::pair<ValueType, int>;

 private:
  /**
   * Tree node of BTree, with layout looked like below:
   *   keys :    | k0 |    | k1 |    | k2 |
   *   child: c0 |    | c1 |    | c2 |    | c3
   */
  struct Node {
    Node* parent_;

    int num_keys_;
    ElementType keys_[2 * D - 1];
    Node* child_[2 * D];

    bool is_leaf_;

    int size_;

    Node() {
      parent_ = nullptr;

      num_keys_ = 0;
      for (int i = 0; i < 2 * D; ++i)
        child_[i] = nullptr;

      is_leaf_ = true;

      size_ = 0;
    }

    ~Node() {
      if (num_keys_ > 0) {
        for (int i = 0; i <= num_keys_; ++i) {
          freep(child_[i]);
        }
      }
    }

    void MaintainSize() {
      size_ = 0;
      for (int i = 0; i < num_keys_; ++i)
        size_ += keys_[i].second;
      if (num_keys_ > 0) {
        for (int i = 0; i <= num_keys_; ++i)
          size_ += GetSize(child_[i]);
      }
    }

    std::string to_string() {
      std::stringstream ss;

      ss << "  Node " << (void*)(this) << " [\n";

      ss << "    SIZE: " << size_ << ",\n";
      ss << "    TYPE: " << (is_leaf_ ? "leaf" : "internal") << ",\n";

      ss << "    NUM_KEYS: " << num_keys_ << ",\n";
      ss << "    KEY: [";
      for (int i = 0; i < num_keys_; ++i) {
        ss << "(" << keys_[i].first << "," << keys_[i].second << "),";
      }
      ss << "],\n";

      ss << "    CHILD: [";
      for (int i = 0; i <= num_keys_; ++i) {
        ss << (void*)(child_[i]) << ",";
      }
      ss << "]\n";

      ss << "  ],\n";

      return ss.str();
    }
  };

  inline static int GetSize(Node* p) {
    if (p == nullptr)
      return 0;
    return p->size_;
  }

  /**
   * Create a new node.
   *
   * In case of disgusting problem with strict time limit, it could easily adapt to stategy that
   * pre-allocate a large buffer as a pool to reduce time cost on memory allocation.
   */
  inline Node* CreateNode() { return new Node(); }

 private:
  /**
   * Right shift a suffix of array.
   */
  template <typename T>
  void RightShiftByOne(T* arr, int begin, int end) {
    for (int i = end; i >= begin + 1; --i) {
      arr[i] = arr[i - 1];
    }
  }

  /**
   * Right shift a suffix of array.
   */
  template <typename T>
  void LeftShiftByOne(T* arr, int begin, int end) {
    for (int i = begin - 1; i < end; ++i) {
      arr[i] = arr[i + 1];
    }
  }

  /**
   * Get position of value in node p.
   *
   * @return If value exist as a key of node p, then return (true, index of key
   * in p->keys_). Otherwise return (false, index of child which value belong
   * to).
   */
  std::pair<bool, int> GetPosition(Node* p, const ValueType& value) {
    if (p->num_keys_ == 0)
      return std::make_pair(false, 0);

    int position = std::lower_bound(p->keys_, p->keys_ + p->num_keys_, value,
                                    [](const ElementType& element, const ValueType& value) {
                                      return element.first < value;
                                    }) -
                   p->keys_;
    bool value_exist = (position < p->num_keys_ && value == p->keys_[position].first);
    return std::make_pair(value_exist, position);
  }

  /**
   * Split p into two nodes and a single key.
   *
   * - The number of keys in node p should be 2 * D - 1.
   * - The number of keys in node l and node r would be D - 1.
   */
  std::tuple<ElementType, Node*, Node*> Split(Node* p) {
    assert(p != nullptr);
    assert(p->num_keys_ == 2 * D - 1);

    ElementType key = p->keys_[D - 1];
    Node* l = CreateNode();
    Node* r = CreateNode();

    // TODO: Set parent of l and r to p->parent_ maybe better.
    l->parent_ = r->parent_ = nullptr;

    l->num_keys_ = r->num_keys_ = D - 1;
    for (int i = 0; i < D - 1; ++i) {
      l->keys_[i] = p->keys_[i];
      r->keys_[i] = p->keys_[D + i];
    }
    for (int i = 0; i < D; ++i) {
      l->child_[i] = p->child_[i];
      r->child_[i] = p->child_[D + i];
    }

    l->is_leaf_ = r->is_leaf_ = p->is_leaf_;

    l->MaintainSize();
    r->MaintainSize();

    p->num_keys_ = -1;
    freep(p);

    return std::make_tuple(key, l, r);
  }

  /**
   * Insert value to tree p recursively. Node p will has enough room for the new
   * value based on the insertion strategy.
   *
   * If it is going to insert value to a full node, split the node first to
   * make enough room for the new value, and then make insertion.
   */
  void InsertNonFull(Node* p, const ValueType& value) {
    assert(p != nullptr);

    auto [value_exist, position] = GetPosition(p, value);

    if (value_exist) {
      ++p->keys_[position].second;
    } else {
      if (p->is_leaf_) {
        RightShiftByOne(p->keys_, position, p->num_keys_);
        p->keys_[position] = std::make_pair(value, 1);
        ++p->num_keys_;
      } else {
        if (p->child_[position]->num_keys_ == 2 * D - 1) {
          auto [new_key, l, r] = Split(p->child_[position]);
          RightShiftByOne(p->keys_, position, p->num_keys_);
          RightShiftByOne(p->child_, position + 1, p->num_keys_ + 1);
          p->child_[position] = l;
          p->keys_[position] = new_key;
          p->child_[position + 1] = r;
          ++p->num_keys_;
          l->parent_ = r->parent_ = p;

          if (value == p->keys_[position].first) {
            ++p->keys_[position].second;
          } else if (value > p->keys_[position].first) {
            ++position;
            InsertNonFull(p->child_[position], value);
          } else {
            InsertNonFull(p->child_[position], value);
          }
        } else {
          InsertNonFull(p->child_[position], value);
        }
      }
    }

    p->MaintainSize();
  }

  /**
   * Merge two nodes with D - 1 keys and a single key into one nodes with 2D - 1
   * keys.
   *
   * - The size of node a and node b should be D - 1.
   * - Keys in a should be smaller than keys in b.
   */
  Node* Merge(const ElementType& key, Node* a, Node* b) {
    assert(a != nullptr);
    assert(a->num_keys_ == D - 1);
    assert(b != nullptr);
    assert(a->num_keys_ == D - 1);

    Node* p = CreateNode();
    {
      p->parent_ = nullptr;

      p->num_keys_ = 2 * D - 1;
      p->keys_[D - 1] = key;
      for (int i = 0; i < D - 1; ++i) {
        p->keys_[i] = a->keys_[i];
        p->keys_[D + i] = b->keys_[i];
      }
      for (int i = 0; i < D; ++i) {
        p->child_[i] = a->child_[i];
        p->child_[D + i] = b->child_[i];
      }

      p->is_leaf_ = a->is_leaf_;

      p->MaintainSize();
    }

    a->num_keys_ = -1;
    freep(a);
    b->num_keys_ = -1;
    freep(b);

    return p;
  }

  /**
   * Delete value from tree p recursively. Delete keys in p will not break the
   * rules based on the deletion strategy.
   *
   * If it is going to delete value from node with D - 1 keys, borrow one node
   * from brother or merge two nodes to ensure that deletion will not break the
   * rules, and then make deletion.
   */
  void Delete(Node* p, const ValueType& value) {
    auto [value_exist, position] = GetPosition(p, value);

    if (value_exist) {
      if (p->keys_[position].second > 1) {
        --p->keys_[position].second;
      } else {
        if (p->is_leaf_) {
          // Node p is a leaf, and based on the deletion strategy, it is safe to
          // delete the key directly.
          LeftShiftByOne(p->keys_, position + 1, p->num_keys_);
          --p->num_keys_;
        } else {
          // Need borrow or merge.
          //
          // If the previous child or the next child has enough node, just
          // borrow one key from it, that is, like deletion in BST, find a
          // replacement(pioneer or succssor) and replace, and then delete
          // recursively.
          //
          // Otherwise merge these two nodes and the key to be
          // deleted into one node. Now it is safe to delete key in the new
          // node.
          if (p->child_[position]->num_keys_ > D - 1) {
            // Use pioneer as replacement.
            Node* np = p->child_[position];
            while (!np->is_leaf_)
              np = np->child_[np->num_keys_];
            std::swap(p->keys_[position], np->keys_[np->num_keys_ - 1]);

            Delete(p->child_[position], value);
          } else if (p->child_[position + 1]->num_keys_ > D - 1) {
            // Use successor as replacement.
            Node* np = p->child_[position + 1];
            while (!np->is_leaf_)
              np = np->child_[0];
            std::swap(p->keys_[position], np->keys_[0]);

            Delete(p->child_[position + 1], value);
          } else {
            // Merge.
            Node* np = Merge(p->keys_[position], p->child_[position], p->child_[position + 1]);
            LeftShiftByOne(p->keys_, position + 1, p->num_keys_);
            p->child_[position] = np;
            LeftShiftByOne(p->child_, position + 2, p->num_keys_ + 1);
            --p->num_keys_;

            if (root_->num_keys_ == 0) {
              root_ = p->child_[position];
              root_->parent_ = nullptr;
            }

            Delete(p->child_[position], value);
          }
        }
      }
    } else {
      if (p->is_leaf_) {
        // value does not exist, do nothing.
      } else {
        if (p->child_[position]->num_keys_ > D - 1) {
          // Safe to make deletion.
          Delete(p->child_[position], value);
        } else {
          // Need borrow or merge.

          if (position > 0 && p->child_[position - 1]->num_keys_ > D - 1) {
            // Borrow one key from previous child, like right rotate.
            // That is, use the largest value inside the previous child as key,
            // and then insert the old key to itself. (Note that it is largest value inside node,
            // not subtree).
            Node* np = p->child_[position - 1];
            ElementType new_key = np->keys_[np->num_keys_ - 1];
            Node* rightmost_child = np->child_[np->num_keys_];
            --np->num_keys_;

            std::swap(p->keys_[position], new_key);
            np->MaintainSize();

            RightShiftByOne(p->child_[position]->keys_, 0, p->child_[position]->num_keys_);
            RightShiftByOne(p->child_[position]->child_, 0, p->child_[position]->num_keys_ + 1);
            p->child_[position]->keys_[0] = new_key;
            p->child_[position]->child_[0] = rightmost_child;
            ++p->child_[position]->num_keys_;

            Delete(p->child_[position], value);
          } else if (position < p->num_keys_ && p->child_[position + 1]->num_keys_ > D - 1) {
            // Borrow one key from next child, like left rotate.
            // That is, use the smallest value inside the next child as key, and
            // then insert the old key to itself. (Note that it is smallest value inside node, not
            // subtree).
            Node* np = p->child_[position + 1];
            ElementType new_key = np->keys_[0];
            Node* leftmost_child = np->child_[0];
            LeftShiftByOne(np->keys_, 1, np->num_keys_);
            LeftShiftByOne(np->child_, 1, np->num_keys_ + 1);
            --np->num_keys_;

            std::swap(p->keys_[position], new_key);
            np->MaintainSize();

            p->child_[position]->keys_[p->child_[position]->num_keys_] = new_key;
            p->child_[position]->child_[p->child_[position]->num_keys_ + 1] = leftmost_child;
            ++p->child_[position]->num_keys_;

            Delete(p->child_[position], value);
          } else {
            // Merge any of nearby child. For convinence, assume that the
            // previous has higher priority for merge.
            position = position == 0 ? position : position - 1;
            Node* np = Merge(p->keys_[position], p->child_[position], p->child_[position + 1]);
            LeftShiftByOne(p->keys_, position + 1, p->num_keys_);
            p->child_[position] = np;
            LeftShiftByOne(p->child_, position + 2, p->num_keys_ + 1);
            --p->num_keys_;

            if (root_->num_keys_ == 0) {
              root_ = p->child_[position];
              root_->parent_ = nullptr;
            }

            Delete(p->child_[position], value);
          }
        }
      }
    }

    p->MaintainSize();
  }

 public:
  BTree() : root_(nullptr) {}

  BTree(const BTree&) = delete;

  BTree(BTree&&) = delete;

  ~BTree() { freep(root_); }

  void Insert(const ValueType& value) {
    if (root_ == nullptr) {
      root_ = CreateNode();
      root_->parent_ = nullptr;
      root_->num_keys_ = 1;
      root_->keys_[0] = std::make_pair(value, 1);
      root_->is_leaf_ = true;
      root_->MaintainSize();
      return;
    }

    // If the root_ is full, this insertion may increase the height of
    // tree. To avoid corner case, just increase the height of tree first.
    if (root_->num_keys_ == 2 * D - 1) {
      auto [new_key, l, r] = Split(root_);

      Node* new_root = CreateNode();
      new_root->parent_ = nullptr;
      new_root->num_keys_ = 1;
      new_root->keys_[0] = new_key;
      new_root->child_[0] = l;
      new_root->child_[1] = r;
      new_root->is_leaf_ = false;
      new_root->MaintainSize();

      l->parent_ = r->parent_ = new_root;

      root_ = new_root;
    }

    InsertNonFull(root_, value);
  }

  void Delete(const ValueType& value) {
    if (root_ == nullptr)
      return;
    Delete(root_, value);
  }

  size_t GetRank(const ValueType& value) { return 0; }

  ValueType GetKth(const size_t& k) { return ValueType(0); }

  ValueType GetPrev(const ValueType& value) { return ValueType(0); }

  ValueType GetNext(const ValueType& value) { return ValueType(0); }

  std::string to_string(Node* p) const {
    std::stringstream ss;
    ss << "BTree: [\n";

    std::function<void(Node*)> dfs = [&](Node* p) {
      if (p == nullptr)
        return;

      ss << p->to_string();

      for (int i = 0; i <= p->num_keys_; ++i) {
        dfs(p->child_[i]);
      }
    };
    dfs(p);

    ss << "]";
    return ss.str();
  }

  std::string to_string() const {
    std::stringstream ss;
    ss << "BTree: [\n";

    std::function<void(Node*)> dfs = [&](Node* p) {
      if (p == nullptr)
        return;

      ss << p->to_string();

      for (int i = 0; i <= p->num_keys_; ++i) {
        dfs(p->child_[i]);
      }
    };
    dfs(root_);

    ss << "]";
    return ss.str();
  }

 private:
  Node* root_;
};

void solve_case(int Case) {
  int n, q;
  std::cin >> n >> q;
  logd(n, q);

  BTree<int, 3> t;
  for (int i = 1, x; i <= n; ++i) {
    x = i;
    t.Insert(x);
    logd(t.to_string());
  }

  for (int i = 1, x; i <= n; ++i) {
    x = i;
    t.Delete(x);
    logd(x);
    logd(t.to_string());
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
  std::cout << ans << "\n";
}
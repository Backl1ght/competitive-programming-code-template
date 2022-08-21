// Problem: P3373 【模板】线段树 2
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P3373
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
#define ASSERT(x) ;
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

template <typename Data, typename Tag>
class SegmentTree {
 public:
  struct Node {
    Node* left_child_;
    Node* right_child_;

    int left_bound_;
    int right_bound_;

    Data data_;
    Tag tag_;

    void ApplayUpdate(const Tag& tag) {
      data_.Apply(left_bound_, right_bound_, tag);
      tag_.Apply(left_bound_, right_bound_, tag);
    }

    void MaintainInfomation() {
      ASSERT(left_child_ && right_child_);

      data_ = left_child_->data_ + right_child_->data_;
    }

    void Propagation() {
      if (tag_.NeedPropagation()) {
        right_child_->ApplayUpdate(tag_);
        left_child_->ApplayUpdate(tag_);
        tag_.Reset();
      }
    }

    Node() : left_child_(nullptr), right_child_(nullptr), left_bound_(-1), right_bound_(-1) {}
  };

  using Judger = std::function<bool(Data)>;

 public:
  SegmentTree(const std::vector<Data>& array) : n_(array.size()) {
    std::function<Node*(int, int)> build = [&](int left, int right) -> Node* {
      Node* p = new Node();
      p->left_bound_ = left;
      p->right_bound_ = right;

      if (left == right) {
        p->data_ = array[left];
      } else {
        int middle = (left + right) >> 1;
        p->left_child_ = build(left, middle);
        p->right_child_ = build(middle + 1, right);
        p->MaintainInfomation();
      }

      return p;
    };

    root_ = build(0, n_);
  }

  ~SegmentTree() {
    std::function<void(Node*)> dfs = [&](Node* p) {
      if (!p)
        return;
      dfs(p->left_child_);
      dfs(p->right_child_);
      delete p;
    };

    dfs(root_);
  }

  void UpdateInternal(Node* p, int left, int right, const Tag& tag) {
    ASSERT(p);
    if (p->left_bound_ >= left && p->right_bound_ <= right) {
      p->ApplayUpdate(tag);
      return;
    }

    p->Propagation();

    if (p->left_child_->right_bound_ >= left)
      UpdateInternal(p->left_child_, left, right, tag);
    if (p->right_child_->left_bound_ <= right)
      UpdateInternal(p->right_child_, left, right, tag);

    p->MaintainInfomation();
  }

  void Update(int left, int right, const Tag& tag) {
    ASSERT(left >= 0 && right < n_);

    UpdateInternal(root_, left, right, tag);
  }

  Data QueryInternal(Node* p, int left, int right) {
    ASSERT(p);

    if (p->left_bound_ >= left && p->right_bound_ <= right)
      return p->data_;

    p->Propagation();

    Data result;
    if (p->left_child_->right_bound_ >= left)
      result = result + QueryInternal(p->left_child_, left, right);
    if (p->right_child_->left_bound_ <= right)
      result = result + QueryInternal(p->right_child_, left, right);

    return result;
  }

  Data Query(int left, int right) {
    ASSERT(left >= 0 && right < n_);

    return QueryInternal(root_, left, right);
  }

  std::pair<int, Data> FindLeftmostIf(Judger judger) {
    Data result;
    return {-1, result};
  }

  std::pair<int, Data> FindRightmostIf(Judger judger) {
    Data result;
    return {-1, result};
  }

 private:
  int n_;
  Node* root_;
};

int p;

struct Tag {
 public:
  i64 range_add_;
  i64 range_mul_;

 public:
  Tag(i64 range_add = 0, i64 range_mul = 1) : range_add_(range_add), range_mul_(range_mul) {}

  bool NeedPropagation() { return range_add_ != 0 || range_mul_ != 1; }

  void Apply(int left, int right, const Tag& tag) {
    range_mul_ = range_mul_ * tag.range_mul_ % p;
    range_add_ = range_add_ * tag.range_mul_ % p;
    range_add_ = (range_add_ + tag.range_add_) % p;
  }

  void Reset() {
    range_add_ = 0;
    range_mul_ = 1;
  }
};

struct Data {
 public:
  i64 range_sum_;

 public:
  Data(i64 range_sum = 0) : range_sum_(range_sum) {}

  void Apply(int left, int right, const Tag& tag) {
    int length = right - left + 1;

    range_sum_ = range_sum_ * tag.range_mul_ % p;
    range_sum_ = (range_sum_ + tag.range_add_ * length % p) % p;
  }

  friend Data operator+(const Data& lhs, const Data& rhs) {
    Data result;

    result.range_sum_ = (lhs.range_sum_ + rhs.range_sum_) % p;

    return result;
  }
};

void solve_case(int Case) {
  int n, m;
  std::cin >> n >> m >> p;

  std::vector<Data> v(n);
  for (int i = 0; i < n; ++i) {
    i64 x;
    std::cin >> x;

    v[i] = Data(x);
  }

  SegmentTree<Data, Tag> seg(v);

  for (int i = 0; i < m; ++i) {
    int op, x, y;
    i64 k;
    std::cin >> op;
    if (op == 1) {
      std::cin >> x >> y >> k;
      --x, --y;

      seg.Update(x, y, Tag(0, k));
    } else if (op == 2) {
      std::cin >> x >> y >> k;
      --x, --y;

      seg.Update(x, y, Tag(k, 1));
    } else if (op == 3) {
      std::cin >> x >> y;
      --x, --y;

      Data data = seg.Query(x, y);
      std::cout << data.range_sum_ << "\n";
    }
  }
}
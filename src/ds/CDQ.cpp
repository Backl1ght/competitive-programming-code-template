// Problem: P3810 【模板】三维偏序（陌上花开）
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P3810
// Memory Limit: 500 MB
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
#define serialize() ""
#endif

using i64 = int64_t;
using u64 = uint64_t;

void Initialize();
void SolveCase(int Case);

int main(int argc, char* argv[]) {
  CPPIO;
  int T = 1;
  // std::cin >> T;
  for (int t = 1; t <= T; ++t) {
    SolveCase(t);
  }
  return 0;
}

void Initialize() {}

template <typename T>
class FenwickTree {
 public:
  using Operator = std::function<T(const T&, const T&)>;

 private:
  inline int lb(int x) { return x & -x; }

 public:
  FenwickTree(int n, const T& init, const Operator& op = std::plus<T>())
      : n_(n), init_(init), c_(n_ + 1, init_), op_(op), t_(n_ + 1, -1), tag_(0) {}

  void Update(int x, T d) {
    for (; x <= n_; x += lb(x)) {
      if (t_[x] != tag_)
        c_[x] = init_;
      c_[x] = op_(c_[x], d);
      t_[x] = tag_;
    }
  }

  const T Query(int x) {
    T r = init_;
    for (; x; x -= lb(x)) {
      if (t_[x] == tag_)
        r = op_(r, c_[x]);
    }
    return r;
  }

  // TODO(backlight): add something to ensure that this can only be called when op_ is inversable.
  const T Query(int l, int r) { return Query(r) - Query(l - 1); }

  // TODO(backlight): similar to range query.
  const T Kth(int k) {
    T ans = 0, cnt = 0;
    for (int i = std::__lg(n_) + 1; i >= 0; --i) {
      ans += (1LL << i);
      if (ans >= n_ || cnt + c_[ans] >= k)
        ans -= (1LL << i);
      else
        cnt += c_[ans];
    }
    return ans + 1;
  }

  void Reset() { ++tag_; }

 private:
  int n_;

  T init_;
  std::vector<T> c_;
  const Operator op_;

  std::vector<int> t_;
  int tag_;
};

void SolveCase(int Case) {
  int n, k;
  std::cin >> n >> k;

  std::vector<std::array<int, 4>> p(n);
  for (int i = 0; i < n; ++i) {
    int a, b, c;
    std::cin >> a >> b >> c;
    p[i] = {a, b, c, 1};
  }
  std::sort(p.begin(), p.end());

  int sz = 0;
  for (int i = 0; i < n; ++i) {
    int cnt = 0;
    int j = i;
    while (j + 1 < n && p[j + 1] == p[i])
      ++j;

    p[sz++] = {p[i][0], p[i][1], p[i][2], j - i + 1};

    i = j;
  }
  p.resize(sz);

  std::vector<int> ans(n, 0);
  FenwickTree<int> bit(k, 0);
  std::vector<std::array<int, 4>> temp(n);
  std::function<void(int, int)> CDQ = [&](int l, int r) {
    if (l == r)
      return;

    int mid = (l + r) >> 1;
    CDQ(l, mid), CDQ(mid + 1, r);

    temp.clear();
    for (int i = l; i <= r; ++i) {
      temp.push_back({p[i][1], p[i][2], i, p[i][3]});
    }
    std::sort(temp.begin(), temp.end());

    bit.Reset();
    for (auto [x, y, id, w] : temp) {
      if (id <= mid) {
        bit.Update(y, w);
      } else {
        ans[id] += bit.Query(y);
      }
    }
  };
  CDQ(0, n - 1);

  std::vector<int> cnt(n, 0);
  for (int i = 0; i < sz; ++i)
    cnt[ans[i] + p[i][3] - 1] += p[i][3];

  for (int i = 0; i < n; ++i)
    std::cout << cnt[i] << "\n";
}

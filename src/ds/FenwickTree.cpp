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
class DSU {
 public:
  DSU(int n) : n_(n), f_(n_) {
    for (int i = 0; i < n_; ++i) {
      f_[i] = i;
    }
  }

  int find(int x) {
    if (x != f_[x])
      f_[x] = find(f_[x]);

    return f_[x];
  }

  void merge(int x, int y) {
    x = find(x), y = find(y);
    if (x == y)
      return;

    f_[y] = x;
  }

 private:
  int n_;
  std::vector<int> f_;
};
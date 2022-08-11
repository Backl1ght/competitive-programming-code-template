class WDSU {
 public:
  WDSU(int n) : n_(n), f_(n_), type_(n_) {
    for (int i = 0; i < n_; ++i) {
      f_[i] = i;
      type_[i] = 0;
    }
  }

  int find(int x) {
    if (x != f_[x]) {
      int fx = f_[x];
      int ffx = find(fx);

      f_[x] = ffx;
      type_[x] = type_[x] ^ type_[fx];
    }
    return f_[x];
  }

  void merge(int x, int y, int z) {
    int fx = find(x), fy = find(y);
    if (fx == fy)
      return;

    f_[fy] = fx;
    type_[fy] = type_[x] ^ type_[y] ^ z;
  }

  int type(int x) {
    int fx = find(x);
    return type_[x];
  }

 private:
  int n_;
  std::vector<int> f_;
  std::vector<int> type_;
};
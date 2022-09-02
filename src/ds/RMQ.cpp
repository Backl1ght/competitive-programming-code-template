template <typename ValueType>
class RMQ {
 private:
  using Operator = std::function<ValueType(const ValueType&, const ValueType&)>;

  int n_;
  std::vector<std::vector<ValueType>> a_;
  std::vector<int> lg_;
  Operator op_;

 public:
  RMQ(const std::vector<ValueType>& a, const Operator& op) : op_(op) {
    n_ = a.size();

    lg_.resize(n_ + 1);
    lg_[1] = 0;
    for (int i = 2; i <= n_; ++i)
      lg_[i] = lg_[i >> 1] + 1;

    a_.resize(n_);
    for (int i = 0; i < n_; ++i) {
      a_[i].resize(lg_[n_] + 1);
      a_[i][0] = a[i];
    }
    for (int j = 1; j <= lg_[n_]; ++j) {
      for (int i = 0; i + (1 << (j - 1)) < n_; ++i) {
        a_[i][j] = op_(a_[i][j - 1], a_[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  const ValueType Query(int l, int r) {
    int k = lg_[r - l + 1];
    return op_(a_[l][k], a_[r - (1 << k) + 1][k]);
  }
};
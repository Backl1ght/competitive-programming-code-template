namespace String {

template <int p, int m>
class SingleStringHash {
 private:
  static std::vector<int> b_;

  void TryExtendBase(int n) {
    while ((int)b_.size() < n + 1) {
      int x = b_.back();
      b_.push_back(int64_t(1) * x * p % m);
    }
  }

 public:
  SingleStringHash(const std::string& s) : n_(s.size()), h_(n_ + 1) {
    TryExtendBase(n_);

    h_[0] = 0;
    for (int i = 0; i < n_; ++i)
      h_[i + 1] = (int64_t(1) * h_[i] * p % m + s[i]) % m;
  }

  int Get(int l, int r) const {
    ASSERT(0 <= l && l <= r && r < n_);
    ++l, ++r;
    int hash = (h_[r] - int64_t(1) * h_[l - 1] * b_[r - l + 1] % m + m) % m;
    return hash;
  }

 public:
  int n_;
  std::vector<int> h_;
};
template <int p, int m>
std::vector<int> SingleStringHash<p, m>::b_ = {1};

template <int p_lo, int m_lo, int p_hi, int m_hi>
class DoubleStringHash {
 public:
  DoubleStringHash(const std::string& s)
      : n_(s.size()), hash_lo_(s), hash_hi_(s) {}

  int64_t Get(int l, int r) const {
    ASSERT(0 <= l && l <= r && r < n_);
    int lo = hash_lo_.Get(l, r);
    int hi = hash_hi_.Get(l, r);
    return int64_t(1) * hi * m_lo + lo;
  }

 public:
  int n_;
  SingleStringHash<p_lo, m_lo> hash_lo_;
  SingleStringHash<p_hi, m_hi> hash_hi_;
};

}  // namespace String
using StringHash = String::DoubleStringHash<29, 998'244'353, 31, 1'000'000'007>;
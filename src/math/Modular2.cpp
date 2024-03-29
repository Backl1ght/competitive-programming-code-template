template <typename ValueType, ValueType mod_, typename SupperType>
class Modular {
  static ValueType normalize(ValueType value) {
    if (value >= 0 && value < mod_)
      return value;
    value %= mod_;
    if (value < 0)
      value += mod_;
    return value;
  }

  template <typename ExponentType>
  static ValueType power(ValueType value, ExponentType exponent) {
    ValueType result = 1;
    ValueType base = value;
    while (exponent) {
      if (exponent & 1)
        result = SupperType(result) * base % mod_;
      base = SupperType(base) * base % mod_;
      exponent >>= 1;
    }
    return result;
  }

 public:
  Modular() : value_(0) {}

  Modular(ValueType value) : value_(normalize(value)) {}

  Modular(SupperType value) : value_(normalize(value % mod_)) {}

  ValueType value() const { return value_; }

  Modular inv() const { return Modular(power(value_, mod_ - 2)); }

  template <typename ExponentType>
  Modular power(ExponentType exponent) const {
    return Modular(power(value_, exponent));
  }

  Modular operator-() const { return Modular(mod_ - value_); }

  Modular& operator+=(const Modular& other) {
    value_ = value_ + other.value() >= mod_ ? value_ + other.value() - mod_
                                            : value_ + other.value();
    return (*this);
  }

  Modular& operator-=(const Modular& other) {
    value_ = value_ - other.value() < 0 ? value_ - other.value() + mod_
                                        : value_ - other.value();
    return (*this);
  }

  Modular& operator*=(const Modular& other) {
    value_ = SupperType(1) * value_ * other.value() % mod_;
    return (*this);
  }

  Modular& operator/=(const Modular& other) {
    value_ = SupperType(1) * value_ * other.inv().value() % mod_;
    return (*this);
  }

  Modular operator+(const Modular& other) const {
    Modular result = *this;
    result += other;
    return result;
  }

  Modular operator-(const Modular& other) const {
    Modular result = *this;
    result -= other;
    return result;
  }

  Modular operator*(const Modular& other) const {
    Modular result = *this;
    result *= other;
    return result;
  }

  Modular operator/(const Modular& other) const {
    Modular result = *this;
    result /= other;
    return result;
  }

  std::string to_string() const { return std::to_string(value_); }

 private:
  ValueType value_;
};

// using Mint = Modular<int, 1'000'000'007, int64_t>;
using Mint = Modular<int, 998'244'353, int64_t>;

class Binom {
 public:
  std::vector<Mint> f, g;

 public:
  Binom(int n) {
    f.resize(n + 1);
    g.resize(n + 1);

    f[0] = Mint(1);
    for (int i = 1; i <= n; ++i)
      f[i] = f[i - 1] * Mint(i);
    g[n] = f[n].inv();
    for (int i = n - 1; i >= 0; --i)
      g[i] = g[i + 1] * Mint(i + 1);
  }
  Mint operator()(int n, int m) {
    if (n < 0 || m < 0 || m > n)
      return Mint(0);
    return f[n] * g[m] * g[n - m];
  }
};
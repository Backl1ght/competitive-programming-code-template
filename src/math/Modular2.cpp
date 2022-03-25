template <typename ValueType, ValueType mod_, typename SupperType = int64_t>
class Modular {
 private:
  ValueType value_;

  ValueType normalize(ValueType value) {
    if (value >= 0 && value < mod_)
      return value;
    value %= mod_;
    if (value < 0)
      value += mod_;
    return value;
  }

  ValueType power(ValueType value, size_t exponent) {
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
  Modular(const ValueType& value) : value_(normalize(value)) {}

  ValueType value() const { return value_; }

  Modular inv() const { return Modular(power(value_, mod_ - 2)); }

  Modular power(size_t exponent) const {
    return Modular(power(value, exponent));
  }

  friend Modular operator+(const Modular& lhs, const Modular& rhs) {
    ValueType result = lhs.value() + rhs.value() >= mod_
                           ? lhs.value() + rhs.value() - mod_
                           : lhs.value() + rhs.value();
    return Modular(result);
  }

  friend Modular operator-(const Modular& lhs, const Modular& rhs) {
    ValueType result = lhs.value() - rhs.value() < 0
                           ? lhs.value() - rhs.value() + mod_
                           : lhs.value() - rhs.value();
    return Modular(result);
  }

  friend Modular operator*(const Modular& lhs, const Modular& rhs) {
    ValueType result = SupperType(1) * lhs.value() * rhs.value() % mod_;
    return Modular(result);
  }

  friend Modular operator/(const Modular& lhs, const Modular& rhs) {
    ValueType result = lhs.value() * rhs.inv();
    return Modular(result);
  }
};
template <typename StreamType,
          typename ValueType,
          ValueType mod,
          typename SupperType = int64_t>
StreamType& operator<<(StreamType& out,
                       const Modular<ValueType, mod, SupperType>& modular) {
  return out << modular.value();
}
template <typename StreamType,
          typename ValueType,
          ValueType mod,
          typename SupperType = int64_t>
StreamType& operator>>(StreamType& in,
                       Modular<ValueType, mod, SupperType>& modular) {
  ValueType value;
  in >> value;
  modular = Modular<ValueType, mod, SupperType>(value);
  return in;
}
using mint = Modular<int, 1'000'000'007>;
// using mint = Modular<int, 998'244'353>;
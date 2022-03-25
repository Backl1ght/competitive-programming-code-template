template <typename ValueType>
class Matrix {
 private:
  using MatrixDataType = std::vector<std::vector<ValueType>>;
  using RowDataType = std::vector<ValueType>;

  size_t n_;
  size_t m_;
  MatrixDataType a_;

 public:
  static Matrix zero(size_t n, size_t m) {
    MatrixDataType data(n, RowDataType(m, 0));
    return Matrix(move(data));
  }

  static Matrix one(size_t n, size_t m) {
    assert(n == m);
    MatrixDataType data(n, RowDataType(m, 0));
    for (size_t i = 0; i < n; ++i) {
      data[i][i] = 1;
    }
    return Matrix(move(data));
  }

 public:
  Matrix(const MatrixDataType& a) : n_(a.size()), m_(a[0].size()), a_(a) {}

  Matrix(const Matrix& matrix) : n_(matrix.n_), m_(matrix.m_), a_(matrix.a_) {}

  size_t n() const { return n_; }

  size_t m() const { return m_; }

  RowDataType& operator[](size_t row) { return a_[row]; }

  const ValueType& at(size_t row, size_t col) const { return a_[row][col]; }

  friend Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    assert(lhs.m() == rhs.n());

    Matrix result = zero(lhs.n(), rhs.m());
    for (size_t i = 0; i < lhs.n(); ++i) {
      for (size_t j = 0; j < lhs.m(); ++j) {
        for (size_t k = 0; k < rhs.m(); ++k) {
          result[i][k] = result.at(i, k) + lhs.at(i, j) * rhs.at(j, k);
        }
      }
    }
    return result;
  }

  void operator=(const Matrix& rhs) {
    n_ = rhs.n_;
    m_ = rhs.m_;
    a_ = rhs.a_;
  }

  friend Matrix operator^(const Matrix& lhs, size_t exponent) {
    assert(lhs.n() == lhs.m());

    Matrix result = one(lhs.n(), lhs.m());
    Matrix base = lhs;
    while (exponent) {
      if (exponent & 1)
        result = result * base;
      base = base * base;
      exponent >>= 1;
    }
    return result;
  }
};
template <typename ValueType>
std::string to_string(const Matrix<ValueType>& matrix) {
  std::stringstream ss;
  ss << "[";
  for (size_t i = 0; i < matrix.n(); ++i) {
    ss << "[";
    for (size_t j = 0; j < matrix.m(); ++j) {
      ss << matrix[i][j] << ", "[j == matrix.m() - 1];
    }
    ss << ",]"[i == matrix.n() - 1];
  }
  return ss.str();
}
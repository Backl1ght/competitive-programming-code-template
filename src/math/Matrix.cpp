template <typename ValueType>
class Matrix {
 private:
  using MatrixDataType = std::vector<std::vector<ValueType>>;
  using RowDataType = std::vector<ValueType>;

  int n_;
  int m_;
  MatrixDataType a_;

 public:
  static Matrix zero(int n, int m) {
    MatrixDataType data(n, RowDataType(m, 0));
    return Matrix(move(data));
  }

  static Matrix one(int n, int m) {
    assert(n == m);
    MatrixDataType data(n, RowDataType(m, 0));
    for (int i = 0; i < n; ++i) {
      data[i][i] = 1;
    }
    return Matrix(move(data));
  }

 public:
  Matrix() : n_(0), m_(0) {}

  Matrix(const MatrixDataType& a) : n_(a.size()), m_(a[0].size()), a_(a) {}

  Matrix(const Matrix& matrix) : n_(matrix.n_), m_(matrix.m_), a_(matrix.a_) {}

  int n() const { return n_; }

  int m() const { return m_; }

  RowDataType& operator[](int row) { return a_[row]; }

  const ValueType& at(int row, int col) const { return a_[row][col]; }

  friend Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    assert(lhs.m() == rhs.n());

    Matrix result = zero(lhs.n(), rhs.m());
    for (int i = 0; i < lhs.n(); ++i) {
      for (int j = 0; j < lhs.m(); ++j) {
        for (int k = 0; k < rhs.m(); ++k) {
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

  friend Matrix operator^(const Matrix& lhs, int64_t exponent) {
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

  std::string to_string() const {
    std::stringstream ss;
    ss << "[";
    for (int i = 0; i < n_; ++i) {
      ss << "[";
      for (int j = 0; j < m_; ++j) {
        ss << a_[i][j].value() << ",]"[j == m_ - 1];
      }
      ss << ",]"[i == n_ - 1];
    }
    return ss.str();
  }
};
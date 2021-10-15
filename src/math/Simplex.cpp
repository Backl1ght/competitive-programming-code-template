/**
 * Simplex Alogorithm:
 * solve $\max z = \sum_{j = 1}^{n} c_{j}x_{j}$
 * with restrictions like: $ \sum_{j = 1}^{n} a_{ij}x_{j} = b_{j}, i = 1, 2, ..., m $
 *                         $ x_j \ge 0 $
 * in $ O(knm) $, where k is a const number。
 * 
 * Tips: 1. min => -min => max
 *       2. $x_1 + 2x_2 \le 9$ => $ x_1 + x_2 + x_3 = 9, x_3 \ge 0 $
 *       3. x_k without restrictions => $x_k = x_m - x_m and x_m, x_n \ge 0$
 * 
 * Notes: 1. c = A_{0}
 *        2. z = max cx
 *        3. Ax = b
 */
enum {
  OK = 1,
  UNBOUNDED = 2,
  INFEASIBLE = 3
};
struct Simplex {
  constexpr static double eps = 1e-10;

  int n, m;
  int flag;
  double z;
  vector<vector<double>> A;
  vector<double> b, x;
  vector<int> idx, idy;

  Simplex(int _n, int _m)
      : n(_n), m(_m) {
    A = vector<vector<double>>(m + 1, vector<double>(n + 1));
    b = vector<double>(m + 1);
    x = vector<double>(n + 1);
    idx = vector<int>(m + 1);
    idy = vector<int>(n + 1);
  }

  void input() {
    for (int i = 1; i <= n; ++i)
      read(A[0][i]);  // A_{0,i} = c_i
    for (int i = 1; i <= m; ++i) {
      for (int j = 1; j <= n; ++j)
        read(A[i][j]);
      read(b[i]);
    }
  }

  void pivot(int x, int y) {
    swap(idx[x], idy[y]);

    double k = A[x][y];
    for (int i = 1; i <= n; ++i)
      A[x][i] /= k;
    b[x] /= k;
    A[x][y] = 1 / k;

    for (int i = 0; i <= m; ++i)
      if (i != x) {
        k = A[i][y];
        b[i] -= k * b[x];
        A[i][y] = 0;
        for (int j = 1; j <= n; ++j)
          A[i][j] -= k * A[x][j];
      }
  }

  void init() {
    flag = OK;
    idx[0] = INT_MAX;
    for (int i = 1; i <= m; ++i)
      idx[i] = n + i;
    idy[0] = INT_MAX;
    for (int i = 1; i <= n; ++i)
      idy[i] = i;

    for (;;) {
      int x = 0, y = 0;
      for (int i = 1; i <= m; ++i)
        if (b[i] < -eps && idx[i] < idx[x])
          x = i;
      if (!x)
        break;

      for (int i = 1; i <= n; ++i)
        if (A[x][i] < -eps && idy[i] < idy[y])
          y = i;
      if (!y) {
        flag = INFEASIBLE;
        break;
      }

      pivot(x, y);
    }
  }

  void simplex() {
    for (;;) {
      int x = 0, y = 0;
      for (int i = 1; i <= n; ++i)
        if (A[0][i] > eps && idy[i] < idy[y])
          y = i;
      if (!y)
        break;

      for (int i = 1; i <= m; ++i)
        if (A[i][y] > eps) {
          if (!x)
            x = i;
          else {
            double delta = b[i] / A[i][y] - b[x] / A[x][y];
            if (delta < -eps)
              x = i;
            else if (delta < eps && idx[i] < idx[x])
              x = i;
          }
        }
      if (!x) {
        flag = UNBOUNDED;
        break;
      }

      pivot(x, y);
    }
    z = -b[0];
  }

  void work() {
    init();
    if (flag == OK)
      simplex();
    if (flag == OK) {
      for (int i = 1; i <= n; ++i) {
        x[i] = 0;
        for (int j = 1; j <= m; ++j)
          if (idx[j] == i) {
            x[i] = b[j];
            break;
          }
      }
    }
  }

  void DEBUG() {
    cerr << fixed << setprecision(3);
    cerr << "Simplex Debug: \n";
    for (int i = 1; i <= m; ++i) {
      for (int j = 1; j <= n; ++j) {
        cerr << A[i][j] << " ";
      }
      cerr << "\n";
    }
    for (int i = 1; i <= n; ++i)
      cerr << x[i] << " ";
    cerr << endl;
    cerr << "Z = " << z << endl;
  }
};
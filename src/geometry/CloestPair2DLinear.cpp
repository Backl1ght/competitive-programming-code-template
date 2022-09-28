// Problem: P7883 平面最近点对（加强加强版）
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P7883
// Memory Limit: 512 MB
// Time Limit: 350 ms
//
// Powered by CP Editor (https://cpeditor.org)
#include <bits/stdc++.h>
using namespace std;

#define CPPIO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
#ifdef BACKLIGHT
#include "debug.h"
#else
#define logd(...) ;
#endif

using i64 = int64_t;
using u64 = uint64_t;

void solve_case(int Case);

int main() {
  CPPIO;
  int T = 1;
  // cin >> T;
  for (int t = 1; t <= T; ++t) {
    solve_case(t);
  }
  return 0;
}

/**
 * Solve Cloest Pair Problem in **expected** linear time.
 * Idea: https://github.com/OI-wiki/OI-wiki/pull/3851
 * */
template <typename CoordinateType, typename DistanceType>
class ClosestPair2DLinear {
 public:
  struct Point {
    CoordinateType x_;
    CoordinateType y_;

    Point() {}

    Point(const CoordinateType& x, const CoordinateType& y) : x_(x), y_(y) {}

    void input() { cin >> x_ >> y_; }

    bool operator==(const Point& p) const { return x_ == p.x_ && y_ == p.y_; }
  };
  using PointIt = typename vector<Point>::iterator;
  using Grid = Point;

 private:
  /* FIXME(backlight): find a better solution. */
  /* Ugly hash map. I tried std::unordered_map and get TLE. */
  class GridMap {
   public:
    static const size_t P = 1'000'000'009LL;
    static const size_t M = 1'000'003;
    static const size_t npos = numeric_limits<size_t>::max();
    vector<size_t> h;
    vector<size_t> nxt;

    void Clear() {
      size_t p = npos;
      fill(h.begin(), h.end(), p);
    }

    size_t GetHash(const Grid& grid) { return (grid.x_ * P + grid.y_) % M; }

    void Insert(size_t index, const Grid& grid) {
      size_t hash = GetHash(grid);
      nxt[index] = h[hash];
      h[hash] = index;
    }

    void Init(size_t n) {
      size_t p = npos;
      h = vector<size_t>(M, p);
      nxt = vector<size_t>(n);
    }
  };

 private:
  PointIt start_;
  vector<size_t> id_;

  CoordinateType grid_length_;
  GridMap grid_map_;

  DistanceType result_;
  size_t a_;
  size_t b_;

 private:
  const Point& At(size_t p) const { return *(start_ + p); }

  DistanceType Distance2(const Point& a, const Point& b) const {
    DistanceType dx = a.x_ - b.x_;
    DistanceType dy = a.y_ - b.y_;
    return dx * dx + dy * dy;
  }

  Grid CalcGrid(const Point& a) const {
    return Grid(a.x_ / grid_length_, a.y_ / grid_length_);
  }

  void TryUpdate(size_t a, size_t b) {
    DistanceType temp = Distance2(At(a), At(b));
    if (temp < result_) {
      result_ = temp;
      a_ = a;
      b_ = b;
    }
  }

  void BuildHashTable(size_t size) {
    grid_map_.Clear();
    for (size_t i = 0; i < size; ++i) {
      const Grid& grid_i = CalcGrid(At(id_[i]));
      grid_map_.Insert(id_[i], grid_i);
    }
  }

 public:
  ClosestPair2DLinear() {}
  DistanceType Solve(PointIt start, PointIt end) {
    size_t length = end - start;
    assert(length > 1);

    start_ = start;
    id_.resize(length);
    iota(id_.begin(), id_.end(), 0);
    shuffle(id_.begin(), id_.end(), mt19937(114514));
    grid_map_.Init(length);

    a_ = id_[0];
    b_ = id_[1];
    result_ = Distance2(At(a_), At(b_));
    grid_length_ = sqrt(result_) + 1;
    BuildHashTable(2);

    size_t size = 2;
    for (size_t i = 2; i < length; ++i) {
      DistanceType before_result = result_;

      const Point& cur_point = At(id_[i]);
      const Grid& cur_grid = CalcGrid(cur_point);
      for (CoordinateType dx = -1; dx <= 1; ++dx) {
        for (CoordinateType dy = -1; dy <= 1; ++dy) {
          const Grid& next_grid = Grid(cur_grid.x_ + dx, cur_grid.y_ + dy);
          size_t hash = grid_map_.GetHash(next_grid);
          for (size_t j = grid_map_.h[hash]; j != GridMap::npos;
               j = grid_map_.nxt[j]) {
            if (next_grid == CalcGrid(At(j))) TryUpdate(id_[i], j);
          }
        }
      }

      if (result_ < before_result) {
        grid_length_ = sqrt(result_) + 1;
        BuildHashTable(size);
        const Grid& new_grid = CalcGrid(cur_point);
        grid_map_.Insert(id_[i], new_grid);
      } else {
        grid_map_.Insert(id_[i], cur_grid);
      }
      size = size + 1;
    }

    return result_;
  }
};
using ClosestPair = ClosestPair2DLinear<int, i64>;
using Point = ClosestPair::Point;
ClosestPair solver;

void solve_case(int Case) {
  int n;
  cin >> n;
  vector<Point> P(n);
  for (auto& p : P) p.input();

  for_each(P.begin(), P.end(), [&](Point& p) {
    p.x_ = p.x_ + 1e7;
    p.y_ = p.y_ + 1e7;
  });

  i64 ans = solver.Solve(P.begin(), P.end());
  cout << ans << "\n";
}
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

template <typename CoordinateType, typename DistanceType>
class ClosestPair2D {
 private:
  const static size_t kBruteForceThreshold = 5;

 public:
  struct Point {
    CoordinateType x_;
    CoordinateType y_;

    Point() {}
    Point(CoordinateType x, CoordinateType y) : x_(x), y_(y) {}

    void input() { cin >> x_ >> y_; }
  };

 private:
  using PointIt = typename vector<Point>::iterator;
  using It = vector<size_t>::iterator;

  struct CompX {
    ClosestPair2D* p_;
    CompX(ClosestPair2D* p) : p_(p) {}
    bool operator()(size_t a, size_t b) const {
      const Point& pa = p_->At(a);
      const Point& pb = p_->At(b);
      if (pa.x_ != pb.x_) return pa.x_ < pb.x_;
      return pa.y_ < pb.y_;
    };
  };

  struct CompY {
    ClosestPair2D* p_;
    CompY(ClosestPair2D* p) : p_(p) {}
    bool operator()(size_t a, size_t b) const {
      const Point& pa = p_->At(a);
      const Point& pb = p_->At(b);
      return pa.y_ < pb.y_;
    };
  };

 private:
  PointIt start_;
  vector<size_t> id_;
  vector<size_t> buffer_;
  CompX comp_x_;
  CompY comp_y_;

  DistanceType result_;
  size_t a_;
  size_t b_;

 private:
  const Point& At(size_t p) { return *(start_ + p); }

  DistanceType Distance2(const Point& a, const Point& b) {
    DistanceType dx = a.x_ - b.x_;
    DistanceType dy = a.y_ - b.y_;
    return dx * dx + dy * dy;
  }

  DistanceType Square(DistanceType x) { return x * x; }

  void TryUpdate(size_t a, size_t b) {
    DistanceType temp = Distance2(At(a), At(b));
    if (temp < result_) {
      result_ = temp;
      a_ = a;
      b_ = b;
    }
  }

  void SolveInternal(It start, It end) {
    size_t length = end - start;
    if (length <= kBruteForceThreshold) {
      for (It i = start; i < end; ++i) {
        for (It j = i + 1; j < end; ++j) {
          TryUpdate(*i, *j);
        }
      }
      sort(start, end, comp_y_);
      return;
    }

    It split = start + (length >> 1);
    const Point& split_point = At(*split);
    SolveInternal(start, split);
    SolveInternal(split, end);

    std::inplace_merge(start, split, end, comp_y_);
    buffer_.clear();
    for (It i = start; i < end; ++i) {
      const Point& cur_point = At(*i);
      DistanceType dx2 = Square(cur_point.x_ - split_point.x_);
      if (dx2 <= result_) {
        for (It j = buffer_.end() - 1; j >= buffer_.begin(); --j) {
          const Point& next_point = At(*j);
          DistanceType dy2 = Square(next_point.y_ - cur_point.y_);
          if (dy2 <= result_) {
            TryUpdate(*i, *j);
          } else {
            break;
          }
        }
        buffer_.push_back(*i);
      }
    }
  }

 public:
  ClosestPair2D() : comp_x_(this), comp_y_(this) {}
  DistanceType Solve(PointIt start, PointIt end) {
    size_t length = end - start;
    assert(length > 1);

    start_ = start;
    id_.resize(length);
    iota(id_.begin(), id_.end(), 0);
    sort(id_.begin(), id_.end(), comp_x_);
    buffer_.clear();
    buffer_.reserve(length);

    a_ = id_[0];
    b_ = id_[1];
    result_ = Distance2(At(a_), At(b_));

    SolveInternal(id_.begin(), id_.end());

    return result_;
  }
};
using ClosestPair = ClosestPair2D<int, i64>;
using Point = ClosestPair::Point;
ClosestPair solver;

void solve_case(int Case) {
  int n;
  cin >> n;
  vector<Point> P(n);
  for (auto& p : P) p.input();

  i64 ans = solver.Solve(P.begin(), P.end());
  cout << ans << "\n";
}
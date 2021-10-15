namespace Geometry {
// 定义以及防止精度出错
const double eps = 1e-8;
const double inf = 1e9;
const double pi = acos(-1.0);

inline int sgn(double x) {
  if (fabs(x) < eps)
    return 0;
  if (x < 0)
    return -1;
  return 1;
}

// 单位换算
inline double degree2radian(const double& alpha) {
  return alpha / 180 * pi;
}

inline double radian2degree(const double& alpha) {
  return alpha / pi * 180;
}

// 点(向量)
// 也是远点到该点的向量
struct point {
  double x, y;
  point(double _x = 0, double _y = 0)
      : x(_x), y(_y) {}

  point operator-(const point& b) const {
    return point(x - b.x, y - b.y);
  }

  point operator+(const point& b) const {
    return point(x + b.x, y + b.y);
  }

  bool operator<(const point& b) const {
    return sgn(x - b.x) == 0 ? sgn(y - b.y) < 0 : sgn(x - b.x) < 0;
  }

  bool operator==(const point& b) const {
    return sgn(x - b.x) == 0 && sgn(y - b.y) == 0;
  }

  point operator*(const double& b) {
    return point(x * b, y * b);
  }

  point operator/(const double& b) {
    return point(x / b, y / b);
  }

  // 绕原点逆时针旋转，给出正弦和余弦值
  // 若绕另一点p，则先转换成以p为原点，完成旋转，再转换回来
  void transxy(const double& sinb, const double& cosb) {
    double tx = x, ty = y;
    x = tx * cosb - ty * sinb;
    y = tx * sinb + ty * cosb;
  }

  // 绕原点逆时针旋转，给出旋转弧度
  void transxy(const double& b) {
    double tx = x, ty = y;
    x = tx * cos(b) - ty * sin(b);
    y = tx * sin(b) + ty * cos(b);
  }

  // 逆时针旋转90度
  point trans90() {
    return point(-y, x);
  }

  // 顺时针旋转90度
  point trans270() {
    return point(y, -x);
  }

  // 与原点的距离
  // a,b之间的距离: (b- a).length()
  double length() {
    return sqrt(x * x + y * y);
  }

  // 与原点的距离的平方
  double length2() {
    return x * x + y * y;
  }

  // 与点a之间的距离
  double disTo(const point& a) {
    return (a - *this).length();
  }

  // 与x轴正方向的夹角，单位为弧度
  double alpha() {
    return atan2(y, x);
  }

  // 单位向量
  point unit() {
    return point(x, y) / length();
  }
};

// 向量Oa和向量Ob的叉积
inline double det(const point& a, const point& b) {
  return a.x * b.y - a.y * b.x;
}

// 向量ab和向量ac的叉积
inline double det(const point& a, const point& b, const point& c) {
  return det(b - a, c - a);
}

// 向量Oa和向量Ob的点积
inline double dot(const point& a, const point& b) {
  return a.x * b.x + a.y * b.y;
}

// 向量ab和向量ac的点积
inline double dot(const point& a, const point& b, const point& c) {
  return dot(b - a, c - a);
}

// 两点间距离
inline double distance(const point& a, const point& b) {
  return (a - b).length();
}

// 两点间距离的平方
inline double distance2(const point& a, const point& b) {
  return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

// LightOJ1203
// 最终答案会在凸包上，然后算ab与ac的夹角，单位为弧度
// ab与ac的夹角
double radian(point a, point b, point c) {
  return fabs(atan2(fabs(det(a, b, c)), dot(a, b, c)));
}

double angle(point a, point b, point c) {
  double r = radian(a, b, c);
  return radian2degree(r);
}

// 从点a，由b遮挡，能否看见c
bool canSee(point a, point b, point c) {
  return sgn(det(a, b, c)) <= 0;
}

// 直线或者线段
struct line {
  point s, e;      // 直线端点
  double a, b, c;  // ax+by+c=0
  double k;        // 斜率,[-pi, pi]

  line(point _s = point(), point _e = point())
      : s(_s), e(_e) {
    k = atan2(e.y - s.y, e.x - s.x);
    a = e.y - s.y;
    b = s.x - e.x;
    c = e.x * s.y - e.y * s.x;
  }

  // ax + by + c = 0;
  line(const double& _a, const double& _b, const double& _c)
      : a(_a), b(_b), c(_c) {
    if (sgn(a) == 0) {
      s = point(0, -c / b);
      e = point(1, -c / b);
    } else if (sgn(b) == 0) {
      s = point(-c / a, 0);
      e = point(-c / a, 1);
    } else {
      s = point(0, -c / b);
      e = point(1, (-c - a) / b);
    }
  }

  // 点和倾斜角确定直线
  line(const point& a, const double b)
      : s(a) {
    if (sgn(b - pi / 2) == 0)
      e = s + point(0, 1);
    else
      e = s + point(1, tan(b));
  }

  bool operator==(const line& l) {
    return (s == l.s) && (e == l.e);
  }

  void adjust() {
    if (e < s)
      swap(s, e);
  }

  double length() {
    return s.disTo(e);
  }

  // 判断点和直线的关系
  // 1 在直线左侧
  // 2 在直线右侧
  // 3 在直线上
  int relationToPoint(point p) {
    int c = sgn(det(s, p, e));
    if (c < 0)
      return 1;
    else if (c > 0)
      return 2;
    else
      return 3;
  }

  // 判断点p是否在线段上
  bool isPointOnLine(const point& p) {
    return sgn(det(p - s, e - s)) == 0 && sgn(det(p - s, p - e)) <= 0;
  }

  // 判断两直线是否平行
  bool parallelTo(line l) {
    return sgn(det(e - s, l.e - l.s)) == 0;
  }

  // 线段相交判断
  // 0 不相交
  // 1 交点是端点
  // 2 交点不是端点
  int isSegCrossSeg(line l) {
    int d1 = sgn(det(s, e, l.s));
    int d2 = sgn(det(s, e, l.e));
    int d3 = sgn(det(l.s, l.e, s));
    int d4 = sgn(det(l.s, l.e, e));
    if ((d1 ^ d2) == -2 && (d3 ^ d4) == -2)
      return 2;
    return (d1 == 0 && sgn(dot(l.s - s, l.s - e)) <= 0) || (d2 == 0 && sgn(dot(l.e - s, l.e - e)) <= 0) || (d3 == 0 && sgn(dot(s - l.s, s - l.e)) <= 0) || (d4 == 0 && sgn(dot(e - l.s, e - l.e)) <= 0);
  }

  // 直线相交判断
  // 0 平行
  // 1 重合
  // 2 相交
  bool isLineCrossLine(line l) {
    if (parallelTo(l))
      return l.relationToPoint(s) == 3;
    return 2;
  }

  // 本直线与线段v相交判断
  // 0 不相交
  // 1 交点是端点
  // 2 交点不是端点
  int isLineCrossSeg(line seg) {
    int d1 = sgn(det(s, e, seg.s));
    int d2 = sgn(det(s, e, seg.e));
    if ((d1 ^ d2) == -2)
      return 2;
    return (d1 == 0 || d2 == 0);
  }

  // 求两直线交点
  // 要求两直线不平行或重合
  point getCrossPoint(line l) {
    double a1 = det(l.s, l.e, s);
    double a2 = -det(l.s, l.e, e);
    return (s * a2 + e * a1) / (a1 + a2);
  }

  // 点到直线的距离
  double disPointToLine(const point& p) {
    double d = det(s, p, e) / length();
    return fabs(d);
  }

  // 点到线段的距离
  double disPointToSeg(const point& p) {
    if (sgn(dot(s, p, e)) < 0 || sgn(dot(e, p, s)) < 0)
      return min(distance(p, s), distance(p, e));
    return fabs(disPointToLine(p));
  }

  // 线段到线段的距离
  double disSegToSeg(line& l) {
    if (isSegCrossSeg(l) == 0) {
      double d1 = min(disPointToSeg(l.s), disPointToSeg(l.e));
      double d2 = min(l.disPointToSeg(s), l.disPointToSeg(e));
      return min(d1, d2);
    }
    return 0;
  }

  // 点在直线上的投影
  point projectionPointOnLine(const point& p) {
    return s + (dot(e - s, dot(s, e, p))) / ((e - s).length2());
  }

  // 点关于直线的对称点
  point symmetryPoint(const point& p) {
    point q = projectionPointOnLine(p);
    return point(2 * q.x - p.x, 2 * q.y - p.y);
  }

  // 垂直平分线
  line getVerticalBisector() {
    point m = (s + e) / 2;
    double radian = (e - s).alpha() + pi / 2;
    return line(m, radian);
  }
};

point getLineCrossLine(line l1, line l2) {
  return l1.getCrossPoint(l2);
}

// 向量表示法, 方向为由s -> e
// struct line
// {
//     point s,v;
//     line(point a=point(),point b=point()) {
//         s=a;
//         v.x=b.x-a.x;
//         v.y=b.y-a.y;
//     }
// };

// 圆
struct circle {
  point p;   // 圆心
  double r;  // 半径

  circle() {}

  circle(point _p, double _r)
      : p(_p), r(_r) {}
  circle(double _x, double _y, double _r)
      : p(point(_x, _y)), r(_r) {}

  // 圆上三点确定圈
  circle(point x1, point x2, point x3) {
    double a = x2.x - x1.x;
    double b = x2.y - x1.y;
    double c = x3.x - x2.x;
    double d = x3.y - x2.y;
    double e = x2.x * x2.x + x2.y * x2.y - x1.x * x1.x - x1.y * x1.y;
    double f = x3.x * x3.x + x3.y * x3.y - x2.x * x2.x - x2.y * x2.y;

    p = point((f * b - e * d) / (c * b - a * d) / 2, (a * f - e * c) / (a * d - b * c) / 2);
    r = distance(p, x1);
  }

  double area() {
    return pi * r * r;
  }

  double perimeter() {
    return 2 * pi * r;
  }

  // 点和圆的关系
  // 0 圆外
  // 1 圆上
  // 2 圆内
  int relationToPoint(point a) {
    double d2 = distance2(p, a);
    if (sgn(d2 - r * r) < 0)
      return 2;
    else if (sgn(d2 - r * r) == 0)
      return 1;
    return 0;
  }

  // 圆和直线的关系
  // 0 圆外
  // 1 圆上
  // 2 圆内
  int relationToLine(line l) {
    double d = l.disPointToLine(p);
    if (sgn(d - r) < 0)
      return 2;
    else if (sgn(d - r) == 0)
      return 1;
    return 0;
  }

  // 圆和线段的关系
  // 0 圆外
  // 1 圆上
  // 2 圆内
  int relationToSeg(line l) {
    double d = l.disPointToSeg(p);
    if (sgn(d - r) < 0)
      return 2;
    else if (sgn(d - r) == 0)
      return 1;
    return 0;
  }

  // 圆和圆的关系
  // 5 相离
  // 4 外切
  // 3 相交
  // 2 内切
  // 1 内含
  int relationToCircle(circle c) {
    double d = distance(p, c.p);
    if (sgn(d - r - c.r) > 0)
      return 5;
    if (sgn(d - r - c.r) == 0)
      return 4;
    double l = fabs(r - c.r);
    if (sgn(d - r - c.r) < 0 && sgn(d - l) > 0)
      return 3;
    if (sgn(d - l) == 0)
      return 2;
    if (sgn(d - l) < 0)
      return 1;
    return -1;
  }
};

// 多边形
struct polygon {
  int n;            // 顶点个数
  vector<point> p;  // 顶点
  vector<line> l;   // 边

  polygon()
      : n(0) {}
  polygon(int _n)
      : n(_n), p(n) {}

  point& operator[](int idx) { return p[idx]; }

  void resize(int _n) {
    n = _n;
    p.resize(n);
  }

  // 多边形周长
  double perimeter() {
    double sum = 0;
    for (int i = 0; i < n; i++)
      sum += (p[(i + 1) % n] - p[i]).length();
    return sum;
  }

  // 多边形面积
  double area() {
    double sum = 0;
    for (int i = 0; i < n; i++)
      sum += det(p[i], p[(i + 1) % n]);
    return fabs(sum) / 2;
  }

  void getline() {
    l.resize(n);
    for (int i = 0; i < n; i++)
      l[i] = line(p[i], p[(i + 1) % n]);
  }

  // 极角排序
  struct cmp {
    point p;
    cmp(const point& _p)
        : p(_p) {}
    bool operator()(const point& a, const point& b) const {
      int d = sgn(det(p, a, b));
      if (d == 0)
        return sgn(distance(a, p) - distance(b, p)) < 0;
      return d > 0;
    }
  };

  // 标准化，即极角排序(逆时针)
  void norm() {
    point mi = p[0];
    for (int i = 1; i < n; i++)
      mi = min(mi, p[i]);
    sort(p.begin(), p.end(), cmp(mi));
  }

  // 凸包(非严格)
  // 若要求严格，则需要再将共线的点除了端点全删去
  polygon getComvex() {
    norm();
    if (n == 0)
      return polygon(0);
    else if (n == 1) {
      polygon convex(1);
      convex[0] = p[0];
      return convex;
    } else if (n == 2) {
      if (p[0] == p[1]) {
        polygon convex(1);
        convex[0] = p[0];
        return convex;
      }
      polygon convex(2);
      convex[0] = p[0];
      convex[1] = p[1];
      return convex;
    }

    polygon convex(n);
    convex.p[0] = p[0];
    convex.p[1] = p[1];
    int top = 2;
    for (int i = 2; i < n; i++) {
      while (top > 1 && sgn(det(convex.p[top - 2], convex.p[top - 1], p[i])) <= 0)
        --top;
      convex.p[top++] = p[i];
    }
    convex.resize(top);
    if (convex.n == 2 && convex.p[0] == convex.p[1])
      convex.resize(1);

    return convex;
  }

  bool isConvex() {
    bool s[3] = {0, 0, 0};
    for (int i = 0, j, k; i < n; i++) {
      j = (i + 1) % n;
      k = (j + 1) % n;
      s[sgn(det(p[i], p[j], p[k])) + 1] = true;
      if (s[0] && s[2])
        return false;
    }
    return true;
  }

  // 多边形方向
  // 1 逆时针
  // 2 顺时针
  int direction() {
    double sum = 0;
    for (int i = 0; i < n; i++)
      sum += det(p[i], p[(i + 1) % n]);
    if (sgn(sum) > 0)
      return 1;
    return 0;
  }

  // 凸包上最远点对
  // 平面最远点对就是点集的凸包上的最远点对
  pair<point, point> getMaxPair() {
    assert(n >= 2);
    if (n == 2)
      return make_pair(p[0], p[1]);
    point p1 = p[0], p2 = p[1];
    double dis = distance(p1, p2);

    // 旋转卡(qia)壳(qiao)
    int k = 1;
    for (int i = 0; i < n; ++i) {
      int j = (i + 1) % n;
      while (sgn(det(p[i], p[j], p[k]) - det(p[i], p[j], p[(k + 1) % n])) <= 0)
        k = (k + 1) % n;

      if (sgn(distance(p[i], p[k]) - dis) > 0)
        p1 = p[i], p2 = p[k], dis = distance(p1, p2);
      if (sgn(distance(p[j], p[k]) - dis) > 0)
        p1 = p[j], p2 = p[k], dis = distance(p1, p2);
    }
    return make_pair(p1, p2);
  }

  double getMaxDis() {
    pair<point, point> pr = getMaxPair();
    return distance(pr.first, pr.second);
  }

  // 平面最近点对(P1257, P1429)
  // 分治法求解平面最近点对，复杂度$ O(n \log n) $
  void __getMinPair(int l, int r, point& p1, point& p2, double& dis) {
    if (r - l <= 9) {
      for (int i = l; i <= r; ++i) {
        for (int j = i + 1; j <= r; ++j) {
          double d = distance(p[i], p[j]);
          if (d < dis) {
            dis = d;
            p1 = p[i];
            p2 = p[j];
          }
        }
      }
      return;
    }

    int m = (l + r) >> 1;
    __getMinPair(l, m, p1, p2, dis);
    __getMinPair(m, r, p1, p2, dis);
    vector<point> tmp;
    for (int i = l; i <= r; ++i)
      if (abs(p[i].x - p[m].x) <= dis)
        tmp.push_back(p[i]);
    sort(tmp.begin(), tmp.end(), [](const point& a, const point& b) {
      return a.y < b.y;
    });
    for (int i = 1; i < (int)tmp.size(); ++i) {
      for (int j = i - 1; j >= 0; --j) {
        if (tmp[j].y < tmp[i].y - dis)
          break;
        double d = distance(tmp[i], tmp[j]);
        if (d < dis) {
          dis = d;
          p1 = tmp[i];
          p2 = tmp[j];
        }
      }
    }
  }

  pair<point, point> getMinPair() {
    assert(n >= 1);
    if (n == 2)
      return make_pair(p[0], p[1]);

    sort(p.begin(), p.end(), [](const point& a, const point& b) {
      return a.x < b.x;
    });
    point p1 = p[0], p2 = p[1];
    double dis = distance(p1, p2);
    __getMinPair(0, n - 1, p1, p2, dis);
    return make_pair(p1, p2);
  }

  double getMinDis() {
    assert(n >= 1);
    if (n == 2)
      return distance(p[0], p[1]);

    sort(p.begin(), p.end(), [](const point& a, const point& b) {
      return a.x < b.x;
    });
    point p1 = p[0], p2 = p[1];
    double dis = distance(p1, p2);
    __getMinPair(0, n - 1, p1, p2, dis);
    return dis;
  }

  // 最小圆覆盖(P2253, P1472)
  // 随机增量法求解最小圆覆盖问题，在随机顺序的点集上，期望复杂度为$ O(n) $
  circle getMinCircle() {
    // 随机打乱顺序
    srand(time(0));
    for (int i = n - 1; i >= 1; --i)
      swap(p[i], p[rand() % i]);

    circle c(p[0], 0);
    for (int i = 0; i < n; ++i) {
      if (c.relationToPoint(p[i]) == 2)
        continue;
      c.p = (p[0] + p[i]) / 2;
      c.r = distance(p[0], p[i]) / 2;

      for (int j = 1; j < i; ++j) {
        if (c.relationToPoint(p[j]) == 2)
          continue;
        c.p = (p[i] + p[j]) / 2;
        c.r = distance(p[i], p[j]) / 2;

        for (int k = 1; k < j; ++k) {
          if (c.relationToPoint(p[k]) == 2)
            continue;
          c = circle(p[i], p[j], p[k]);
        }
      }
    }
    return c;
  }

  // 点与多边形的位置关系
  // 0 外部
  // 1 内部
  // 2 边上
  // 3 点上
  int relationToPoint(point a) {
    for (int i = 0; i < n; ++i)
      if (p[i] == a)
        return 3;

    getline();
    for (int i = 0; i < n; ++i)
      if (l[i].relationToPoint(a) == 3)
        return 2;

    int cnt = 0;
    for (int i = 0, j; i < n; ++i) {
      j = (i + 1) % n;
      int k = sgn(det(p[j], a, p[i]));
      int u = sgn(p[i].y - a.y);
      int v = sgn(p[j].y - a.y);
      if (k > 0 && u < 0 && v >= 0)
        ++cnt;
      if (k < 0 && v < 0 && u >= 0)
        --cnt;
    }
    return cnt != 0;
  }

  void DEBUG() {
    cout << n << endl;
    for (int i = 0; i < n; ++i) {
      cout << p[i].x << " " << p[i].y << endl;
    }
  }
};

// 半平面(ax + by + c >= 0), 其实也就是直线
// 对于直线(s, e), h.s为起点，h.e为方向向量(e - s)
struct halfplane {
  point s, v;
  double k;
  halfplane() {}
  halfplane(point _s, point _v)
      : s(_s), v(_v) {
    k = v.alpha();
  }
  bool operator<(const halfplane& h) const {
    return k < h.k;
  }
};

// 点和半平面的位置关系
// 0 不在右侧
// 1 在右侧
int relationPointToHalfplane(point p, halfplane h) {
  return sgn(det(h.v, p - h.s)) < 0;
}

// 半平面交点
point HalfplaneCrossHalfplane(halfplane h1, halfplane h2) {
  double a = det(h2.v, h1.s - h2.s) / det(h1.v, h2.v);
  return h1.s + h1.v * a;
}

// 从点集构造出半平面集
// 多边形的半平面集即为多边形边集
void getHalfPlanes(polygon& p, vector<halfplane>& h) {
  if (p.direction() != 1)
    reverse(p.p.begin(), p.p.end());
  int n = p.n;
  for (int i = 0, j; i < n; ++i) {
    j = (i + 1) % n;
    h.push_back(halfplane(p[i], p[j] - p[i]));
  }
}

// 有时候题目给的不一定是闭合图形，需要自行添加边界
// (x1, y1)为矩形边界左下角，(x2, y2)为矩形边界右上角
// Usage: addBorderHalfPlanes(0, 0, 1e4, 1e4, h);
// POJ2451
void addBorderHalfPlanes(double x1, double y1, double x2, double y2, vector<halfplane>& h) {
  polygon p(4);
  p[0] = point(x1, y1);
  p[1] = point(x2, y1);
  p[2] = point(x2, y2);
  p[3] = point(x1, y2);
  getHalfPlanes(p, h);
}

// 半平面交
// 排序随机增量法(SI)求解半平面交，复杂度为$ O(n \log n) $
// 瓶颈为排序算法，用基数排序则为$ O(n) $
// 最终的结果为一个凸包，若少于3个点则说明无解

// 多边形的核: 位于多边形内且可以看到多边形内所有点的点集(P5969, POJ1279)
// 多边形的半平面交即为多边形的核(P4196)

bool getHalfPlaneIntersection(vector<halfplane>& h, polygon& hpi) {
  int n = int(h.size()), l, r;
  sort(h.begin(), h.end());

  vector<point> p(n);
  vector<halfplane> q(n);

  l = r = 0;
  q[l] = h[0];
  for (int i = 1; i < n; ++i) {
    while (l < r && relationPointToHalfplane(p[r - 1], h[i]))
      --r;
    while (l < r && relationPointToHalfplane(p[l], h[i]))
      ++l;
    q[++r] = h[i];
    if (l < r && sgn(det(q[r].v, q[r - 1].v)) == 0) {
      --r;
      if (!relationPointToHalfplane(h[i].s, q[r]))
        q[r] = h[i];
    }
    if (l < r)
      p[r - 1] = HalfplaneCrossHalfplane(q[r - 1], q[r]);
  }
  while (l < r && relationPointToHalfplane(p[r - 1], q[l]))
    --r;
  if (r - l + 1 <= 2)
    return false;  // 交不存在
  p[r] = HalfplaneCrossHalfplane(q[l], q[r]);

  hpi.resize(r - l + 1);
  for (int i = l, j = 0; i <= r; ++i)
    hpi[j++] = p[i];

  return true;
}

// 多边形内部半径最大的圆半径(POJ3525)
// 二分半径，对多边形边集向内部进行平移，若平移后的多边形存在核，则可行
double getMaxInsideCircleRadius(polygon& p) {
  if (p.direction() != 1)
    reverse(p.p.begin(), p.p.end());
  int n = p.n;

  // 方向向量, 垂直单位向量
  vector<point> d(n), v(n);
  for (int i = 0; i < n; ++i) {
    d[i] = p[(i + 1) % n] - p[i];
    v[i] = d[i].trans90().unit();
  }

  double l = 0, r = 1e4, m;
  while (r - l >= eps) {
    m = (l + r) / 2;

    vector<halfplane> h(n);
    polygon hpi;
    for (int i = 0; i < n; ++i)
      h[i] = halfplane(p[i] + v[i] * m, d[i]);
    bool can = getHalfPlaneIntersection(h, hpi);

    if (can)
      l = m;
    else
      r = m;
  }
  return l;
}
}  // namespace Geometry
using namespace Geometry;
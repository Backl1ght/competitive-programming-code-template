namespace Geometry3 {
const double eps = 1e-8;

int sgn(double x) {
  if (fabs(x) < eps)
    return 0;
  if (x < 0)
    return -1;
  return 1;
}

struct point3 {
  double x, y, z;
  point3(double _x = 0, double _y = 0, double _z = 0)
      : x(_x), y(_y), z(_z) {}

  bool operator==(const point3& p) const {
    return sgn(x - p.x) == 0 && sgn(y - p.y) == 0 && sgn(z - p.z) == 0;
  }

  bool operator<(const point3& p) const {
    if (sgn(x - p.x) != 0)
      return sgn(x - p.x) < 0;
    if (sgn(y - p.y) != 0)
      return sgn(y - p.y) < 0;
    return sgn(z - p.z) < 0;
  }

  point3 operator-(const point3& p) const {
    return point3(x - p.x, y - p.y, z - p.z);
  }

  point3 operator+(const point3& p) const {
    return point3(x + p.x, y + p.y, z + p.z);
  }

  point3 operator*(const double& a) const {
    return point3(x * a, y * a, z * a);
  }

  point3 operator/(const double& a) const {
    return point3(x / a, y / a, z / a);
  }

  double operator*(const point3& p) const {
    return x * p.x + y * p.y + z * p.z;
  }

  point3 operator^(const point3& p) const {
    return point3(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
  }

  double length() {
    return sqrt(x * x + y * y + z * z);
  }

  double length2() {
    return x * x + y * y + z * z;
  }

  double disTo(const point3& p) {
    return (p - *this).length();
  }

  point3 trunc(double r) {
    double l = length();
    if (sgn(l) == 0)
      return *this;
    r /= l;
    return *this * r;
  }
};

double distance(point3 a, point3 b) {
  return (b - a).length();
}

double distance2(point3 a, point3 b) {
  return (b - a).length2();
}

point3 det(point3 a, point3 b) {
  return a ^ b;
}

point3 det(point3 a, point3 b, point3 c) {
  return (b - a) ^ (c - a);
}

double dot(point3 a, point3 b) {
  return a * b;
}

double dot(point3 a, point3 b, point3 c) {
  return (b - a) * (c - a);
}

// ab与ac之间的夹角
double radian(point3 a, point3 b, point3 c) {
  return acos((b - a) * (c - a)) / (distance(a, b), distance(a, c));
}

// 三角形面积
double triArea(point3 a, point3 b, point3 c) {
  return (det(a, b, c)).length() / 2;
}

double triArea2(point3 a, point3 b, point3 c) {
  return (det(a, b, c)).length();
}

// 四面体有向面积
double QuadVolume(point3 a, point3 b, point3 c, point3 d) {
  return (det(a, b, c) * (d - a)) / 6;
};

double QuadVolume6(point3 a, point3 b, point3 c, point3 d) {
  return det(a, b, c) * (d - a);
};

struct line3 {
  point3 s, e;

  line3(point3 _s = point3(), point3 _e = point3())
      : s(_s), e(_e) {}

  bool operator==(const line3& l) const {
    return (s == l.s) && (e == l.e);
  }

  // 点到直线的距离
  double disPointToLine(point3 p) {
    return det(s, e, p).length() / distance(s, e);
  }

  // 点到线段的距离
  double disPointToSeg(point3 p) {
    if (sgn(dot(s, p, e)) < 0 || sgn(dot(e, p, s)) < 0)
      return min(distance(s, p), distance(e, p));
    return disPointToLine(p);
  }

  // 点在直线上的投影
  point3 projectionPointOnLine(point3 p) {
    return s + (((e - s) * dot(s, e, p)) / (e - s).length2());
  }

  // 绕p旋转alpha度
  point3 rotate(point3 p, double alpha) {
    if (sgn(det(p, s, e).length()) == 0)
      return p;
    point3 p1 = det(s, e, p);
    point3 p2 = det(e - s, p1);
    double len = det(p, s, e).length() / distance(s, e);
    p1 = p1.trunc(len);
    p2 = p2.trunc(len);
    point3 p3 = p + p2;
    point3 p4 = p3 + p1;
    return p3 + ((p - p3) * cos(alpha) + (p4 - p3) * sin(alpha));
  }

  // 点在线段上
  bool isPointOnSeg(point3 p) {
    return sgn(det(p, s, e).length()) == 0 && sgn(dot(p, s, e)) == 0;
  }
};

struct plane {
  point3 a, b, c;  // 3点确定平面
  point3 o;        // 平面的法向量

  point3 pvec() {
    return det(a, b, c);
  }

  plane(point3 _a, point3 _b, point3 _c)
      : a(_a), b(_b), c(_c) {}

  plane(point3 _a, point3 _o)
      : a(_a), o(_o) {}

  // ax + by + cz + d = 0;
  plane(double _a, double _b, double _c, double _d) {
    o = point3(_a, _b, _c);
    if (sgn(_a) != 0)
      a = point3((-_d - _c - _b) / _a, 1, 1);
    else if (sgn(_b) != 0)
      a = point3(1, (-_d - _c - _a) / _b, 1);
    else if (sgn(_c != 0))
      a = point3(1, 1, (-_d - _b - _a) / _c);
  }

  // 点在平面上
  bool isPointOnPlane(point3 p) {
    return sgn((p - a) * o) == 0;
  }

  // 两平面夹角
  double angle(plane f) {
    return acos(o * f.o) / (o.length() * f.o.length());
  }

  // 平面和直线是否相交
  int PlaneCrossLine(line3 l, point3& p) {
    double x = o * (l.e - a);
    double y = o * (l.s - a);
    double d = x - y;
    if (sgn(d) == 0)
      return 0;
    p = ((l.s * x) - (l.e * y)) / d;
    return 1;
  }

  // 点到平面的最近点
  point3 PointToPlane(point3 p) {
    line3 l = line3(p, p + o);
    PlaneCrossLine(l, p);
    return p;
  }

  // 平面和平面是否相交
  int PlaneCrossPlane(plane f, line3& l) {
    point3 o1 = o ^ f.o;
    point3 o2 = o ^ o1;

    double d = fabs(f.o * o2);
    if (sgn(d) == 0)
      return 0;
    point3 p = a + (o2 * (f.o * (f.a - a)) / d);
    l = line3(p, p + o1);
    return 1;
  }
};

struct polygon3 {
  struct face {
    int a, b, c;
    bool ok;
  };

  int n;
  vector<point3> P;

  int num;
  vector<face> F;
  vector<vector<int> > G;

  polygon3()
      : n(0) {}
  polygon3(int _n)
      : n(_n), P(n), F(8 * n), G(n, vector<int>(n)) {}

  double cmp(point3 p, face f) {
    point3 p1 = P[f.b] - P[f.a];
    point3 p2 = P[f.c] - P[f.a];
    point3 p3 = p - P[f.a];
    return (p1 ^ p2) * p3;
  }

  void deal(int p, int a, int b) {
    int f = G[a][b];
    if (F[f].ok) {
      if (cmp(P[p], F[f]) > eps)
        dfs(p, f);
      else {
        face add = {b, a, p, true};
        G[p][b] = G[a][p] = G[b][a] = num;
        F[num++] = add;
      }
    }
  }

  void dfs(int p, int now) {
    F[now].ok = false;
    deal(p, F[now].b, F[now].a);
    deal(p, F[now].c, F[now].b);
    deal(p, F[now].a, F[now].c);
  }

  bool same(int s, int t) {
    point3 a = P[F[s].a];
    point3 b = P[F[s].b];
    point3 c = P[F[s].c];

    bool flag = sgn(QuadVolume6(a, b, c, P[F[t].a])) == 0 &&
                sgn(QuadVolume6(a, b, c, P[F[t].b])) == 0 &&
                sgn(QuadVolume6(a, b, c, P[F[t].c])) == 0;

    return flag;
  }

  void buildConvex3() {
    // step 1: 确保前4点不共面
    bool flag = true;
    for (int i = 1; i < n; ++i) {
      if (!(P[0] == P[i])) {
        swap(P[1], P[i]);
        flag = false;
        break;
      }
    }
    if (flag)
      return;

    flag = true;
    for (int i = 2; i < n; ++i) {
      if (det(P[0], P[1], P[i]).length() > eps) {
        swap(P[2], P[i]);
        flag = false;
        break;
      }
    }
    if (flag)
      return;

    flag = true;
    for (int i = 3; i < n; ++i) {
      if (fabs(det(P[0], P[1], P[2]) * (P[i] - P[0])) > eps) {
        swap(P[3], P[i]);
        flag = false;
        break;
      }
    }
    if (flag)
      return;

    // step 2
    num = 0;
    for (int i = 0; i < 4; ++i) {
      face add = {(i + 1) % 4, (i + 2) % 4, (i + 3) % 4, true};
      if (cmp(P[i], add) > 0)
        swap(add.b, add.c);
      G[add.a][add.b] = G[add.b][add.c] = G[add.c][add.a] = num;
      F[num++] = add;
    }

    for (int i = 4; i < n; ++i) {
      for (int j = 0; j < num; ++j) {
        if (F[j].ok && cmp(P[i], F[j]) > eps) {
          dfs(i, j);
          break;
        }
      }
    }

    int tmp = num;
    num = 0;
    for (int i = 0; i < tmp; ++i)
      if (F[i].ok) {
        F[num++] = F[i];
      }
  }

  // 三维凸包表面积(POJ3528)
  double area() {
    if (n == 3)
      return det(P[0], P[1], P[2]).length() / 2;

    double res = 0;
    for (int i = 0; i < num; ++i)
      res += triArea(P[F[i].a], P[F[i].b], P[F[i].c]);
    return res;
  }

  // 三维凸包体积
  double volume() {
    double res = 0;
    point3 tmp(0, 0, 0);
    for (int i = 0; i < num; ++i)
      res += QuadVolume(tmp, P[F[i].a], P[F[i].b], P[F[i].c]);
    return fabs(res);
  }

  // 表面三角形个数
  double getTriangleCount() {
    return num;
  }

  // 表面多边形个数(HDU3662)
  int getPolygonCount() {
    int res = 0;
    for (int i = 0; i < num; ++i) {
      bool flag = true;
      for (int j = 0; j < i; ++j) {
        if (same(i, j)) {
          flag = 0;
          break;
        }
      }
      res += flag;
    }
    return res;
  }

  // 重心(HDU4273)
  point3 getBaryCenter() {
    point3 ans(0, 0, 0);
    point3 o(0, 0, 0);

    double all = 0;
    for (int i = 0; i < num; ++i) {
      double v = QuadVolume6(o, P[F[i].a], P[F[i].b], P[F[i].c]);
      ans = ans + (((o + P[F[i].a] + P[F[i].b] + P[F[i].c]) / 4) * v);
      all += v;
    }
    ans = ans / all;
    return ans;
  }

  // 点到凸包第i个面上的距离
  double PointToFace(point3 p, int i) {
    double v1 = fabs(QuadVolume6(P[F[i].a], P[F[i].b], P[F[i].c], p));
    double v2 = det(P[F[i].a], P[F[i].b], P[F[i].c]).length();
    return v1 / v2;
  }
};
}  // namespace Geometry3
using namespace Geometry3;
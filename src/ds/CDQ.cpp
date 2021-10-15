// P3810
// 3-D Partial Order($a_j <= a_i and b_j <= b_i && c_j <= c_i$)
#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;

const int N = 1e5 + 5;
const int MAXV = 2e5 + 5;
int k, c[MAXV];
inline int lb(int x) {
  return x & (-x);
}
void add(int x, int d) {
  for (; x <= k; x += lb(x))
    c[x] += d;
}
int getsum(int x) {
  int ret = 0;
  for (; x; x -= lb(x))
    ret += c[x];
  return ret;
}

int n, m;
struct node {
  int a, b, c, w, ans;
  void input() {
    scanf("%d %d %d", &a, &b, &c);
    w = ans = 0;
  }
} a[N], b[N];

int cnt[N];
void CDQ(int l, int r) {
  if (l == r)
    return;
  int mid = (l + r) >> 1;
  CDQ(l, mid);
  CDQ(mid + 1, r);

  sort(a + l, a + 1 + mid, [](const node& x, const node& y) -> bool {
    if (x.b != y.b)
      return x.b < y.b;
    return x.c < y.c;
  });
  sort(a + mid + 1, a + 1 + r, [](const node& x, const node& y) -> bool {
    if (x.b != y.b)
      return x.b < y.b;
    return x.c < y.c;
  });

  int p1 = l, p2 = mid + 1;
  for (; p2 <= r; ++p2) {
    while (a[p1].b <= a[p2].b && p1 <= mid) {
      add(a[p1].c, a[p1].w);
      ++p1;
    }
    a[p2].ans += getsum(a[p2].c);
  }

  for (int i = l; i < p1; ++i)
    add(a[i].c, -a[i].w);
}

int main() {
  scanf("%d %d", &m, &k);
  for (int i = 1; i <= m; ++i)
    b[i].input();

  sort(b + 1, b + 1 + m, [](const node& x, const node& y) -> bool {
    if (x.a != y.a)
      return x.a < y.a;
    if (x.b != y.b)
      return x.b < y.b;
    return x.c < y.c;
  });

  int w = 0;
  for (int i = 1; i <= m; ++i) {
    ++w;
    if (b[i].a != b[i + 1].a || b[i].b != b[i + 1].b || b[i].c != b[i + 1].c) {
      ++n;
      a[n] = b[i];
      a[n].w = w;
      w = 0;
    }
  }
  CDQ(1, n);
  for (int i = 1; i <= n; ++i)
    if (a[i].ans + a[i].w - 1 < m)
      cnt[a[i].ans + a[i].w - 1] += a[i].w;
  for (int i = 0; i < m; ++i)
    printf("%d\n", cnt[i]);
  return 0;
}
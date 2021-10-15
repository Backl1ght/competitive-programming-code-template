template <typename V>
struct LeftistForest {
  struct LeftistTree {
    V v;
    int dist;
    int l, r, rt;
  } t[N];
  LeftistTree& operator[](int x) { return t[x]; }
  void init(int n, V* a) {
    FOR(i, 1, n) {
      t[i].v = a[i];
      t[i].l = t[i].r = t[i].dist = 0;
      t[i].rt = i;
    }
  }
  int find(int x) { return t[x].rt == x ? x : t[x].rt = find(t[x].rt); }
  int merge(int x, int y) {
    if (!x)
      return y;
    if (!y)
      return x;
    if (t[x].v > t[y].v)
      swap(x, y);  // 小根堆
    t[x].r = merge(t[x].r, y);
    t[t[x].r].rt = x;
    if (t[t[x].l].dist < t[t[x].r].dist)
      swap(t[x].l, t[x].r);
    if (!t[x].r)
      t[x].dist = 0;
    else
      t[x].dist = t[t[x].r].dist + 1;
    return x;
  }
  V top(int x) {
    if (t[x].v == -1)
      return -1;
    x = find(x);
    return t[x].v;
  }
  void pop(int x) {
    if (t[x].v == -1)
      return;
    x = find(x);
    t[t[x].l].rt = t[x].l;
    t[t[x].r].rt = t[x].r;
    t[x].rt = merge(t[x].l, t[x].r);
    t[x].v = -1;
  }
};

int n, m, a[N];
void solve(int Case) {
  rd(n, m);
  FOR(i, 1, n)
  rd(a[i]);
  LeftistForest<int> T;
  T.init(n, a);

  int op, x, y;
  FOR(_, 1, m) {
    rd(op);
    debug(op);
    if (op == 1) {
      rd(x, y);
      if (T[x].v == -1 || T[y].v == -1)
        continue;
      x = T.find(x);
      y = T.find(y);
      if (x == y)
        continue;
      T[x].rt = T[y].rt = T.merge(x, y);
    } else {
      rd(x);
      pln(T.top(x));
      T.pop(x);
    }
  }
}
template <typename T>
struct ScapegoatTree {
  static constexpr double alpha = 0.75;  // alpha \in (0.5, 1)
  int root, tot, buf_size;
  T v[N];
  int s[N], sz[N], sd[N], cnt[N], l[N], r[N], buf[N];

  ScapegoatTree() {
    root = tot = 0;
  }

  int new_node(T _v) {
    ++tot;
    v[tot] = _v;
    s[tot] = sz[tot] = sd[tot] = cnt[tot] = 1;
    l[tot] = r[tot] = 0;
    return tot;
  }

  void push_up(int x) {
    if (!x)
      return;
    int lc = l[x], rc = r[x];
    s[x] = s[lc] + 1 + s[rc];
    sz[x] = sz[lc] + cnt[x] + sz[rc];
    sd[x] = sd[lc] + (cnt[x] != 0) + sd[rc];
  }

  bool balance(int x) {
    int lc = l[x], rc = r[x];
    if (alpha * s[x] <= max(s[lc], s[rc]))
      return false;
    if (alpha * s[x] >= sd[x])
      return false;
    return true;
  }

  void flatten(int x) {
    if (!x)
      return;
    flatten(l[x]);
    if (cnt[x])
      buf[++buf_size] = x;
    flatten(r[x]);
  }

  void build(int& x, int L, int R) {
    if (L > R) {
      x = 0;
      return;
    }
    int mid = (L + R) >> 1;
    x = buf[mid];
    build(l[x], L, mid - 1);
    build(r[x], mid + 1, R);
    push_up(x);
  }

  void rebuild(int& x) {
    buf_size = 0;
    flatten(x);
    build(x, 1, buf_size);
  }

  void ins(int& rt, T val) {
    if (!rt) {
      rt = new_node(val);
      return;
    }
    if (val == v[rt]) {
      ++cnt[rt];
    } else if (val < v[rt]) {
      ins(l[rt], val);
    } else {
      ins(r[rt], val);
    }
    push_up(rt);
    if (!balance(rt))
      rebuild(rt);
  }

  void del(int& rt, T val) {
    if (!rt)
      return;

    if (val == v[rt]) {
      if (cnt[rt])
        --cnt[rt];
    } else if (val < v[rt]) {
      del(l[rt], val);
    } else {
      del(r[rt], val);
    }
    push_up(rt);
    if (!balance(rt))
      rebuild(rt);
  }

  int getPrevRank(int rt, T val) {
    if (!rt)
      return 0;
    if (v[rt] == val && cnt[rt])
      return sz[l[rt]];
    if (v[rt] < val)
      return sz[l[rt]] + cnt[rt] + getPrevRank(r[rt], val);
    return getPrevRank(l[rt], val);
  }

  int getSuccRank(int rt, T val) {
    if (!rt)
      return 1;
    if (v[rt] == val && cnt[rt])
      return sz[l[rt]] + cnt[rt] + 1;
    if (v[rt] < val)
      return sz[l[rt]] + cnt[rt] + getSuccRank(r[rt], val);
    return getSuccRank(l[rt], val);
  }

  T getKth(int rt, int k) {
    if (!rt)
      return 0;
    if (k <= sz[l[rt]])
      return getKth(l[rt], k);
    if (k - sz[l[rt]] <= cnt[rt])
      return v[rt];
    return getKth(r[rt], k - sz[l[rt]] - cnt[rt]);
  }

  void ins(T val) {
    ins(root, val);
  }

  void del(T val) {
    del(root, val);
  }

  int getRank(T val) {
    return getPrevRank(root, val) + 1;
  }

  T getKth(int k) {
    return getKth(root, k);
  }

  T getPrev(T val) {
    return getKth(getPrevRank(root, val));
  }

  T getSucc(T val) {
    return getKth(getSuccRank(root, val));
  }

  void debug(int x) {
    if (!x)
      return;
    debug(l[x]);
    cerr << v[x] << " ";
    debug(r[x]);
  }

  void debug() {
    cerr << "ScapegoatTree:" << endl;
    debug(root);
    cerr << endl;
  }
};
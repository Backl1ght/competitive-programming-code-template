// Kuhn Munkres, O(V^3)
template <typename T>
struct biwraph {
  T TMAX, TMIN;

  int n, nl, nr;
  vector<vector<T>> G;
  vector<T> highl, highr;
  vector<T> slack;
  vector<int> matchl, matchr;  // match
  vector<int> pre;             // pre node
  vector<bool> visl, visr;     // vis
  vector<int> q;
  int ql, qr;

  biwraph(int _nl, int _nr) {
    TMAX = numeric_limits<T>::max();

    nl = _nl;
    nr = _nr;
    n = max(nl, nr);
    G = vector<vector<T>>(n + 1, vector<T>(n + 1));
    highl = vector<T>(n + 1);
    highr = vector<T>(n + 1);
    slack = vector<T>(n + 1);
    matchl = vector<int>(n + 1);
    matchr = vector<int>(n + 1);
    pre = vector<int>(n + 1);
    visl = vector<bool>(n + 1);
    visr = vector<bool>(n + 1);
    q = vector<int>(n + 1);
  }

  void addarc(int u, int v, T w) {
    G[u][v] = max(G[u][v], w);
  }

  bool check(int v) {
    visr[v] = true;
    if (matchr[v]) {
      q[qr++] = matchr[v];
      visl[matchr[v]] = true;
      return false;
    }

    while (v) {
      matchr[v] = pre[v];
      swap(v, matchl[pre[v]]);
    }

    return true;
  }

  void bfs(int now) {
    ql = qr = 0;
    q[qr++] = now;
    visl[now] = 1;
    while (true) {
      while (ql < qr) {
        int u = q[ql++];
        for (int v = 1; v <= n; ++v) {
          if (!visr[v]) {
            T delta = highl[u] + highr[v] - G[u][v];
            if (slack[v] >= delta) {
              pre[v] = u;
              if (delta)
                slack[v] = delta;
              else if (check(v))
                return;
            }
          }
        }
      }

      T a = TMAX;
      for (int i = 1; i <= n; ++i)
        if (!visr[i])
          a = min(a, slack[i]);
      for (int i = 1; i <= n; ++i) {
        if (visl[i])
          highl[i] -= a;
        if (visr[i])
          highr[i] += a;
        else
          slack[i] -= a;
      }
      for (int i = 1; i <= n; ++i)
        if (!visr[i] && !slack[i] && check(i))
          return;
    }
  }

  void match() {
    fill(highr.begin(), highr.end(), 0);
    fill(matchl.begin(), matchl.end(), 0);
    fill(matchr.begin(), matchr.end(), 0);
    for (int i = 1; i <= n; ++i)
      highl[i] = *max_element(G[i].begin() + 1, G[i].end());

    for (int i = 1; i <= n; ++i) {
      fill(slack.begin(), slack.end(), TMAX);
      fill(visl.begin(), visl.end(), false);
      fill(visr.begin(), visr.end(), false);
      bfs(i);
    }
  }

  T getMaxMatch() {
    T res = 0;
    match();
    for (int i = 1; i <= n; ++i) {
      if (G[i][matchl[i]] > 0)
        res += G[i][matchl[i]];
      else
        matchl[i] = 0;
    }
    return res;
  }
};
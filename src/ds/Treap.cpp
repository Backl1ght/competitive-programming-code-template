namespace Treap {
    using T = long long;
    const int S = N;
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    int tot, rt, sz[S], L[S], R[S], rnd[S];

    T v[S];

    inline void init() {
        tot = rt = 0;
    }

    inline int newnode(T val) {
        ++tot;
        sz[tot] = 1;
        L[tot] = R[tot] = 0;
        rnd[tot] = rng();
        v[tot] = val;
        return tot;
    }

    inline void push_up(int x) {
        sz[x] = sz[L[x]] + 1 + sz[R[x]];
    }

    void split(int u, T k, int &x, int &y) {
        if (!u) x = y = 0;
        else {
            if (v[u] <= k) {
                x = u;
                split(R[u], k, R[u], y);
            } else {
                y = u;
                split(L[u], k, x, L[u]);
            }
            push_up(u);
        }
    }

    int merge(int x, int y) {
        if (!x || !y) return x | y;
        if (rnd[x] < rnd[y]) {
            R[x] = merge(R[x], y);
            push_up(x);
            return x;
        } else {
            L[y] = merge(x, L[y]);
            push_up(y);
            return y;
        }
    }

    void insert(T val) {
        int x, y;
        split(rt, val, x, y);
        x = merge(x, newnode(val));
        rt = merge(x, y);
    }

    void remove(T val) {
        int x1, y1, x2, y2;
        split(rt, val, x1, y1);
        split(x1, val - 1, x2, y2);
        y2 = merge(L[y2], R[y2]);
        x1 = merge(x2, y2);
        rt = merge(x1, y1);
    }

    int getrank(T val) {
        int x, y;
        split(rt, val - 1, x, y);
        int res = sz[x] + 1;
        rt = merge(x, y);
        return res;
    }

    T getkth(int k) {
        int u = rt;
        while(true) {
            if (k <= sz[L[u]]) u = L[u];
            else {
                if (sz[L[u]] + 1 == k) break;
                else k -= sz[L[u]] + 1, u = R[u];
            }
        }
        return v[u];
    }

    T getpre(T val) {
        int x, y;
        split(rt, val - 1, x, y);
        int p = x;
        while(R[p]) p = R[p];
        rt = merge(x, y);
        return v[p];
    }

    T getsuc(T val) {
        int x, y;
        split(rt, val, x, y);
        int p = y;
        while(L[p]) p = L[p];
        rt = merge(x, y);
        return v[p];
    }

    void DEBUG(int u) {
        if (!u) return;
        DEBUG(L[u]);
        cerr << v[u] << " ";
        DEBUG(R[u]);
    }

    void DEBUG() {
        cerr << "Treap: ";
        DEBUG(rt);
        cerr << endl;
    }
}
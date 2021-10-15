template<typename T>
struct FenwickTree {
    int n;
    vector<T> c;
    FenwickTree(int _n) : n(_n), c(n + 1) {}
    inline int lb(int x) { return x & -x; }
    void add(int x, T d) { for (; x <= n; x += lb(x)) c[x] += d; }
    T getsum(int x) { T r = 0; for (; x; x -= lb(x)) r += c[x]; return r; }
    T getsum(int l, int r) { return getsum(r) - getsum(l - 1); }
    T kth(int k) {
        T ans = 0, cnt = 0;
        for (int i = __lg(n) + 1; i >= 0; --i) {
            ans += (1LL << i);
            if (ans >= n || cnt + c[ans] >= k) ans -= (1LL << i);
            else cnt += c[ans];
        }
        return ans + 1;
    }
};
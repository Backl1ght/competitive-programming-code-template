namespace Backlight {

namespace FenwickTree {
    using T = long long;
    T c[N];
    inline int lb(int x) { return x & -x; }
    void add(int x, T d) { for (; x < N; x += lb(x)) c[x] += d; }
    T getsum(int x) { T r = 0; for (; x; x -= lb(x)) r += c[x]; return r; }
    T getsum(int l, int r) { return getsum(r) - getsum(l - 1); }
} // namespace FenwickTree
#define BIT FenwickTree

} // namespace Backlight
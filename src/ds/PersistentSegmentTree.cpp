namespace Backlight {

namespace PersistentSegmentTree {
    using T = ll;

    T s[N << 5];
    int tot, L[N << 5], R[N << 5];

    int update(int rt, int l, int r, int p) {
        int nrt = ++tot;
        L[nrt] = L[rt]; R[nrt] = R[rt]; s[nrt] = s[rt] + 1;
        if (l != r) {
            int mid = (l + r) >> 1;
            if (p <= mid) L[nrt] = update(L[rt], l, mid, p);
            else R[nrt] = update(R[rt], mid + 1, r, p);
        }
        return nrt;
    }

    // 区间第k小
    int query(int u, int v, int l, int r, int k) {
        if (l == r) return l;
        int lsum = s[L[v]] - s[L[u]], mid = (l + r) >> 1;;
        if (k <= lsum) return query(L[u], L[v], l, mid, k);
        return query(R[u], R[v], mid + 1, r, k - lsum);
    }
} // namespace PersistentSegmentTree
#define PST PersistentSegmentTree

} // namespace Backlight
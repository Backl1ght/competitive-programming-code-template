namespace Backlight {

namespace SegmentTree {
    using T = long long;

    #define ls (x << 1)
    #define rs (x << 1 | 1)
    #define mid ((l + r) >> 1)
    #define lson ls, l, mid
    #define rson rs, mid+1, r

    T v[N << 2];

    void change(int x, int l, int r, int val) {

    }

    void push_up(int x) {

    }

    void push_down(int x, int l, int r) {

    }

    void build(int x, int l, int r, T* a) {
        if (l == r) {
            v[x] = a[l];
            return;
        }
        build(lson); build(rson);
        push_up(x);
    }

    T queryPoint(int x, int l, int r, int p) {
        if (l == r) {
            assert(l == p);
            return v[x];
        }
        push_down(x, l, r);
        if (p <= mid) return queryPoint(lson, p);
        return queryPoint(rson, p);
    }

    T querySegment(int x, int l, int r, int L, int R) {
        if (l == L && r == R) return v[x];
        push_down(x, l, r);
        if (R <= mid) return querySegment(lson, L, R);
        else if (L > mid) return querySegment(rson, L, R);
        return querySegment(lson, L, mid) + querySegment(rson, mid + 1, R);
    }

    void changePoint(int x, int l, int r, int p, int val) {
        if (l == r) {
            assert(l == p);
            // make change here
            return;
        }
        push_down(x, l, r);
        if (p <= mid) changePoint(lson, p, val);
        else changePoint(rson, p, val);
        push_up(x);
    }

    void changeSegment(int x, int l, int r, int L, int R, int val) {
        if (l == L && r == R) {
            change(x, l, r, val);
            return;
        }
        push_down(x, l, r);
        if (R <= mid) changeSegment(lson, L, R, val);
        else if (L > mid) changeSegment(rson, L, R, val);
        else {
            changeSegment(lson, L, mid, val);
            changeSegment(rson, mid + 1, R, val);
        }
        push_up(x);
    }
} // namespace SegmentTree

} // namespace Backlight
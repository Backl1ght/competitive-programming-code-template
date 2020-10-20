namespace SegmentTree {
    #define ls (x<<1)
    #define rs (x<<1|1)
    #define mid ((l+r)>>1)
    #define lson ls,l,mid
    #define rson rs,mid+1,r

    T v[N << 2];

    void change(int x, int val) {

    }

    void push_up(int x) {

    }

    void push_down(int x) {

    }

    void build(int x, int l, int r) {
        if (l == r) {
            // do something here
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
        push_down(x);
        if (p <= mid) return queryPoint(lson, p);
        return queryPoint(rson, p);
    }

    T querySegment(int x, int l, int r, int L, int R) {
        if (l == L && r == R) return v[x];
        push_down(x);
        if (R <= mid) return querySegment(lson, L, R);
        else if (L > mid) return querySegment(rson, L, R);
        return querySegment(lson, L, mid) + querySegment(rson, mid + 1, R);
    }

    void changePoint(int x, int l, int r, int p, int val) {
        if (l == r) {
            assert(l == p);
            // make change hear
            return;
        }
        push_down(x);
        if (p <= mid) changePoint(lson, p, val);
        else changePoint(rson, p, val);
        push_up(x);
    }

    void changeSegment(int x, int l, int r, int L, int R, int val) {
        if (l == L && r == R) {
            // make change here
            return;
        }
        push_down(x);
        if (R <= mid) changeSegment(lson, L, R, val);
        else if (L > mid) changeSegment(rson, L, R, val);
        else {
            changeSegment(lson, L, mid, val);
            changeSegment(rson, mid + 1, R, val);
        }
        push_up(x);
    }
}
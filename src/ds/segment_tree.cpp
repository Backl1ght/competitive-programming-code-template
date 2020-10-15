namespace SegmentTree{
    #define ls (x<<1)
    #define rs (x<<1|1)
    #define mid ((l+r)>>1)
    #define lson ls,l,mid
    #define rson rs,mid+1,r

    T v[N << 2];

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
        if (l == r && l == p) return v[x];
        push_down(x);
        if (p <= mid) return queryPoint(lson, p);
        return queryPoint(rson, p);
    }

    T querySegment(int x, int l, int r, int L, int R) {
        if (l == L && r == R) return v[x];
        push_down(x);
        if (R <= mid) return querySegment(lson, L, R);
        else if (L > mid) return querySegment(rson, L, R);
        else {
            return querySegment(lson, L, mid) + querySegment(rson, mid + 1, R);
        }
    }
}
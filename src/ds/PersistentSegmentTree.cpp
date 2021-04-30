struct PersistentSegmentTree
{
// $SIZE = N \log N$
#define SIZE 200005 * 20

    int tot;
    int c[SIZE];
    int L[SIZE], R[SIZE];

    PersistentSegmentTree() { tot = 0; }

    int update(int rt, int l, int r, int p, int d)
    {
        int nrt = ++tot;
        L[nrt] = L[rt];
        R[nrt] = R[rt];
        c[nrt] = c[rt] + d;

        if (l != r)
        {
            int mid = (l + r) >> 1;
            if (p <= mid)
                L[nrt] = update(L[rt], l, mid, p, d);
            else
                R[nrt] = update(R[rt], mid + 1, r, p, d);
        }

        return nrt;
    }

    // 区间第k小
    int query(int u, int v, int l, int r, int k)
    {
        if (l == r)
            return l;
        int left_size = c[L[v]] - c[L[u]];
        int mid = (l + r) >> 1;
        if (k <= left_size)
            return query(L[u], L[v], l, mid, k);
        return query(R[u], R[v], mid + 1, r, k - left_size);
    }
};
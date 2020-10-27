// Captain Mo
// 询问[l, r]内的元素是否互不相同
int Ans, ans[N];
int block_sz, block_id[N];
struct Query {
    int l, r, id;
    Query() {}
    Query(int _l, int _r, int _id) : l(_l), r(_r), id(_id) {}
    bool operator < (const Query& q) const {
        if (block_id[l] == block_id[q.l]) 
            return block_id[l] & 1 ? r < q.r : r > q.r;
        return block_id[l] < block_id[q.l];
    }
} Q[N];

int n, q, a[N];

int cnt[N], ge2;
inline void add(int p) {
    ++cnt[a[p]];
    if (cnt[a[p]] == 2) ++ge2;
}

inline void del(int p) {
    if (cnt[a[p]] == 2) --ge2;
    --cnt[a[p]];
}

void CaptainMo() {
    block_sz = sqrt(n);
    for (int i = 1; i <= n; ++i) block_id[i] = i / block_sz;
    sort(Q + 1, Q + 1 + q);

    int l = 1, r = 0;
    ge2 = 0;
    for (int i = 1; i <= q; ++i) {
        while(r < Q[i].r) ++r, add(r);
        while(l < Q[i].l) del(l), ++l;
        while(l > Q[i].l) --l, add(l);
        while(r > Q[i].r) del(r), --r;
        ans[Q[i].id] = (ge2 == 0);
    }
}
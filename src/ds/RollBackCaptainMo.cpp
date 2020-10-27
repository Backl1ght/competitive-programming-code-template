// Roll Back Captain Mo
// 询问[l, r]内值相同的元素的最远距离
int Ans, ans[N];
int block_sz, block_cnt, block_id[N], L[N], R[N];
struct Query {
    int l, r, id;
    Query() {}
    Query(int _l, int _r, int _id) : l(_l), r(_r), id(_id) {}
    bool operator < (const Query& q) const {
        if (block_id[l] == block_id[q.l]) return r < q.r;
        return block_id[l] < block_id[q.l];
    }
} Q[N];

int n, m, q, a[N], b[N];


int nums[N], cn;
int mi[N], ma[N];
int __mi[N];

int brute_force(int l, int r) {
    int res = 0;
    for (int i = l; i <= r; ++i) __mi[a[i]]= 0;
    for (int i = l; i <= r; ++i) {
        if (__mi[a[i]]) res = max(res, i - __mi[a[i]]);
        else __mi[a[i]] = i;
    }
    return res;
}

inline void addl(int p) {
    if (ma[a[p]]) Ans = max(Ans, ma[a[p]] - p);
    else ma[a[p]] = p;
}

inline void addr(int p) {
    ma[a[p]] = p;
    if (!mi[a[p]]) mi[a[p]] = p, nums[++cn] = a[p];
    Ans = max(Ans, p - mi[a[p]]);
}

inline void dell(int p) {
    if (ma[a[p]] == p) ma[a[p]] = 0;
}

inline void delr(int p) {

}

inline void clear() {
    for (int i = 1; i <= cn; ++i) mi[nums[i]] = ma[nums[i]] = 0;
}

void RollBackCaptainMo() {
    block_sz = sqrt(n); block_cnt = n / block_sz;

    for (int i = 1; i <= block_cnt; ++i) L[i] = R[i - 1] + 1, R[i] = i * block_sz;
    if (R[block_cnt] < n) { ++block_cnt; L[block_cnt] = R[block_cnt - 1] + 1; R[block_cnt] = n; }

    for (int i = 1; i <= block_cnt; ++i)
        for (int j = L[i]; j <= R[i]; ++j)
            block_id[j] = i;

    sort(Q + 1, Q + 1 + q);

    for (int i = 1, j = 1; j <= block_cnt; ++j) {
        int l = R[j] + 1, r = R[j];
        Ans = 0; cn = 0;
        for (; block_id[Q[i].l] == j; ++i) {
            if (block_id[Q[i].l] == block_id[Q[i].r]) ans[Q[i].id] = brute_force(Q[i].l, Q[i].r);
            else {
                while(r < Q[i].r) ++r, addr(r);
                int tmp = Ans;
                while(l > Q[i].l) --l, addl(l);
                ans[Q[i].id] = Ans;
                while(l <= R[j]) dell(l), ++l;
                Ans = tmp;
            }
        }
        clear();
    }
}
class segtree {
public:
    struct node {
        // 声明变量，记得设置初始值
        // ie. 最大值: int mx = INT_MIN;

        ...

        void apply(int l, int r, ll addv) {
            // 更新节点信息
            // ie. 最大值+区间加: mx = mx + addv
            
            ...
        }
    };

    friend node operator + (const node& tl, const node& tr) {
        node t;
        // 合并两个区间的信息
        // ie. 区间和: t.sum = t1.sum + t2.sum;

        ...

        return t;
    }

    inline void push_down(int x, int l, int r) {
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        // 标记下传
        // ie. 区间加法
        // if (tr[x].add != 0) {
        //     tr[lc].apply(l, mid, tr[x].add);
        //     tr[rc].apply(mid + 1, r, tr[x].add);
        //     tr[x].add = 0;
        // }

        ...
    }

    /************************************************************************/
    inline void push_up(int x) {
        int lc = x << 1, rc = lc | 1;
        tr[x] = tr[lc] + tr[rc];
    }

    int n;
    vector<node> tr;

    void build(int x, int l, int r) {
        if (l == r) {
            return;
        }
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        build(lc, l, mid);
        build(rc, mid + 1, r);
        push_up(x);
    }

    template<class T>
    void build(int x, int l, int r, const vector<T>& arr){
        if (l == r) {
            tr[x].apply(l, r, arr[l]);
            return;
        }
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        build(lc, l, mid, arr);
        build(rc, mid + 1, r, arr);
        push_up(x);
    }

    template<class T>
    void build(int x, int l, int r, T* arr){
        if (l == r) {
            tr[x].apply(l, r, arr[l]);
            return;
        }
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        build(lc, l, mid);
        build(rc, mid + 1, r);
        push_up(x);
    }

    node get(int x, int l, int r, int L, int R) {
        if (L <= l && r <= R) {
            return tr[x];
        }
        push_down(x, l, r);
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        node res;
        if (R <= mid) res = get(lc, l, mid, L, R);
        else if (L > mid) res = get(rc, mid + 1, r, L, R);
        else res = get(lc, l, mid, L, mid) + get(rc, mid + 1, r, mid + 1, R);
        push_up(x);
        return res;
    }

    template<class... T>
    void upd(int x, int l, int r, int L, int R, const T&... v) {
        if (L <= l && r <= R) {
            tr[x].apply(l, r, v...);
            return;
        }
        push_down(x, l, r);
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        node res;
        if (L <= mid) upd(lc, l, mid, L, R, v...);
        if (R > mid) upd(rc, mid + 1, r, L, R, v...);
        push_up(x);
    }

    int __get_first(int x, int l, int r, const function<bool(const node&)> &f) {
        if (l == r) {
            return l;
        }
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        push_down(x, l, r);
        int res;
        if (f(tr[lc])) res = __get_first(lc, l, mid, f);
        else res = __get_first(rc, mid + 1, r, f);
        push_up(x);
        return res;
    }

    int get_first(int x, int l, int r, int L, int R, const function<bool(const node&)> &f) {
        if (L <= l && r <= R) {
            if (!f(tr[x])) {
                return -1;
            }
            return __get_first(x, l, r, f);
        }
        push_down(x, l, r);
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        int res;
        if (L <= mid) res = get_first(lc, l, mid, L, R, f);
        if (res == -1 && R > mid) res = get_first(rc, mid + 1, r, L, R, f);
        push_up(x);
        return res;
    }

    int __get_last(int x, int l, int r, const function<bool(const node&)> &f) {
        if (l == r) {
            return l;
        }
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        push_down(x, l, r);
        int res;
        if (f(tr[lc])) res = __get_first(rc, mid + 1, r, f);
        else res = __get_first(lc, l, mid, f);
        push_up(x);
        return res;
    }

    int get_last(int x, int l, int r, int L, int R, const function<bool(const node&)> &f) {
        if (L <= l && r <= R) {
            if (!f(tr[x])) {
                return -1;
            }
            return __get_first(x, l, r, f);
        }
        push_down(x, l, r);
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        int res;
        if (R > mid) res = get_last(rc, mid + 1, r, L, R, f);
        if (res == -1 && L <= mid) res = get_last(lc, l, mid, L, R, f);
        push_up(x);
        return res;
    }

    int find_first(int l, int r, const function<bool(const node&)> &f) {
        int L = l, R = r, mid, res = -1;
        while(L <= R) {
            mid = (L + R) >> 1;
            if (f(get(l, mid))) R = mid - 1, res = mid;
            else L = mid + 1;
        }
        return res;
    }
 
    int find_last(int l, int r, const function<bool(const node&)> &f) {
        int L = l, R = r, mid, res = -1;
        while(L <= R) {
            mid = (L + R) >> 1;
            if (f(get(l, mid))) L = mid + 1, res = mid;
            else R = mid - 1;
        }
        return res;
    }

    segtree(int _n) : n(_n) {
        assert(n > 0);
        tr.resize((n << 2) + 5);
        build(1, 1, n);
    }

    template<class T>
    segtree(const vector<T>& arr) {
        n = arr.size() - 1;
        assert(n > 0);
        tr.resize((n << 2) + 5);
        build(1, 1, n, arr);
    }

    template<class T>
    segtree(int _n, T* arr) {
        n = _n;
        assert(n > 0);
        tr.resize((n << 2) + 5);
        build(1, 1, n, arr);
    }

    node get(int l, int r) {
        assert(l >= 1 && l <= r && r <= n);
        return get(1, 1, n, l, r);
    }

    node get(int p) {
        assert(1 <= p && p <= n);
        return get(1, 1, n, p, p);
    }
    
    template <class... T> 
    void upd(int l, int r, const T&... v) {
        assert(l >= 1 && l <= r && r <= n);
        upd(1, 1, n, l, r, v...);
    }

    template <class... T> 
    void upd1(int p, const T&... v) {
        assert(p >= 1 && p <= n);
        upd(1, 1, n, p, p, v...);
    }

    int get_first(int l, int r, const function<bool(const node&)> &f) {
        assert(l >= 1 && l <= r && r <= n);
        return get_first(1, 1, n, l, r, f);
    }

    
    int get_last(int l, int r, const function<bool(const node&)> &f) {
        assert(l >= 1 && l <= r && r <= n);
        return get_last(1, 1, n, l, r, f);
    }

    void print(int x, int l, int r) {
        if (l == r) {
            cerr << tr[x].sum << " ";
            return;
        }
        push_down(x, l, r);
        int lc = x << 1, rc = lc | 1, mid = (l + r) >> 1;
        print(lc, l, mid);
        print(rc, mid + 1, r);
    }

    void print() {
        #ifdef BACKLIGHT
        cerr << "SEGTREE: " << endl;
        print(1, 1, n);
        cerr << "\n-----------------------------" << endl;
        #endif
    }
};
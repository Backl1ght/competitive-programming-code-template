namespace Backlight {

namespace Splay {
    using T = int;
    #define ls ch[x][0]
    #define rs ch[x][1]
    const int S = N;

    int tot, rt, sz[S], cnt[S], ch[S][2], fa[S];

    T v[S];

    inline void init() { tot = rt = 0; }

    inline void clear(int x) { ch[x][0] = ch[x][1] = fa[x] = sz[x] = cnt[x] = v[x] = 0; }

    inline int get(int x) { return ch[fa[x]][1] == x; }

    inline int newnode(T val) {
        ++tot;
        sz[tot] = cnt[tot] = 1;
        ch[tot][0] = ch[tot][1] = fa[tot] = 0;
        v[tot] = val;
        return tot;
    }

    inline void push_up(int x) {
        if (!x) return;
        sz[x] = sz[ls] + cnt[x] + sz[rs];
    }

    void rotate(int x) {
        int f = fa[x], g = fa[f], i = get(x);
        ch[f][i] = ch[x][i^1]; fa[ch[f][i]] = f;
        ch[x][i^1] = f; fa[f] = x;
        fa[x] = g;
        if (g) ch[g][ch[g][1] == f] = x;
        push_up(f); push_up(x);
    }

    void splay(int x, int ed) {
        for (int f; (f = fa[x]) != ed; rotate(x))
            if (fa[f] != ed) rotate((get(x) == get(f) ? f : x));
        if (ed == 0) rt = x;
    }


    void insert(T val) {
        if (rt == 0) { rt = newnode(val); return; }
        int p = rt, f = 0;
        while(true) {
            if (val == v[p]) {
                ++cnt[p];
                push_up(p); push_up(f);
                break;
            }
            f = p;
            p = ch[p][v[p] < val];
            if (p == 0) {
                p = newnode(val);
                fa[p] = f; ch[f][v[f] < val] = p;
                push_up(f);
                break;
            }
        }
        splay(p, 0);
    }

    int getrank(T val) {
        int p = rt, res = 0;
        while(p) {
            if (v[p] > val) p = ch[p][0];
            else {
                res += sz[ch[p][0]];
                if (v[p] == val) break;
                res += cnt[p];
                p = ch[p][1];
            }
        }
        assert(p != 0);
        splay(p, 0); 
        return res + 1;
    }

    T getkth(int k) {
        int p = rt, res = 0;
        while(p) {
            if (k <= sz[ch[p][0]]) p = ch[p][0];
            else {
                if (k <= sz[ch[p][0]] + cnt[p]) { res = v[p]; break; }
                else k -= sz[ch[p][0]] + cnt[p], p = ch[p][1];
            }
        }
        assert(p != 0);
        splay(p, 0); 
        return res;
    }

    void remove(T val) {
        getrank(val); // splay val to root
        if (cnt[rt] > 1) { --cnt[rt]; push_up(rt); return; }
        if (!ch[rt][0] && !ch[rt][1]) { clear(rt); rt = 0; return; }
        if (!ch[rt][0] || !ch[rt][1]) {
            int nrt = ch[rt][0] ? ch[rt][0] : ch[rt][1];
            clear(rt); rt = nrt; fa[rt] = 0;
            return;
        }
        int ort = rt;
        int p = ch[rt][0]; while(ch[p][1]) p = ch[p][1];
        splay(p, 0);
        ch[rt][1] = ch[ort][1];
        fa[ch[ort][1]] = rt;
        clear(ort);
        push_up(rt);
    }

    T getpre(T val) {
        int p = rt, res = -INF;
        while(p) {
            if (v[p] < val && v[p] > res) res = v[p];
            if (val > v[p]) p = ch[p][1];
            else p = ch[p][0];
        }
        // splay(p, 0);
        return res;
    }

    T getsuc(T val) {
        int p = rt, res = INF;
        while(p) {
            if (v[p] > val && v[p] < res) res = v[p];
            if (val < v[p]) p = ch[p][0];
            else p = ch[p][1];
        }
        // splay(p, 0);
        return res;
    }

    void DEBUG(int x) {
        if (!x) return;
        DEBUG(ls);
        cerr << v[x] << " ";
        DEBUG(rs);
    }

    void DEBUG() {
        cerr << "Splay: ";
        DEBUG(rt);
        cerr << endl;
    }
} // namespace Splay

} // namespace Backlight
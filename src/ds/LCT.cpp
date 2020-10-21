namespace Backlight {

namespace LinkCutTree {
    using T = int;
    #define ls ch[x][0]
    #define rs ch[x][1]
    const int S = N;

    int tot, sz[S], rev[S], ch[S][2], fa[S];

    T v[S], sum[S];

    inline void init() { tot = 0; }

    inline void clear(int x) { ch[x][0] = ch[x][1] = fa[x] = sz[x] = rev[x] = sum[x] = v[x] = 0; }

    inline int get(int x) { return ch[fa[x]][1] == x; }

    inline int isroot(int x) { return ch[fa[x]][0] != x && ch[fa[x]][1] != x; }

    inline int newnode(T val) {
        ++tot;
        sz[tot] = 1;
        ch[tot][0] = ch[tot][1] = fa[tot] = rev[tot] = 0;
        sum[tot] = v[tot] = val;
        return tot;
    }

    inline void reverse(int x) {
        swap(ls, rs);
        rev[x] ^= 1;
    }

    inline void push_up(int x) {
        sz[x] = sz[ls] + 1 + sz[rs];
        sum[x] = sum[ls] ^ v[x] ^ sum[rs];
    }

    inline void push_down(int x) {
        if (rev[x]) {
            reverse(ls); reverse(rs);
            rev[x] = 0;
        }
    }

    inline void update(int x) {
        if(!isroot(x)) update(fa[x]);
        push_down(x);
    }

    inline void rotate(int x) {
        int f = fa[x], g = fa[f], i = get(x);
        if (!isroot(f)) ch[g][get(f)] = x;
        fa[x] = g;
        ch[f][i] = ch[x][i^1]; fa[ch[f][i]] = f;
        ch[x][i^1] = f; fa[f] = x;
        push_up(f); push_up(x);
    }

    inline void splay(int x) {
        update(x);
        for (; !isroot(x); rotate(x))
            if (!isroot(fa[x])) rotate(get(fa[x]) == get(x) ? fa[x] : x);
    }

    inline void access(int x) {
        for(int y = 0; x; y = x, x = fa[x]) splay(x), rs = y, push_up(x);
    }

    inline void makeroot(int x) {
        access(x);
        splay(x);
        reverse(x);
    }

    inline int findroot(int x) {
        access(x); splay(x);
        while(ls) push_down(x), x = ls;
        return x;
    }

    inline void link(int x, int y) {
        makeroot(x);
        if (findroot(y) != x) fa[x] = y;
    }

    inline void cut(int x, int y) {
        makeroot(x);
        if (findroot(y) == x && fa[x] == y && ch[y][0] == x && !ch[y][1]) {
            fa[x] = ch[y][0] = 0;
            push_up(y);
        }
    }

    inline void split(int x, int y) {
        makeroot(x); 
        access(y);
        splay(y);
    }

    inline int query(int x, int y) {
        split(x, y);
        return sum[y];
    }

} // namespace LinkCutTree
#define LCT LinkCutTree

} // namespace Backlight
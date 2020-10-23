namespace Trie {
    // 1-based
    const int __N = 4e6 + 5;
    const int __M = 26;
    int tot;
    int ch[__N][__M];
    int f[__N], e[__N];

    inline void init() {
        tot = 0;
        memset(ch[0], 0, sizeof(ch[0]));
        f[0] = e[0] = 0;
    }
 
    inline int newnode() {
        ++tot;
        memset(ch[tot], 0, sizeof(ch[tot]));
        f[tot] = e[tot] = 0;
        return tot;
    }
 
    inline int idx(char c) { return c - 'a'; }
 
    void insert(char* s) {
        int n = strlen(s + 1), p = 0, c;
        for (int i = 1; i <= n; ++i) {
            c = idx(s[i]);
            if (!ch[p][c]) ch[p][c] = newnode();
            p = ch[p][c];
            ++f[p];
        }
        ++e[p];
    }
 
    int query(char* s) {
        int p = 0, n = strlen(s + 1), c;
        for(int i = 1; i <= n; i++){
            c = idx(s[i]);
            if(!ch[p][c]) return 0;
            p = ch[p][c];
        }
        return e[p];
    }
}
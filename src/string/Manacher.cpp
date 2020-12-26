namespace Manacher {
    // 1-based

    const int __N = N << 1;

    char s[__N];
    int n, len[__N];

    // @ t1 t2 t3 \0
    // ==> @ # t1 # t2 # t3 # \0
    inline void init(char* t, int m) {
        n = 2 * m + 1;
        s[0] = '@'; s[n] = '#'; s[n + 1] = 0;
        for (int i = 1; i <= m; ++i) {
            s[2 * i - 1] = '#';
            s[2 * i] = t[i];
        }
    }

    // s[i-len[i]...i+len[i]] is palindromic
    // len[i]-1 is palindromic length in t
    void manacher(char* t, int m) {
        init(t, m);
        for (int i = 1, l = 0, r = 0, k; i <= n; ++i) {
            k = i > r ? 1 : min(r - i, len[l + r - i]);
            while(s[i - k] == s[i + k]) ++k;
            len[i] = k--;
            if (i + k > r) {
                l = i -  k;
                r = i + k;
            }
        }
    }

    int getMaxPalindromicLength(char* t, int m) {
        manacher(t, m);
        int ma = 0;
        for (int i = 1; i <= n; ++i) updMax(ma, len[i]);
        return ma - 1;
    }
}
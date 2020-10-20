namespace Backlight {

namespace ACAM {
    const int N = 1e6 + 5;
    const int M = 26;
    int tot, tr[N][M], fail[N];

    int f[N], e[N];

    inline void init() {
        tot = 0;
        memset(tr[0], 0, sizeof(tr[0]));
        f[0] = e[0] = 0;
    }

    inline int newnode() {
        ++tot;
        memset(tr[tot], 0, sizeof(tr[tot]));
        f[tot] = e[tot] = 0;
        return tot;
    }

    void insert(char* s) {
        int n = strlen(s), p = 0, c;
        for (int i = 0 ; i < n; ++i) {
            c = s[i] - 'a';
            if (!tr[p][c]) tr[p][c] = newnode();
            p = tr[p][c];
            ++f[p];
        }
        ++e[p];
    }

    void getfail() {
        queue<int> q;
        for (int i = 0; i < M; ++i) if (tr[0][i]) fail[tr[0][i]] = 0, q.push(tr[0][i]);
        while(!q.empty()) {
            int p = q.front(); q.pop();
            for (int i = 0; i < M; ++i) {
                if (tr[p][i]) fail[tr[p][i]] = tr[fail[p]][i], q.push(tr[p][i]);
                else tr[p][i] = tr[fail[p]][i];
            }
        }
    }

    int query(char* t) {
        int n = strlen(t), p = 0, res = 0;
        for (int i = 0; i < n; ++i) {
            p = tr[p][t[i] - 'a'];
            for (int j = p; j && e[j] != -1; j = fail[j]) res += e[j], e[j] = -1;
        }
        return res;
    }
} // namespace ACAM

} // namespace Backlight
const int N = 1e5 + 5;
vector<int> G[N], R[N];
void init(int n) {
    for (int i = 1; i <= n; ++i) G[i].clear(), R[i].clear();
}
inline void addarc(int u, int v) {
    G[u].push_back(v);
    R[v].push_back(u);
}

int n, m;
int dfs_clock, scc_cnt;
int dfn[N], belong[N];
bool vis[N];
void dfs1(int u) {
    vis[u] = true;
    for (const int& v: G[u]) {
        if (!vis[v]) dfs1(v);
    }
    dfn[++dfs_clock] = u;
}
void dfs2(int u) {
    belong[u] = scc_cnt;
    for (const int& v: R[u]) {
        if (!belong[v]) dfs2(v);
    }
}
void kosaraju() {
    dfs_clock = scc_cnt = 0;
    fill(dfn + 1, dfn + 1 + n, 0);
    fill(belong + 1, belong + 1 + n, 0);
    fill(vis + 1, vis + 1 + n, false);
    for (int i = 1; i <= n; ++i) {
        if (!vis[i]) dfs1(i);
    }

    for (int i = n; i >= 1; --i) {
        if (!belong[dfn[i]]) {
            ++scc_cnt;
            dfs2(dfn[i]);
        }
    }
}
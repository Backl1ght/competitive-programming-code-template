// CF600E
// 对于每个节点，输出其子树中出现次数最多的颜色之和。
vector<int> G[N];
inline void addedge(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
}

int n, color[N];

int sz[N], son[N], cnt[N], ma;
ll cur, ans[N];
void dfs1(int u, int fa) {
    sz[u] = 1; son[u] = -1;
    for (int v: G[u]) {
        if (v == fa) continue;
        dfs1(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[son[u]]) son[u] = v;
    }
}

void add(int u, int fa, int Son, int d) {
    cnt[color[u]] += d;
    if (cnt[color[u]] > ma) ma = cnt[color[u]], cur = 0;
    if (cnt[color[u]] == ma) cur += color[u];

    for (int v: G[u]) {
        if (v == fa || v == Son) continue;
        add(v, u, Son, d);
    }
}

void dfs2(int u, int fa, bool keep) {
    for (int v: G[u]) {
        if (v == fa || v == son[u]) continue;
        dfs2(v, u, false);
    }
    if (son[u] != -1) dfs2(son[u], u, true);

    add(u, fa, son[u], 1);

    // answer queries here
    ans[u] = cur;

    if (!keep) {
        add(u, fa, -1, -1);
        ma = 0; cur = 0;
    }
}

void solve() {
    read(n);
    FOR(i, 1, n) read(color[i]);

    int u, v;
    FOR(i, 2, n) {
        read(u, v);
        addedge(u, v);
    }

    dfs1(1, 0);
    dfs2(1, 0, 0);

    FOR(i, 1, n - 1) printf("%lld ", ans[i]);
    println(ans[n]);
}
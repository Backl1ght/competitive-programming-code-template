int tot, head[N];
struct Edge {
    int v, nxt;
}e[M];

void addedge(int u, int v) {
    ++tot; e[tot] = (Edge){v, head[u]}; head[u] = tot;
    ++tot; e[tot] = (Edge){u, head[v]}; head[v] = tot;
}

int h[N], f[N], sz[N], son[N], top[N];
void dfs1(int u, int fa) {
    h[u] = h[fa] + 1; f[u] = fa;
    sz[u] = 1; son[u] = 0;
    for (int i = head[u]; i; i = e[i].nxt) {
        int v = e[i].v;
        if (v == fa) continue;
        dfs1(v, u);
        sz[u] += sz[v];
        if(sz[v] > sz[son[u]]) son[u] = v;
    }
}

void dfs2(int u, int fa, int tp) {
    top[u] = tp;
    if(son[u]) dfs2(son[u], u, tp);
    for (int i = head[u]; i; i = e[i].nxt) {
        int v = e[i].v;
        if (v == fa || v == son[u]) continue;
        dfs2(v, u, v);
    }
}

int LCA(int u, int v) {
    while(top[u] != top[v]) {
        if (h[top[u]] < h[top[v]]) swap(u, v);
        u = f[top[u]];
    }
    if (h[u] > h[v]) swap(u, v);
    return u;
}
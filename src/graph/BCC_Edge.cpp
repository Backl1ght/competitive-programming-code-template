namespace Backlight {

struct Graph {
    #define fore(i, u) for (int i = h[u]; i; i = e[i].nxt)
    struct Edge {
        int v, nxt;
        Edge(){}
        Edge(int _v, int _nxt): v(_v), nxt(_nxt) {}
    };

    int V, E, tot;
    vector<int> h;
    vector<Edge> e;

    Graph() : V(0) {}
    Graph(int _V, int _E) : V(_V), E(2 * _E), tot(0), h(_V + 1), e(2 * _E + 1) { }

    inline void addarc(int u, int v) {
        assert(1 <= u && u <= V);
        assert(1 <= v && v <= V);

        e[++tot] = Edge(v, h[u]); h[u] = tot;
    }

    inline void addedge(int u, int v) {
        addarc(u, v);
        addarc(v, u);
    }

    /**************************************************/
    int bcc_clock, bcc_cnt;
    vector<int> dfn, low, belong, bcc_size;
    vector<vector<int>> bcc;
    vector<bool> bridge;

    void tarjan(int u, int fa) {
        dfn[u] = low[u] = ++bcc_clock;
        fore(i, u) {
            int v = e[i].v;
            if (v == fa) continue;

            if (!dfn[v]) {
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                if (dfn[u] < low[v]) {
                    bridge[i] = true;
                    if (i & 1) bridge[i + 1] = true;
                    else bridge[i - 1] = true;
                }
            } else if (dfn[v] < dfn[u]) {
                low[u] = min(low[u], dfn[v]);
            } 
        }
    }

    void blood_fill(int u) {
        belong[u] = bcc_cnt; bcc[bcc_cnt].push_back(u);
        fore(i, u) {
            if (bridge[i]) continue;
            int v = e[i].v;
            if (!belong[v]) blood_fill(v);
        }
    }

    void build_bcc_point() {
        bcc_clock = bcc_cnt = 0;
        dfn = vector<int>(V + 1);
        low = vector<int>(V + 1);
        belong = vector<int>(V + 1);
        bridge = vector<bool>(E + 1);
        bcc = vector<vector<int>>(1);

        for (int i = 1; i <= V; ++i) {
            if (!dfn[i]) {
                tarjan(i, i);
            }
        }

        for (int i = 1; i <= V; ++i) {
            if (!belong[i]) {
                ++bcc_cnt;
                bcc.push_back(vector<int>());
                blood_fill(i);
            }
        }

        bcc_size = vector<int> (bcc_cnt + 1);
        for (int i = 1; i <= bcc_cnt; ++i) bcc_size[i] = bcc[i].size();
    }
};

}
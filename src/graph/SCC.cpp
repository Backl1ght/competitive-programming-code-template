namespace Backlight {

#define SCC
#define GRAPH
struct Graph {
    struct Edge {
        int u, v;
        Edge(){}
        Edge(int _u, int _v): u(_u), v(_v) {}
    };

    int V;
    vector<vector<Edge>> G;

    Graph() : V(0) {}
    Graph(int _V) : V(_V), G(_V + 1) {}

    inline void addarc(int u, int v) {
        assert(1 <= u && u <= V);
        assert(1 <= v && v <= V);
        G[u].push_back(Edge(u, v));
    }

    inline void addedge(int u, int v) {
        addarc(u, v);
        addarc(v, u);
    }

    /**************************************************/
    int scc_clock, scc_cnt;
    vector<int> dfn, low, belong, scc_size;
    vector<bool> ins;
    stack<int> stk;

    void tarjan(int u, int fa) {
        dfn[u] = low[u] = ++scc_clock;
        ins[u] = true;
        stk.push(u);

        // bool flag = false;
        for (Edge& e: G[u]) {
            int v = e.v;
            // if (v == fa && !flag) {
            //     k = true;
            //     continue;
            // }

            if (!dfn[v]) {
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
            } else if (ins[v]) low[u] = min(low[u], dfn[v]);
        }

        if (dfn[u] == low[u]) {
            ++scc_cnt; scc_size.push_back(0);
            int top;
            do {
                top = stk.top(); stk.pop();
                ins[top] = false;
                belong[top] = scc_cnt;
                ++scc_size[scc_cnt];
            } while(u != top);
        }
    }

    void build_scc() {
        scc_clock = scc_cnt = 0;
        dfn = vector<int>(V + 1);
        low = vector<int>(V + 1);
        belong = vector<int>(V + 1);
        ins = vector<bool>(V + 1);
        scc_size = vector<int>(1);

        for (int i = 1; i <= V; ++i) {
            if (!dfn[i]) tarjan(i, i);
        }
    }
};

}
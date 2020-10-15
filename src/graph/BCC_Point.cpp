namespace Backlight {

#define BCC_POINT
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
    int bcc_clock;
    vector<int> dfn, low;
    vector<vector<int>> bcc;
    vector<bool> cut;
    stack<int> stk;

    void tarjan(int u, int fa) {
        dfn[u] = low[u] = ++bcc_clock; stk.push(u);

        if (u == fa && G[u].empty()) {
            vector<int> nb; 
            nb.push_back(u);
            bcc.push_back(nb);
            return;
        }

        int son = 0;
        for (Edge& e: G[u]) {
            int v = e.v;
            if (v == fa) continue;

            if (!dfn[v]) {
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    ++son;
                    if (u != fa || son > 1) cut[u] = true;
                    vector<int> nb;
                    int top;
                    do {
                        top = stk.top(); stk.pop();
                        nb.push_back(top);
                    } while(top != v);
                    nb.push_back(u);
                    bcc.push_back(nb);
                }
            } else low[u] = min(low[u], dfn[v]);
        }
    }

    void build_bcc_point() {
        bcc_clock = 0;
        dfn = vector<int>(V + 1);
        low = vector<int>(V + 1);
        cut = vector<bool>(V + 1);
        bcc = vector<vector<int>>(1);

        for (int i = 1; i <= V; ++i) {
            if (!dfn[i]) {
                while(!stk.empty()) stk.pop();
                tarjan(i, i);
            }
        }
    }
};

}
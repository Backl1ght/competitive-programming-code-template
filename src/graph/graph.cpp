namespace Backlight {

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
};

}
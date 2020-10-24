namespace Backlight {

template<typename T>
struct Wraph {
    struct Edge {
        int u, v;
        T w;
        Edge(){}
        Edge(int _u, int _v, T _w): u(_u), v(_v), w(_w) {}
    };

    int V;
    vector<vector<Edge>> G;

    Wraph() : V(0) {}
    Wraph(int _V) : V(_V), G(_V + 1) {}

    inline void addarc(int u, int v, T w = 1) {
        assert(1 <= u && u <= V);
        assert(1 <= v && v <= V);
        G[u].push_back(Edge(u, v, w));
    }

    inline void addedge(int u, int v, T w = 1) {
        addarc(u, v, w);
        addarc(v, u, w);
    }
};

}
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

    inline void addarc(int u, int v, T w) {
        assert(1 <= u && u <= V);
        assert(1 <= v && v <= V);
        G[u].push_back(Edge(u, v, w));
    }

    inline void addedge(int u, int v, T w) {
        addarc(u, v, w);
        addarc(v, u, w);
    }

    /**************************************************/
    vector<T> dijkstra(int S, T T_MAX) {
        typedef pair<T, int> Node;
        priority_queue<Node, vector<Node>, greater<Node>> q;
        vector<T> dis(V + 1);
        for (int i = 1; i <= V; i++) dis[i] = T_MAX;
        dis[S] = 0; q.push(Node(0, S));
        while (!q.empty()){
            Node p = q.top(); q.pop();
            T cost = p.first; int u = p.second;
            if (dis[u] != cost) continue;

            for (Edge e: G[u]){
                int v = e.v;
                T w = e.w;
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    q.push(Node(dis[v], v));
                }
            }
        }
        return dis;
    }
};

}
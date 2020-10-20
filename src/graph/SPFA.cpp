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
    vector<T> spfa(int S, T T_MAX) {
        queue<int> q;
        vector<T> dis(V + 1, T_MAX);
        vector<bool> inq(V + 1, 0);
        q.push(S); dis[S] = 0;
        while(!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = 0;
            for(Edge e: G[u]) {
                if(dis[e.v] > dis[u] + e.w) {
                    dis[e.v] = dis[u] + e.w;
                    if(!inq[e.v]) {
                        inq[e.v] = 1;
                        q.push(e.v);  
                    }   
                }
            }
        }
        return dis;
    }
};

}
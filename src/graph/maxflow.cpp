namespace Backlight {

    template<typename T>
    struct mf_graph {

        struct Edge {
            int v, nxt;
            T c, f;
            Edge(){}
            Edge(int _v, int _nxt, T _c): v(_v), nxt(_nxt), c(_c), f(0) {}
        };

        int V, E;
        vector<int> h;
        vector<Edge> e;

        mf_graph() : V(0) {}
        mf_graph(int _V) : V(_V), h(_V + 1, -1) { }

        inline void addarc(int u, int v, T c) {
            assert(1 <= u && u <= V);
            assert(1 <= v && v <= V);
            assert(0 <= c);

            e.push_back(Edge(v, h[u], c)); h[u] = e.size() - 1;
        }

        inline void addedge(int u, int v, T c) {
            addarc(u, v, c);
            addarc(v, u, 0);
        }

        T maxflow(int s, int t) {
            assert(1 <= s && s <= V);
            assert(1 <= t && t <= V);
            assert(s != t);

            vector<int> f(V + 1), d(V + 1), st(V + 1);

            auto bfs = [&] () {
                fill(d.begin(), d.end(), -1);
                queue<int> q;
                q.push(s); d[s] = 0;
                while(!q.empty()){
                    int u = q.front(); q.pop();
                    for(int i = h[u]; i != -1; i = e[i].nxt) {
                        int v = e[i].v;
                        if(e[i].c > e[i].f && d[v] == -1) {
                            d[v] = d[u] + 1;
                            if (v == t) break;
                            q.push(v);
                        }
                    }
                }
                return (d[t] != -1);
            };

            auto dfs = [&] (auto self, int u, T up) {
                if(u == t || up == 0) return up;
                T res = 0;
                for(int& i = f[u]; i != -1; i = e[i].nxt) {
                    int v = e[i].v;
                    if(d[u] + 1 == d[v]) {
                        T nf = self(self, v, min(up, e[i].c - e[i].f));
                        if (nf <= 0) continue;
                        up -= nf;
                        res += nf;
                        e[i].f += nf;
                        e[i ^ 1].f -= nf;
                        if(up == 0) break;
                    }
                }
                if(res == 0) d[u] = -1;
                return res;
            };
            
            T res = 0;
            while(bfs()) {
                f = h;
                res += dfs(dfs, s, numeric_limits<T>::max());
            }
            return res;
        }
    };

}  // namespace Backlight
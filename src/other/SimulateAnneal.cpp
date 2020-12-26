struct SimulateAnneal {
    constexpr static double p = 0.996;
    inline double Rand() { return 1.0 * rand() / RAND_MAX; }

    int n;
    vector<int> X, Y, W;
    double ax, ay;

    SimulateAnneal(int _n) : n(_n), X(n), Y(n), W(n) {}

    void input() {
        for (int i = 0; i < n; ++i) {
            read(X[i], Y[i], W[i]);
        }
    }

    double cost(double x, double y) {
        double res = 0;
        for (int i = 0; i < n; ++i) {
            double dx = X[i] - x;
            double dy = Y[i] - y;
            double d = sqrt(dx * dx + dy * dy);
            res += d * W[i];
        }
        return res;
    }

    void init() {
        ax = 0; ay = 0;
        for (int i = 0; i < n; ++i) ax += X[i], ay += Y[i];
        ax /= n; ay /= n;
    }

    void simulate_anneal() {
        srand(time(0));
        double T = 1e6, TE = 1e-8;
        double cx = ax, cy = ay, cc = cost(cx, cy);
        while(T > TE) {
            double nx = ax + (2 * Rand() - 1) * T;
            double ny = ay + (2 * Rand() - 1) * T;
        
            double nc = cost(nx, ny);
            double d = nc - cc;

            if (d < 0) cc = nc, ax = cx = nx, ay = cy = ny;
            else if (exp(-d / T) > Rand()) {
                cx = nx;
                cy = ny;
            }

            T *= p;
        }
    }

    void work() {
        init();
        // try a try, AC is ok.
        simulate_anneal();
        simulate_anneal();
        simulate_anneal();
        simulate_anneal();
    }
};
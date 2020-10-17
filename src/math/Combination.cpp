struct Combination {
    int N;
    vector<Mint> f, g;

    Combination() : N(0) {}
    Combination(int _n) : N(_n), f(N + 1), g(N + 1) {
        f[0] = 1;
        for (int i = 1; i <= N; ++i) f[i] = f[i - 1] * i;
        g[N] = f[N].inv();
        for (int i = N - 1; i >= 0; --i) g[i] = g[i + 1] * (i + 1);
    }

    Mint get(int n, int m) {
        if (n < 0 || m < 0 || n < m) return 0;
        return f[n] * g[m] * g[n - m];
    }
} C(N);
namespace FFT {
    const long double PI = acos(-1.0);
    using LL = int64_t;
    struct Complex {
        long double r, i;
        Complex() : r(0), i(0) {}
        Complex(long double _r, long double _i) : r(_r), i(_i) {}
        Complex conj() { return Complex(r, -i); }
        inline Complex operator-(const Complex &c) const { return Complex(r - c.r, i - c.i); }
        inline Complex operator+(const Complex &c) const { return Complex(r + c.r, i + c.i); }
        inline Complex operator*(const Complex &c) const { return Complex(r * c.r - i * c.i, r * c.i + i * c.r); }
    };
    ostream& operator << (ostream& os, Complex& c)  { return os << "(" << c.r << ", " << c.i << ")"; }

    int N;
    vector<int> r;
    void init(int n) {
        N = 1; while(N <= n) N <<= 1;
        r.resize(N);
        for(int i = 1; i < N; ++i) r[i] = (r[i >> 1] >> 1) + ((i & 1) ? (N >> 1) : 0);
    }

    void FFT(vector<Complex>& a, int op) {
        for (int i = 1; i < N; ++i) if (i < r[i]) swap(a[i], a[r[i]]);
        for(int i = 2; i <= N; i <<= 1){
            int l = i >> 1;
            Complex w, x, wk(cos(PI / l), op * sin(PI / l));
            for(int j = 0; j < N; j += i) {
                w = Complex(1, 0);
                for(int k = j; k < j + l; ++k) {
                    x = a[k + l] * w;
                    a[k + l] = a[k] - x;
                    a[k] = a[k] + x;
                    w = w * wk;
                }
            }
        }
        if(op == -1) 
            for(int i = 0 ; i < N; i++) a[i].r /= N, a[i].i /= N;
    }

    inline void FFT(vector<Complex>& a) { FFT(a, 1); }
    inline void IFT(vector<Complex>& a) { FFT(a, -1); }

    vector<int> convolution(const vector<int>& f, const vector<int>& g) {
        int n = f.size(), m = g.size(), k = n + m - 1;
        init(k);
        vector<Complex> a(N), b(N);
        for (int i = 0; i < n; ++i) a[i] = Complex(f[i], 0);
        for (int i = 0; i < m; ++i) b[i] = Complex(g[i], 0);

        FFT(a); FFT(b);
        for (int i = 0; i < N; ++i) a[i] = a[i] * b[i];
        IFT(a);

        vector<int> h(k);
        for (int i = 0; i < k; ++i) h[i] = int(a[i].r + 0.5);
        return h;
    }

    // 任意模数FFT
    vector<int> convolutionM(const vector<int>& f, const vector<int>& g, int p) {
        int n = f.size(), m = g.size(), k = n + m - 1;
        init(k);
        vector<Complex> a(N), b(N), c(N), d(N);
        for (int i = 0; i < n; ++i) a[i] = Complex(f[i] >> 15, f[i] & 32767);
        for (int i = 0; i < m; ++i) c[i] = Complex(g[i] >> 15, g[i] & 32767);
        FFT(a); FFT(c);
        for (int i = 1; i < N; ++i) b[i] = a[N - i].conj();
        for (int i = 1; i < N; ++i) d[i] = c[N - i].conj();
        b[0] = a[0].conj(); d[0] = c[0].conj();
        for (int i = 0; i < N; ++i) {
            Complex aa, bb, cc, dd;
            aa = (a[i] + b[i]) * Complex(0.5, 0);
            bb = (a[i] - b[i]) * Complex(0, -0.5);
            cc = (c[i] + d[i]) * Complex(0.5, 0);
            dd = (c[i] - d[i]) * Complex(0, -0.5);
            a[i] = aa * cc + Complex(0, 1) * (aa * dd + bb * cc);
            b[i] = bb * dd;
        }
        IFT(a); IFT(b);
        vector<int> h(k);
        for (int i = 0; i < k; ++i) {
            int aa, bb, cc;
            aa = LL(a[i].r + 0.5) % p;
            bb = LL(a[i].i + 0.5) % p;
            cc = LL(b[i].r + 0.5) % p;
            h[i] = ((1ll * aa * (1 << 30) % p + 1ll * bb * (1 << 15) % p + cc) % p + p) % p;
        }
        return h;
    }
} // namespace FFT
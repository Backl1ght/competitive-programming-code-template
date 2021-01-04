namespace Poly {
    const int N = ...;
    const int MAXN = N << 3;
    const int P = 998244353;
    const int G = 3;

    ll qp(ll a, ll b) {
        ll res = 1; a %= P;
        while(b) {
            if (b & 1) res = res * a % P;
            a = a * a % P;
            b >>= 1;
        }
        return res;
    }
    
    const int Gi = qp(G, P - 2);
    const int I2 = qp(2, P - 2);
    int r[MAXN];
    ll t1[MAXN], t2[MAXN], t3[MAXN], t4[MAXN], t5[MAXN], t6[MAXN], t7[MAXN];

    // int N, l;
    // void init(int n) {
    //     int N = 1, l = -1; while(N <= n << 1) N <<= 1, l++;
    //     for(int i = 1; i < N; ++i) r[i] = (r[i >> 1] >> 1) | ((i & 1) << l);
    // }

    void inplaceNTT(ll *a, int n, int op) {
        for(int i = 0; i < n; ++i) if(i < r[i]) swap(a[i],a[r[i]]);
        for(int m2 = 2, m = 1; m2 <= n; m = m2, m2 <<= 1) {
            ll wn = qp(op == 1 ? G : Gi, (P - 1) / m2), x, y;
            for(int l = 0; l < n; l += m2) {
                ll w = 1;
                for(int i = l; i < l + m; ++i) {
                    x = a[i], y = w * a[i + m] % P;
                    a[i] = (x + y) % P;
                    a[i + m] = (x + P - y) % P;
                    w = w * wn % P;
                }
            }
        }
        if (op == -1) {
            ll inv = qp(n, P - 2);
            for(int i = 0; i < n; ++i) a[i] = a[i] * inv % P;
        }
    }
    inline void NTT(ll *a, int n) { inplaceNTT(a, n, 1); }
    inline void INTT(ll *a, int n) { inplaceNTT(a, n, -1); }

    // 多项式微分(求导)
    inline void Derivative(ll *a, ll *b, int n) {
        for(int i = 0; i < n; ++i) b[i] = a[i + 1] * (i + 1) % P;
        b[n - 1] = 0;
    }

    // 多项式积分 
    inline void Integral(ll *a, ll *b, int n) {
        for(int i = 0; i < n; ++i) b[i + 1] = a[i] * qp(i + 1, P - 2) % P;
        b[0] = 0;
    }

    // 多项式翻转
    // b(x) = x^{n} a(\frac{1}{x}) 
    inline void Reverse(ll *a, ll *b, int n) {
        for (int i = 0; i < n; ++i) b[i] = a[n - i - 1];
    }

    // 多项式乘法逆
    // b(x) = a^{-1}(x) mod x^n
    void __Inverse(ll *a, ll *b, int n) {
        if(n == 1) {
            b[0] = qp(a[0], P - 2);
            return;
        }

        __Inverse(a, b, (n + 1) >> 1);

        int N = 1, l = -1; while(N <= n << 1) N <<= 1, l++;
        for(int i = 1; i < N; ++i) r[i] = (r[i >> 1] >> 1) | ((i & 1) << l);

        memcpy(t1, a, sizeof(a[0]) * n); fill(t1 + n, t1 + N, 0);

        NTT(t1, N); NTT(b, N);
        for(int i = 0; i < N; ++i) b[i] = ((b[i] << 1) % P + P - t1[i] * b[i] % P * b[i] % P) % P;
        INTT(b, N);

        fill(b + n, b + N, 0);
    }

    inline void Inverse(ll *a, ll *b, int n) {
        fill(b, b + (n << 2), 0);
        __Inverse(a, b, n);
    }

    // 多项式对数函数
    // b(x) = ln a(x) mod x^n
    void Ln(ll *a, ll *b, int n) {
        #define aD t3
        #define aI t4

        Derivative(a, aD, n); Inverse(a, aI, n);
        int N = 1, l = -1; while(N <= n << 1) N <<= 1, l++;
        for(int i = 1; i < N; ++i) r[i] = (r[i >> 1] >> 1) | ((i & 1) << l);
        NTT(aD, N); NTT(aI, N);
        for(int i = 0; i < N; ++i) aD[i] = aD[i] * aI[i] % P;
        INTT(aD, N); Integral(aD, b, n);

        #undef aD
        #undef aI
    }

    // 多项式指数函数
    // b(x) = exp a(x) mod x^n
    void Exp(ll *a, ll *b, int n) {
        #define Lnb t2

        if(n == 1) {
            b[0] = 1;
            return;
        }
        Exp(a, b, (n + 1) >> 1);
        Ln(b, Lnb, n);
        int N = 1, l = -1; while(N <= n << 1) N <<= 1, l++;
        for(int i = 1; i < N; ++i) r[i] = (r[i >> 1] >> 1) | ((i & 1) << l);

        memcpy(t1, a, sizeof(a[0]) * n); fill(t1 + n, t1 + N, 0);
        fill(Lnb + n, Lnb + N, 0);
        
        for(int i = 0; i < N; ++i) t1[i] = ((t1[i] - Lnb[i]) % P + P) % P;
        ++t1[0];
        NTT(b, N); NTT(t1, N);
        for(int i = 0; i < N; ++i) b[i] = b[i] * t1[i] % P;
        INTT(b, N);

        fill(b + n, b + N, 0);
        #undef Lnb
    }

    // 多项式乘法(卷积)
    // c(x) = a(x) * b(x) mod x^(n + m)
    // deg c = n + m - 1
    void Convolution(ll *a, int n, ll *b, int m, ll *c) {
        int N = 1, l = -1; while(N <= (n + m) << 1) N <<= 1, l++;
        for(int i = 1; i < N; ++i) r[i] = (r[i >> 1] >> 1) | ((i & 1) << l);

        memcpy(t1, a, sizeof(a[0]) * n); fill(t1 + n, t1 + N, 0);
        memcpy(t2, b, sizeof(b[0]) * m); fill(t2 + m, t2 + N, 0);

        NTT(t1, N); NTT(t2, N);
        for (int i = 0; i < N; ++i) c[i] = t1[i] * t2[i] % P;
        INTT(c, N);
        fill(c + n + m, c + N, 0);
    }
    #define Multiply Convolution

    // 多项式除法
    // a(x) = b(x)Q(x) + R(x)
    // deg Q = n - m + 1
    // deg R = m - 1
    void Divide(ll *a, int n, ll *b, int m, ll *Q, ll *R) {
        #define aR t3
        #define bR t4
        #define bRi t5
        #define QR t6
        #define bQ t7

        int degQ = n - m + 1;
        int degR = m - 1;

        Reverse(a, aR, n); Reverse(b, bR, m);
        for (int i = degQ; i < m; ++i) bR[i] = 0;

        // get Q(x)
        Inverse(bR, bRi, degQ);
        Multiply(aR, n, bRi, degQ, QR);
        Reverse(QR, Q, degQ);
        
        // get R(x)
        Multiply(b, m, Q, degQ, bQ);
        for (int i = 0; i < degR; ++i) R[i] = (a[i] - bQ[i] + P) % P;

        #undef aR
        #undef bR
        #undef bRi
        #undef QR
    }

    // 多项式求平方根
    // b^{2}(x) = a(x)
    #define bI t3
    void __Sqrt(ll *a, ll* b, int n) {
        if (n == 1) {
            b[0] = 1;
            return;
        }

        __Sqrt(a, b, (n + 1) >> 1);

        Inverse(b, bI, n);
        Multiply(a, n, bI, n, bI);
        for (int i = 0; i < n; ++i) b[i] = (b[i] + bI[i]) * I2 % P;
    }
    inline void Sqrt(ll *a, ll *b, int n) {
        fill(bI, bI + (n << 2), 0);
        __Sqrt(a, b, n);
    }
    #undef bI

    struct poly {
        vector<ll> a;
        int size() const { return a.size(); }
        int deg() const { return size() - 1; }
        ll& operator [] (int i) { assert(i < size()); return a[i]; }
        ll operator [] (int i) const { return i < size() ? a[i] : 0LL; }
        void reverse() { std::reverse(a.begin(), a.end()); }
        void resize(int n) { a.resize(n); }
        poly(int n = 0) : a(n, 0) {}

        void DEBUG() {
            cerr << "Poly DEBUG: " << endl;
            for (const ll& v: a) cerr << v << " ";
            cerr << endl;
        }

        void DEBUG() const {
            cerr << "Poly DEBUG: " << endl;
            for (const ll& v: a) cerr << v << " ";
            cerr << endl;
        }


        void input() {
            for (ll& x: a) read(x);
        }

        void output() {
            if (a.empty()) { puts(""); return; }
            int n = a.size();
            for (int i = 0; i < n - 1; ++i) printf("%lld ", a[i]);
            printf("%lld\n", a[n - 1]);
        }

        void output() const  {
            if (a.empty()) { puts(""); return; }
            int n = a.size();
            for (int i = 0; i < n - 1; ++i) printf("%lld ", a[i]);
            printf("%lld\n", a[n - 1]);
        }

        poly inv(int n = -1) const {
            if (n == -1) n = size();
            static ll f[MAXN], g[MAXN];
            for (int i = 0; i < n; ++i) f[i] = a[i];
            Inverse(f, g, n);
            poly res(n);
            for (int i = 0; i < n; ++i) res[i] = g[i];
            return res;
        }

        poly rev() const {
            int n = size();
            poly r(n);
            for (int i = 0; i < n; ++i) r[i] = a[n - i - 1];
            return r;
        }

        poly sqrt() {
            int n = a.size();
            static ll f[MAXN], g[MAXN];
            for (int i = 0; i < n; ++i) f[i] = a[i];
            Sqrt(f, g, n);
            poly res(n);
            for (int i = 0; i < n; ++i) res[i] = g[i];
            return res;
        }
    };

    poly operator + (const poly& a, const poly& b) {
        int k = max(a.size(), b.size());
        poly c(k);
        for (int i = 0; i < k; ++i) c[i] = (a[i] + b[i]) % P;
        return c;
    }

    poly operator - (const poly& a, const poly& b) {
        int k = max(a.size(), b.size());
        poly c(k);
        for (int i = 0; i < k; ++i) c[i] = (a[i] - b[i] + P) % P;
        return c;
    }

    poly operator * (const poly& a, const poly& b) {
        static ll ta[MAXN], tb[MAXN];
        int n = a.size(), m = b.size(), k = n + m - 1;
        for (int i = 0; i < n; ++i) ta[i] = a[i];
        for (int i = 0; i < m; ++i) tb[i] = b[i];

        Multiply(ta, n, tb, m, ta);

        poly c(k);
        for (int i = 0; i < k; ++i) c[i] = ta[i];
        return c;
    }

    pair<poly, poly> Divide(const poly& a, const poly& b) {
        static ll ta[MAXN], tb[MAXN], tq[MAXN], tr[MAXN];
        int n = a.size(), m = b.size();
        if (n < m) return make_pair(poly(0), a);

        int degQ = n - m + 1, degR = m - 1;
        for (int i = 0; i < n; ++i) ta[i] = a[i];
        for (int i = 0; i < m; ++i) tb[i] = b[i];

        Divide(ta, n, tb, m, tq, tr);

        poly q(degQ); for (int i = 0; i < degQ; ++i) q[i] = tq[i];
        poly r(degR); for (int i = 0; i < degR; ++i) r[i] = tr[i];

        return make_pair(q, r);
    }

    poly operator / (const poly &a, const poly &b) { return Divide(a, b).first; }
    poly operator % (const poly &a, const poly &b) { return Divide(a, b).second; }



    // given a(x), deg a = n
    // calc y_i = a(x_i) for i in [0, m), O(n \log^2 n)
    poly t[N << 2], p[N];
    void build(int o, int l, int r) {
        if (l == r) {
            t[o] = p[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(o << 1, l, mid); 
        build(o << 1 | 1, mid + 1, r);
        t[o] = t[o << 1] * t[o << 1 | 1];
    }
    void __calcValue(int o, int l, int r, const poly& f, ll *x, ll *y) {
        // if (l == r) {
        //     y[l] = f[0];
        //     return;
        // }
        if (r - l <= 75) { // 降低常数(魔法)
            for (int i = l; i <= r; ++i) {
                ll v = 0;
                for (int j = f.size() - 1; j >= 0; --j)
                    v = (v * x[i] % P + f[j]) % P;
                y[i] = v;
            }
            return;
        }

        int mid = (l + r) >> 1, lc = o << 1, rc = o << 1 | 1;
        __calcValue(lc, l, mid, f % t[lc], x, y);
        __calcValue(rc, mid + 1, r, f % t[rc], x, y);
    }
    void calcValue(const poly& f, ll *x, ll*y, int m) {
        for (int i = 1; i <= m; ++i) {
            p[i].resize(2);
            p[i][0] = P - x[i];
            p[i][1] = 1;
        }
        build(1, 1, m);
        __calcValue(1, 1, m, f % t[1], x, y);   
    }
}
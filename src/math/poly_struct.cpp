// Referece: jiangly
constexpr int P = 998244353;
vector<int> rev, roots{0, 1};
int power(int a, int b) {
    int r = 1;
    while(b) {
        if (b & 1)
            r = 1ll * r * a % P;
        a = 1ll * a * a % P;
        b >>= 1;
    }
    return r;
}
void dft(vector<int> &a) {
    int n = a.size();
    if (int(rev.size()) != n) {
        int k = __builtin_ctz(n) - 1;
        rev.resize(n);
        for (int i = 0; i < n; ++i)
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
    }
    for (int i = 0; i < n; ++i)
        if (rev[i] < i)
            swap(a[i], a[rev[i]]);
    if (int(roots.size()) < n) {
        int k = __builtin_ctz(roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            int e = power(3, (P - 1) >> (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); ++i) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = 1ll * roots[i] * e % P;
            }
            ++k;
        }
    }
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int u = a[i + j];
                int v = 1ll * a[i + j + k] * roots[k + j] % P;
                int x = u + v;
                if (x >= P)
                    x -= P;
                a[i + j] = x;
                x = u - v;
                if (x < 0)
                    x += P;
                a[i + j + k] = x;
            }
        }
    }
}
void idft(vector<int> &a) {
    int n = a.size();
    reverse(a.begin() + 1, a.end());
    dft(a);
    int inv = power(n, P - 2);
    for (int i = 0; i < n; ++i)
        a[i] = 1ll * a[i] * inv % P;
}
struct poly {
    vector<int> a;

    poly() {}
    poly(int f0) { a = {f0}; }
    poly(const vector<int> &f) : a(f) {
        while (!a.empty() && !a.back())
            a.pop_back();
    }
    poly(const vector<int> &f, int n) : a(f) {
        a.resize(n);
    }
    int size() const {
        return a.size();
    }
    int deg() const {
        return a.size() - 1;
    }
    int operator[](int idx) const {
        if (idx < 0 || idx >= size())
            return 0;
        return a[idx];
    }
    void input(int n) {
        a.resize(n);
        FE(v, a) rd(v);
    }
    void output(int n) {
        for (int i = 0; i < n - 1; ++i) printf("%d ", (*this)[i]);
        printf("%d\n", (*this)[n - 1]);
    }
    poly mulxk(int k) const {
        auto b = a;
        b.insert(b.begin(), k, 0);
        return poly(b);
    }
    poly modxk(int k) const {
        k = min(k, size());
        return poly(std::vector<int>(a.begin(), a.begin() + k));
    }
    poly alignxk(int k) const {
        return poly(a, k);
    }
    poly divxk(int k) const {
        if (size() <= k)
            return poly();
        return poly(vector<int>(a.begin() + k, a.end()));
    }
    friend poly operator+(const poly& f, const poly& g) {
        int k = max(f.size(), g.size());
        vector<int> res(k);
        for (int i = 0; i < k; ++i) {
            res[i] = f[i] + g[i];
            if (res[i] >= P)
                res[i] -= P;
        }
        return poly(res);
    }
    friend poly operator - (const poly& f, const poly &g) {
        int k = max(f.size(), g.size());
        vector<int> res(k);
        for (int i = 0; i < k; ++i) {
            res[i] = f[i] - g[i];
            if (res[i] < 0)
                res[i] += P;
        }
        return poly(res);
    }
    friend poly operator * (const poly& f, const poly& g) {
        int sz = 1, k = f.size() + g.size() - 1;
        while (sz < k) sz *= 2;
        vector<int> p = f.a, q = g.a;
        p.resize(sz); q.resize(sz);
        dft(p); dft(q);
        for (int i = 0; i < sz; ++i)
            p[i] = 1ll * p[i] * q[i] % P;
        idft(p);
        return poly(p);
    }
    friend poly operator / (const poly& f, const poly& g) {
        return f.divide(g).first;
    }
    friend poly operator % (const poly& f, const poly& g) {
        return f.divide(g).second;
    }
    poly &operator += (const poly& f) {
        return (*this) = (*this) + f;
    }
    poly &operator -= (const poly& f) {
        return (*this) = (*this) - f;
    }
    poly &operator *= (const poly& f) {
        return (*this) = (*this) * f;
    }
    poly &operator /= (const poly& f) {
        return (*this) = divide(f).first;
    }
    poly &operator %= (const poly& f) {
        return (*this) = divide(f).second;
    }
    poly derivative() const {
        if (a.empty()) return poly();
        int n = a.size();
        vector<int> res(n - 1);
        for (int i = 0; i < n - 1; ++i)
            res[i] = 1ll * (i + 1) * a[i + 1] % P;
        return poly(res);
    }
    poly integral() const {
        if (a.empty()) return poly();
        int n = a.size();
        vector<int> res(n + 1);
        for (int i = 0; i < n; ++i)
            res[i + 1] = 1ll * a[i] * power(i + 1, P - 2) % P;
        return poly(res);
    }
    poly inv(int m) const {
        poly x(power(a[0], P - 2));
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (2 - modxk(k) * x)).modxk(k);
        }
        return x.modxk(m);
    }
    poly rev() const {
        return poly(vector<int>(a.rbegin(), a.rend()));
    }
    poly log(int m) const {
        return (derivative() * inv(m)).integral().modxk(m);
    }
    poly exp(int m) const {
        poly x(1);
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x * (1 - x.log(k) + modxk(k))).modxk(k);
        }
        return x.modxk(m);
    }
    poly sqrt(int m) const {
        poly x(1);
        int k = 1;
        while (k < m) {
            k *= 2;
            x = (x + (modxk(k) * x.inv(k)).modxk(k)) * ((P + 1) / 2);
        }
        return x.modxk(m);
    }
    poly mulT(const poly& b) const {
        if (b.size() == 0)
            return poly();
        int n = b.size();
        return ((*this) * b.rev()).divxk(n - 1);
    }
    pair<poly, poly> divide(const poly& g) const {
        int n = a.size(), m = g.size();
        if (n < m) return make_pair(poly(), a);

        poly fR = rev();
        poly gR = g.rev().alignxk(n - m + 1);
        poly gRI = gR.inv(gR.size());

        poly qR = (fR * gRI).modxk(n - m + 1);

        poly q = qR.rev();

        poly r = ((*this) - g * q).modxk(m - 1);

        return make_pair(q, r);
    }
    vector<int> eval(vector<int> x) const {
        if (size() == 0)
            return vector<int>(x.size(), 0);
        const int n = max(int(x.size()), size());
        vector<poly> q(4 * n);
        vector<int> ans(x.size());
        x.resize(n);
        function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                q[p] = vector<int>{1, (P - x[l]) % P};
            } else {
                int m = (l + r) / 2;
                build(2 * p, l, m);
                build(2 * p + 1, m, r);
                q[p] = q[2 * p] * q[2 * p + 1];
            }
        };
        build(1, 0, n);
        function<void(int, int, int, const poly &)> work = [&](int p, int l, int r, const poly &num) {
            if (r - l == 1) {
                if (l < int(ans.size()))
                    ans[l] = num[0];
            } else {
                int m = (l + r) / 2;
                work(2 * p, l, m, num.mulT(q[2 * p + 1]).modxk(m - l));
                work(2 * p + 1, m, r, num.mulT(q[2 * p]).modxk(r - m));
            }
        };
        work(1, 0, n, mulT(q[1].inv(n)));
        return ans;
    }
};
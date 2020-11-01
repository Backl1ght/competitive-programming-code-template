namespace Backlight {

namespace Pollard_Rho {
    typedef long long ll;
    typedef pair<ll, ll> PLL;
    mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

    const int N = 1010000;
    ll C, fac[10010], n, mut, a[1001000];
    int T, cnt, i, l, prime[N], p[N], psize, _cnt;
    ll _e[100], _pr[100];
    vector<ll> d;

    inline ll mul(ll a, ll b, ll p) {
        if (p <= 1000000000) return a * b % p;
        else if (p <= 1000000000000ll) return (((a*(b>>20)%p)<<20)+(a*(b&((1<<20)-1))))%p;
        else {
            ll d = (ll)floor(a*(long double)b / p + 0.5);
            ll ret = (a * b - d * p) % p;
            if (ret < 0) ret += p;
            return ret;
        }
    }

    void prime_table(){
        int i, j, tot, t1;
        for (i = 1; i <= psize; i++) p[i] = i;
        for (i = 2, tot = 0; i <= psize; i++) {
            if (p[i] == i) prime[++tot] = i;
            for (j = 1; j <= tot && (t1 = prime[j] * i) <= psize; j++){
                p[t1] = prime[j];
                if (i % prime[j] == 0) break;
            }
        }
    }
    
    void init(int ps) {
        psize = ps;
        prime_table();
    }

    ll powl(ll a, ll n, ll p) {
        ll ans = 1;
        for (; n; n >>= 1) {
            if (n & 1) ans = mul(ans, a, p);
            a = mul(a, a, p);
        }
        return ans;
    }

    bool witness(ll a, ll n) {
        int t = 0;
        ll u = n - 1;
        for (; ~u&1; u >>= 1) t++;
        ll x = powl(a, u, n), _x = 0;
        for (; t; t--) {
            _x = mul(x, x, n);
            if (_x == 1 && x != 1 && x != n - 1) return 1;
            x = _x;
        }
        return _x != 1;
    }

    bool miller(ll n) {
        if (n < 2) return 0;
        if (n <= psize) return p[n] == n;
        if (~n & 1) return 0;
        for (int j = 0; j <= 7; j++) if (witness(rnd() % (n - 1) + 1, n)) return 0;
        return 1;
    }

    ll gcd(ll a,ll b) {
        ll ret = 1;
        while (a != 0) {
            if ((~a&1) && (~b&1)) ret <<= 1, a >>= 1,b >>= 1;
            else if (~a&1) a >>= 1; 
            else if (~b&1) b >>= 1;
            else {
                if (a < b) swap(a, b);
                a -= b;
            }
        }
        return ret * b;
    }

    ll rho(ll n) {
        for (;;) {
            ll X = rnd() % n, Y, Z, T = 1, *lY = a, *lX = lY;
            int tmp = 20;
            C = rnd() % 10 + 3;
            X = mul(X, X, n) + C; *(lY++) = X; lX++;
            Y = mul(X, X, n) + C; *(lY++) = Y;
            for(; X != Y;) {
                ll t = X - Y + n;
                Z = mul(T, t, n);
                if(Z == 0) return gcd(T, n);
                tmp--;
                if (tmp == 0) {
                    tmp = 20;
                    Z = gcd(Z, n);
                    if (Z != 1 && Z != n) return Z;
                }
                T = Z;
                Y = *(lY++) = mul(Y, Y, n) + C;
                Y = *(lY++) = mul(Y, Y, n) + C;
                X = *(lX++);
            }
        }
    }

    void _factor(ll n) {
        for (int i = 0; i < cnt; i++) {
            if (n % fac[i] == 0) n /= fac[i], fac[cnt++] = fac[i];
        }
        if (n <= psize) {
            for (; n != 1; n /= p[n]) fac[cnt++] = p[n];
            return;
        }
        if (miller(n)) fac[cnt++] = n;
        else {
            ll x = rho(n);
            _factor(x); _factor(n / x);
        }
    }

    void dfs(ll x,int dep) {
        if (dep == _cnt) d.push_back(x);
        else {
            dfs(x, dep+1);
            for (int i = 1; i <= _e[dep]; i++) dfs(x *=_pr[dep], dep + 1);
        }
    }

    void norm() {
        sort(fac, fac + cnt);
        _cnt = 0;
        for(int i = 0; i < cnt; ++i) 
            if (i == 0 || fac[i] != fac[i-1]) _pr[_cnt] = fac[i], _e[_cnt++] = 1;
            else _e[_cnt-1]++;
    }
    
    vector<ll> getd() {
        d.clear();
        dfs(1, 0);
        return d;
    }

    /**************************************************/

    // Attention: call init() before use

    // get all factors
    vector<ll> factorA(ll n) {
        cnt = 0;
        _factor(n);
        norm();
        vector<ll> d = getd();
        sort(d.begin(), d.end());
        return d;
    }

    // get prime factors
    vector<ll> factorP(ll n) {
        cnt = 0;
        _factor(n);
        norm();
        vector<ll> d(_cnt);
        for (int i = 0; i < _cnt; ++i) d[i] = _pr[i];
        return d;
    }

    // get prime factors, n = pr_i^e_i
    vector<PLL> factorG(ll n) {
        cnt = 0;
        _factor(n);
        norm();
        vector<PLL> d(_cnt);
        for (int i = 0; i < _cnt; ++i) d[i] = make_pair(_pr[i], _e[i]);
        return d;
    }

    bool is_primitive(ll a,ll p) {
        assert(miller(p));
        vector<PLL> D = factorG(p - 1);
        for (int i = 0; i < (int)D.size(); ++i) if (powl(a, (p-1) / D[i].first, p) == 1) return 0;
        return 1;
    }
}

}
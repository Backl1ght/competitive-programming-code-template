// use this when n, m is really large and p is small
namespace Lucas {
    inline ll pow(ll a, ll b, ll p) {
        ll res = 1;
        a %= p;
        while(b) {
            if (b & 1) res = res * a % p;
            a = a * a % p;
            b >>= 1;
        }
        return res;
    }

    inline ll inv1(ll n, ll p) { return pow(n, p - 2, p); }

    inline ll C1(ll n, ll m, ll p) {
        if (m > n) return 0;
        if (m > n - m) m = n - m;
        ll u = 1, d = 1;
        for (ll i = 1; i <= m; ++i) {
            u = u * (n - i + 1) % p;
            d = d * i % p;
        }
        return u * inv1(d, p) % p;
    }

    // solve n choose m (mod p) while p is a prime
    ll lucas(ll n, ll m, ll p) {
        if (m == 0) return 1;
        return C1(n % p, m % p, p) * lucas(n / p, m / p, p) % p;
    }


    ll exgcd(ll a, ll b, ll& x, ll& y) {
        if (b == 0) {
            x = 1; y = 0; 
            return a; 
        }
        ll d = exgcd(b, a % b, x, y);
        ll z = x; x = y; y = z - y * (a / b);
        return d;
    }

    inline ll inv2(ll n, ll p) {
        ll x, y;
        ll d = exgcd(n, p, x, y);
        return d == 1 ? (p + x % p) % p : -1;
    }

    // n! mod pk without pi^x
    ll f(ll n, ll pi, ll pk) {
        if (!n) return 1;
        ll res = 1;
        if (n / pk) {
            for (ll i = 2; i <= pk; ++i)
                if (i % pi) res = res * i % pk;
            res = pow(res, n / pk, pk);
        }
        for (ll i = 2; i <= n % pk; ++i)
            if (i % pi) res = res * i % pk;
        return res * f(n / pi, pi, pk) % pk;
    }

    ll C2(ll n, ll m, ll p, ll pi, ll pk) {
        if (m > n) return 0;
        ll a = f(n, pi, pk), b = f(m, pi, pk), c = f(n - m, pi, pk);
        ll k = 0;
        for (ll i = n; i; i /= pi) k += i / pi;
        for (ll i = m; i; i /= pi) k -= i / pi;
        for (ll i = n - m; i; i /= pi) k -= i / pi;
        ll ans = a * inv2(b, pk) % pk * inv2(c, pk) % pk * pow(pi, k, pk) % pk;
        ans = ans * (p / pk) % p * inv2(p / pk, pk) % p;
        return ans;
    }

    // solve n choose m (mod p) while p is a prime
    ll exlucas(ll n, ll m, ll p) {
        ll x = p;
        ll ans = 0;
        for (ll i = 2; i <= p; ++i) {
            if (x % i == 0) {
                ll pk = 1;
                while(x % i == 0) pk *= i, x /= i;
                ans = (ans + C2(n, m, p, i, pk)) % p;
            }
        }
        return ans;
    }

} // namespace Lucas

} // namespace Backlight
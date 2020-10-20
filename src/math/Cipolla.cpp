namespace Backlight {

namespace Cipolla {
    mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
    ll W, P;
    struct complex {
        ll r, i;
        complex(ll _r, ll _i) : r(_r), i(_i) {}
        inline complex operator * (const complex& c) const { return complex((r * c.r % P + i * c.i % P * W) % P, (r * c.i % P + i * c.r % P) % P); }
    };

    inline complex pow(complex a, int b) {
        complex res(1, 0);
        while(b) {
            if (b & 1) res = res * a;
            a = a * a;
            b >>= 1;
        }
        return res;
    }

    inline ll pow(ll a, ll b, ll p) {
        ll res = 1;
        while(b) {
            if (b & 1) res = res * a % p;
            a = a * a % p;
            b >>= 1;
        }
        return res;
    }
    
    // solve x for x^2 = a (mod p)
    ll solve(ll a, ll p) {
        P = p; a %= p;
        if (a == 0) return 0;

        ll t = pow(a, (p - 1) / 2, p);
        if (t != 1) return -1;
        while(true) {
            t = rnd() % p;
            ll c = (t * t % p + p - a) % p;
            if (pow(c, (p - 1) / 2, p) == p - 1) break;
        }

        W = (t * t % p + p - a) % p;
        ll x = pow(complex(t, 1), (p + 1) / 2).r;
        return x;
    }

} // namespace Cipolla

} // namespace Backlight
namespace Backlight {

namespace BSGS {
    typedef long long ll;

    ll exgcd(ll a, ll b, ll& x, ll& y) {
        if (b == 0) {
            x = 1; y = 0; 
            return a; 
        }
        ll d = exgcd(b, a % b, x, y);
        ll z = x; x = y; y = z - y * (a / b);
        return d;
    }

    ll qpow(ll a, ll n, ll p) {
        ll ans = 1;
        for (; n; n >>= 1) {
            if (n & 1) ans = ans *  a % p;
            a = a * a % p;
        }
        return ans;
    }

    ll BSGS(ll a, ll b, ll p) {
        unordered_map<ll, int> mp;
        if(__gcd(a, p) != 1) return -1;
        if(b % p == 1) return 0;
        a %= p; b %= p;
        ll k = sqrt(p), t = qpow(a, k, p), s = b;
        for(int i = 0; i <= k; i++, s = s * a % p) mp[s] = i;
        s=1;
        for(int i = 0; i <= k; i++, s = s * t % p) {
            int ans = mp.count(s) ? mp[s] : -1;
            if(ans != -1 && i * k - ans >= 0) return i* k - ans;
        }
        return -1;
    }

    ll EXBSGS(ll a, ll b, ll mod) {
        ll k = 0, d, c = 1, x, y;
        a %= mod; b %= mod;
        if(a == b) return 1;
        if(b == 1) return 0;
        while((d=__gcd(a,mod)) != 1) {
            if(b % d) return -1;
            k++; b /= d; mod /= d; c = c * (a / d) % mod;
            if(c == b) return k;
        }
        if(mod == 1) return k;
        exgcd(c, mod, x, y); b = (b * x % mod + mod) % mod; a %= mod;
        ll ans = BSGS(a, b, mod);
        return ans == -1 ? ans : ans + k;
    }
}

}
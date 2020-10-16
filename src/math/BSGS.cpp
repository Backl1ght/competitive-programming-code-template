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

    // solve a^x = b (mod p), p is a prime must hold
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

    // solve a^x = b (mod p), p don't need to be a prime
    ll EXBSGS(ll a, ll b, ll p) {
        ll k = 0, d, c = 1, x, y;
        a %= p; b %= p;
        if(a == b) return 1;
        if(b == 1) return 0;
        while((d=__gcd(a,p)) != 1) {
            if(b % d) return -1;
            k++; b /= d; p /= d; c = c * (a / d) % p;
            if(c == b) return k;
        }
        if(p == 1) return k;
        exgcd(c, p, x, y); b = (b * x % p + p) % p; a %= p;
        ll ans = BSGS(a, b, p);
        return ans == -1 ? ans : ans + k;
    }
}

}
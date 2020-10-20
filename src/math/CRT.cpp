namespace Backlight {

// get x, y for ax + by = GCD(a, b)
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = 1; y = 0; 
        return a; 
    }
    ll d = exgcd(b, a % b, x, y);
    ll z = x; x = y; y = z - y * (a / b);
    return d;
}

    

// CRT: solve x = a_i (mod m_i) for i in [0, n)

// GCD(m_i, m_j) = 1 hold
ll CRT(vector<ll>& a, vector<ll>& m) {
    assert(a.size() == m.size());
    assert(a.size() > 0);
    int n = a.size();
    ll M = 1, res = 0;
    for (int i = 0; i < n; ++i) M *= m[i];
    ll _M, x, y;
    for (int i = 0; i < n; ++i) {
        _M = M / m[i];
        exgcd(_M, m[i], x, y);
        res = (res + a[i] * _M % M * x % M) % M;
    }
    if (res < 0) res += M;
    return res;
}

ll mul(ll a, ll b, ll mod) {
    ll res = 0;
    while(b) {
        if (b & 1) res = (res + a) % mod;
        b >>= 1;
        a = (a + a) % mod;
    }
    return res;
}

// GCD(m_i, m_j) = 1 not hold
ll EXCRT(vector<ll>& a, vector<ll>& m) {
    assert(a.size() == m.size());
    assert(a.size() > 0);
    int n = a.size();
    ll res = a[0], M = m[0], B, g, x, y;
    for (int i = 1; i < n; ++i) {
        B = ((a[i] - res) % m[i] + m[i]) % m[i];
        g = exgcd(M, m[i], x, y);
        x = mul(x, B / g, m[i]);
        res += M * x;
        M *= m[i] / g;
        res = (res + M) % M;
    }
    return res;
}


}
namespace Backlight {
    
ll qp(ll a, ll b) {
    ll res = 1;
    a %= MOD;
    assert(b >= 0);
    while(b){
        if(b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}
ll inv(ll x) {return qp(x, MOD - 2);}
ll factor[N], finv[N];
void init() {
    factor[0]=1;
    for(int i=1; i<N; i++) factor[i] = factor[i-1] * i % MOD;
    finv[N-1] = qp(factor[N-1], MOD - 2);
    for(int i=N-2; i>=0; i--) finv[i] = finv[i+1] * (i+1) % MOD;
}
ll c(ll n, ll m) {
    return factor[n] * finv[m] % MOD * finv[n-m] % MOD;
}

}
int primes[N], pcnt;
bool is[N];
int mu[N]; // 莫比乌斯函数，在这里是其前缀和
void seive() {
    pcnt = 0; mu[1] = 1;
    for (int i = 2; i < N; ++i) is[i] = true;
    for (int i = 2; i < N; ++i) {
        if (is[i]) primes[++pcnt] = i, mu[i] = -1;
        for (int j = 1; j <= pcnt; ++j) {
            ll nxt = 1ll * i * primes[j];
            if (nxt >= N) break;
            is[nxt] = false;
            if (i % primes[j] == 0) {
                mu[nxt] = 0;
                break;
            }
            mu[nxt] = -mu[i];
        }
    }
    for (int i = 1; i < N; ++i) mu[i] += mu[i - 1];
}
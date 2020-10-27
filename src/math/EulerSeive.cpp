namespace Backlight {

vector<int> euler_seive(int n) {
    vector<int> primes;
    vector<bool> is(n + 1, 1);

    for (int i = 2; i <= n; ++i) {
        if (is[i]) primes.push_back(i);
        for (int j = 0; j < (int)primes.size(); ++j) {
            ll nxt = 1ll * primes[j] * i;
            if (nxt > n) break;
            is[nxt] = false;
            if (i % primes[j] == 0) break;
        }
    }
    return primes;
}

}
struct LinearBasis {
    static const int B = 62;
    ll b[B];
    int tot, n;

    LinearBasis() {
        tot = 0; n = 0;
        memset(b, 0, sizeof(b));
    }

    bool insert(ll x) {
        ++n;
        for (int i = B - 1; i >= 0; --i) {
            if (!(x >> i)) continue;
            if (!b[i]) {
                ++tot;
                b[i] = x;
                break;
            }
            x ^= b[i];
        }
        return x > 0;
    }

    bool query(ll x) {
        for (int i = B - 1; i >= 0; --i) {
            if (!(x >> i)) continue;
            if (!b[i]) return false;
            x ^= b[i];
        }
        return x == 0;
    }

    ll queryMax() {
        ll res = 0;
        for (int i = B - 1; i >= 0; --i) {
            if ((res ^ b[i]) > res) res ^= b[i];
        }
        return res;
    }

    ll queryMin() {
        for (int i = 0; i < B; ++i) if (b[i]) return b[i];
        return -1;
    }

    ll count() {
        return 1LL << tot;
    }

    void rebuild() {
        for (int i = B - 1; i >= 0; --i) {
            for (int j = i - 1; j >= 0; --j) {
                if (b[i] & (1LL << j))
                    b[i] ^= b[j];
            }
        }
    }

    // need rebuid first
    ll queryKth(int k) {
        if (k == 1 && tot < n) return 0;
        if (tot < n) --k;
        if (k > (1LL << tot) - 1) return -1;
        ll res = 0;
        for (int i = 0; i < B; ++i) {
            if (b[i]) {
                if (k & 1) res ^= b[i];
                k >>= 1;
            }
        }
        return res;
    }
};
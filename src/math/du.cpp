#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;

const int LIM = 1e7;

int pcnt, prime[LIM], mu[LIM];
bool vis[LIM];
void seive(int n) {
  pcnt = 0;
  mu[0] = 0;
  mu[1] = 1;
  for (int i = 2; i <= n; ++i) {
    if (!vis[i]) {
      prime[++pcnt] = i;
      mu[i] = -1;
    }
    for (int j = 1; j <= pcnt; ++j) {
      ll nxt = 1ll * i * prime[j];
      if (nxt > n)
        break;
      vis[nxt] = true;
      if (i % prime[j] == 0) {
        mu[nxt] = 0;
        break;
      }
      mu[nxt] = -mu[i];
    }
  }
  for (int i = 1; i <= n; ++i)
    mu[i] += mu[i - 1];
}

map<ll, ll> mp;

// $ S(n) = 1 - \sum_{i=2} S(\lfloor \frac{n}{i} \rfloor) $
// Time Complexity: $O(n^{\frac{2}{3}})$
ll S_mu(ll n) {
  if (n < LIM)
    return mu[n];
  if (mp.count(n))
    return mp[n];

  ll ret = 0;
  for (ll i = 2, j; i <= n; i = j + 1) {
    j = n / (n / i);
    ret += (j - i + 1) * S_mu(n / i);
  }
  ret = 1 - ret;

  mp[n] = ret;
  return ret;
}

// $ S(n) = \frac{(n + 1)n}{2} - \sum_{i=2}^{n} S(\lfloor \frac{n}{i} \rfloor) $
// $ S(n) = \sum_{d=1}^{n} \mu(d) \lfloor \frac{n}{d} \rfloor \lfloor \frac{n}{d} \rfloor$
ll S_phi(ll n) {
  ll ret = 0;
  for (ll i = 1, j; i <= n; i = j + 1) {
    j = n / (n / i);
    ret += (S_mu(j) - S_mu(i - 1)) * (n / i) * (n / i);
  }
  ret = (ret - 1) / 2 + 1;
  return ret;
}

void solve(int Case) {
  ll n;
  scanf("%lld", &n);
  printf("%lld %lld\n", S_phi(n), S_mu(n));
}

int main() {
  seive(LIM - 1);
  int T = 1;
  scanf("%d", &T);
  for (int _ = 1; _ <= T; _++)
    solve(_);
  return 0;
}
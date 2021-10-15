namespace Hash {
// 1 based, double hash
typedef long long ll;
const ll P1 = 29;
const ll P2 = 131;
const ll MOD1 = 1e9 + 7;
const ll MOD2 = 1e9 + 9;
ll p1[N], p2[N], h1[N], h2[N];
void init_hash(char* s, int n) {
  p1[0] = p2[0] = 1;
  for (int i = 1; i <= n; i++)
    p1[i] = (p1[i - 1] * P1) % MOD1;
  for (int i = 1; i <= n; i++)
    p2[i] = (p2[i - 1] * P2) % MOD2;
  for (int i = 1; i <= n; i++)
    h1[i] = (h1[i - 1] * P1 + s[i]) % MOD1;
  for (int i = 1; i <= n; i++)
    h2[i] = (h2[i - 1] * P2 + s[i]) % MOD2;
}

ll get_hash(int l, int r) {
  ll H1 = ((h1[r] - h1[l - 1] * p1[r - l + 1]) % MOD1 + MOD1) % MOD1;
  ll H2 = ((h2[r] - h2[l - 1] * p2[r - l + 1]) % MOD2 + MOD2) % MOD2;
  return H1 * MOD2 + H2;
}
}  // namespace Hash
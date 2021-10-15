namespace Backlight {

namespace NTT {
// 998244353, 1004535809
const int P = 998244353, G = 3, Gi = 332748118;

inline ll pow(ll a, ll b) {
  ll res = 1;
  a %= P;
  while (b) {
    if (b & 1)
      res = res * a % P;
    a = a * a % P;
    b >>= 1;
  }
  return res;
}

int N, L;
vector<ll> r;
void init(vector<ll>& a, vector<ll>& b) {
  int l = a.size() + b.size();
  N = 1;
  L = 0;
  while (N < l)
    N <<= 1, ++L;
  a.resize(N);
  b.resize(N);
  r.resize(N);
  for (int i = 0; i < N; ++i)
    r[i] = (r[i >> 1] >> 1) | ((i & 1) << (L - 1));
}

void work(vector<ll>& a, int flag) {
  for (int i = 0; i < N; i++)
    if (i < r[i])
      swap(a[i], a[r[i]]);
  for (int mid = 1; mid < N; mid <<= 1) {
    ll wn = pow(flag == 1 ? G : Gi, (P - 1) / (mid << 1));
    for (int j = 0; j < N; j += (mid << 1)) {
      ll w = 1;
      for (int k = 0; k < mid; k++, w = (w * wn) % P) {
        int x = a[j + k], y = w * a[j + k + mid] % P;
        a[j + k] = (x + y) % P,
              a[j + k + mid] = (x - y + P) % P;
      }
    }
  }
}

inline void NTT(vector<ll>& a) {
  work(a, 1);
}
inline void INTT(vector<ll>& a) {
  work(a, -1);
}

vector<ll> convolution(vector<ll> a, vector<ll> b) {
  init(a, b);
  NTT(a);
  NTT(b);
  for (int i = 0; i < N; ++i)
    a[i] = a[i] * b[i] % P;
  INTT(a);
  ll inv = pow(N, P - 2);
  for (int i = 0; i < N; ++i)
    a[i] = a[i] * inv % P;
  return a;
}
}  // namespace NTT

}  // namespace Backlight
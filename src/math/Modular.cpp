const int MOD = 1e9 + 7;
int add(int x, int y) {
  return x + y >= MOD ? x + y - MOD : x + y;
}
int mul(int x, int y) {
  return 1ll * x * y % MOD;
}
int sub(int x, int y) {
  return x - y < 0 ? x - y + MOD : x - y;
}
int qp(int x, int y) {
  int r = 1;
  while (y) {
    if (y & 1)
      r = mul(r, x);
    x = mul(x, x);
    y >>= 1;
  }
  return r;
}
int dvd(int x, int y) {
  return 1ll * x * qp(y, MOD - 2) % MOD;
}
namespace SA {
// 0 based, 倍增法构建， O(nlogn)
int height[N], c[N], x[N], y[N], sa[N], rk[N];
void build_sa(int* s, int n) {
  n++;
  int i, j, k, m = 256;  //m为字符集大小, max(s[i])<m
  for (i = 0; i < m; i++)
    c[i] = 0;
  for (i = 0; i < n; i++)
    c[x[i] = s[i]]++;
  for (i = 1; i < m; i++)
    c[i] += c[i - 1];
  for (i = n - 1; i >= 0; i--)
    sa[--c[x[i]]] = i;
  for (j = 1; j <= n; j <<= 1) {
    k = 0;
    for (i = n - j; i < n; i++)
      y[k++] = i;
    for (i = 0; i < n; i++)
      if (sa[i] >= j)
        y[k++] = sa[i] - j;
    for (i = 0; i < m; i++)
      c[i] = 0;
    for (i = 0; i < n; i++)
      c[x[y[i]]]++;
    for (i = 1; i < m; i++)
      c[i] += c[i - 1];
    for (i = n - 1; i >= 0; i--)
      sa[--c[x[y[i]]]] = y[i];
    swap(x, y);
    m = 0;
    x[sa[0]] = m++;
    for (i = 1; i < n; i++) {
      if (y[sa[i]] == y[sa[i - 1]] && y[sa[i] + j] == y[sa[i - 1] + j])
        x[sa[i]] = m - 1;
      else
        x[sa[i]] = m++;
    }
    if (m >= n)
      break;
  }
  k = 0;
  for (i = 0; i < n; i++)
    rk[sa[i]] = i;
  for (i = 0; i < n - 1; i++) {
    if (k)
      k--;
    j = sa[rk[i] - 1];
    while (s[i + k] == s[j + k])
      k++;
    height[rk[i]] = k;
  }
}
}  // namespace SA
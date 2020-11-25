const int LG = log2(N) + 1;
int mi[N][LG], lg[N];
void init_rmq(int n) {
    lg[1] = 0;
    for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
}

void build_rmq(int n, int* a) {
    for (int i = 1; i <= n; ++i) mi[i][0] = a[i];
    for (int j = 1; j <= lg[n]; ++j) {
        for (int i = 1; i + (1 << j) <= n; ++i) {
            mi[i][j] = min(mi[i][j - 1], mi[i + (1 << (j - 1))][j - 1]);
        }
    }
}

int rmqMin(int l, int r) {
    int k = lg[r - l + 1];
    return min(mi[l][k], mi[r - (1 << k) + 1][k]);
}
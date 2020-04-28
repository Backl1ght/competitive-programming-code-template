#include <bits/stdc++.h>
using namespace std;

const int N = 1e4 + 5;
const int M = 1e6 + 5;
int s[N], t[M];
int n, m, nxt[N], fail[N];
void get_fail() {
    int k = 0;
    for (int i = 1; i <= n; i++) {
        nxt[i] = k;
        fail[i] = (s[i] == s[k]) ? fail[k] : k;
        while(k && s[i] != s[k]) k = fail[k];
        if (s[i] == s[k]) k++;
    }
}

int match() {
    int pos = -1;
    int j =0;
    for (int i = 0; i < m; i++) {
        while(j && t[i] != s[j]) j = fail[j];
        if (t[i] == s[j]) {
            j++;
            if (j == n) {
                pos = i + 2 - n;
                break;
            }
        }
    }
    return pos;
}

int repete() {
    int len = m - nxt[m];
    return m % len ? m : len;
}

void solve(int Case) {
    scanf("%d %d", &m, &n);
    for(int i = 0; i < m; i++) scanf("%d", &t[i]);
    for(int i = 0; i < n; i++) scanf("%d", &s[i]);
    get_fail();
    printf("%d\n", match());
}

int main()
{
    int T; scanf("%d", &T); for(int i=1;i<=T;i++) solve(i);
    return 0;
}
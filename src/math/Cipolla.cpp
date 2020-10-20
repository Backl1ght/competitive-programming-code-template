#include <bits/stdc++.h>
using namespace std;
 
// #include <ext/rope>
// using namespace __gnu_cxx;c
 
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>
// using namespace __gnu_pbds;
 
// typedef ll key_type;
// typedef null_mapped_type value_type;
// typedef tree<key_type, value_type, less<key_type>, rb_tree_tag, tree_order_statistics_node_update> rbtree;
 
// typedef __gnu_pbds::priority_queue<pi,greater<pi>,pairing_heap_tag > heap;
 
// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
// int rnd(int l,int r){return l+rng()%(r-l+1);}
 
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int,int> PII;
typedef vector<int> VI;
typedef vector<ll> VL;
typedef vector<vector<int>> VVI;
typedef vector<vector<ll>> VVL;
 
#define REP(i, _, __) for (int i = (_); i < (__); ++i)
#define PER(i, _, __) for (int i = (_-1); i >= (__); --i)
#define FOR(i, _, __) for (int i = (_); i <= (__); ++i)
#define ROF(i, _, __) for (int i = (_); i >= (__); --i)
#define FE(v, V) for (auto& v: V)

#define EB emplace_back
#define PB push_back
#define MP make_pair
#define FI first
#define SE second
#define SZ(x) ((int)(x).size())
#define ALL(x) (x).begin(),(x).end()
#define LLA(x) (x).rbegin(),(x).rend()

const double PI = acos(-1.0);
   
namespace Backlight {
    inline bool isdowncase(char c) { return (c >= 'a') && (c <= 'z'); }
    inline bool isupcase(char c) { return (c >= 'A') && (c <= 'Z'); }
    inline bool isdigit(char c) { return (c >= '0') && (c <= '9'); }

    template<typename T>
    T MIN(T a, T b) { return min(a, b); }

    template<typename First, typename... Rest>
    First MIN(First f, Rest... r) { return min(f, MIN(r...)); }

    template<typename T>
    T MAX(T a, T b) { return max(a, b); }

    template<typename First, typename... Rest>
    First MAX(First f, Rest... r) { return max(f, MAX(r...)); }

    template<typename T>
    void updMin(T& a, T b) { if (a > b) a = b; }

    template<typename T>
    void updMax(T& a, T b) { if (a < b) a = b; }

    bool NEOF = 1; //为0表示文件结尾
    const int BUFFER_SIZE = 1 << 20;
    inline char nc() {
        static char buf[BUFFER_SIZE], *p1 = buf, *p2 = buf;
        return p1 == p2 && NEOF && (p2 = (p1 = buf) + fread(buf, 1, BUFFER_SIZE, stdin), p1 == p2) ? (NEOF = 0, EOF) : *p1++;
    }
   
    template<typename T>
    inline bool read(T &x) {
        char c = nc();
        bool f = 0; x = 0;
        while (!isdigit(c)) c == '-' && (f = 1), c = nc();
        while (isdigit(c)) x = (x << 3) + (x << 1) + (c ^ 48), c = nc();
        if (f) x = -x;
        return NEOF;
    }

    inline bool read_db(double &x){
        bool f = 0; char c = nc(); x = 0;
        while (!isdigit(c))  { f |= (c == '-'); c = nc(); }
        while (isdigit(c)) { x = x * 10.0 + (c ^ 48); c = nc(); }
        if (c == '.') {
            double temp = 1; c = nc();
            while (isdigit(c)) { temp = temp / 10.0; x = x + temp * (c ^ 48); c = nc(); }
        }
        if (f) x = -x;
        return NEOF;
    }

    template<typename T, typename... T2>
    inline bool read(T &x, T2 &... rest) {
        read(x);
        return read(rest...);
    }

    // inline bool need(char c) { return (c == '-') || (c == '>') || (c == '<');}
    // inline bool need(char c) { return isdowncase(c) || isupcase(c) || isdigit(c) || c == '.' || c == '#'; }
    inline bool need(char c) { return isdowncase(c) || isupcase(c) || isdigit(c); }

    inline bool read_str(char *a) {
        while ((*a = nc()) && need(*a) && NEOF) ++a; *a = '\0';
        return NEOF;
    }

    template<typename T>
    inline void println(T x, bool flag = true) {
        if (x < 0) putchar('-'), x = -x;
        if (x > 9) println(x / 10, false);
        putchar('0' + x % 10);
        if (flag) putchar('\n');
    }
   
    template<typename T>
    inline void _dbg(const char *format, T value) { cerr << format << '=' << value << endl; }
   
    template<typename First, typename... Rest>
    inline void _dbg(const char *format, First f, Rest... r) {
        while(*format != ',') cerr << *format++;
        cerr << '=' << f << ", ";
        _dbg(format + 1, r...);
    }
      
    template<typename T>
    ostream &operator<<(ostream& os, vector<T> V) {
        os << "[ "; for (auto v : V) os << v << ","; return os << " ]";
    }
   
    template<typename T>
    ostream &operator<<(ostream& os, set<T> V) {
        os << "[ "; for (auto v : V) os << v << ","; return os << " ]";
    }

    template<typename T>
    ostream &operator<<(ostream& os, multiset<T> V) {
        os << "[ "; for (auto v : V) os << v << ","; return os << " ]";
    }
 
    template<typename T1, typename T2>
    ostream &operator<<(ostream& os, map<T1,T2> V) {
        os << "[ "; for (auto v : V) os << v << ","; return os << " ]";
    }
  
    template<typename L, typename R>
    ostream &operator<<(ostream &os, pair<L,R> P) {
        return os << "(" << P.first << "," << P.second << ")";
    }
 
    #ifdef BACKLIGHT
    #define debug(...) cerr << "\033[31m"; _dbg(#__VA_ARGS__, __VA_ARGS__); cerr << "\033[0m";
    // #define debug(...) _dbg(#__VA_ARGS__, __VA_ARGS__); 
    #else
    #define debug(...)
    #endif
}

/************     Backlight     **********
 * 一发入魂
 * 仔细读题，注意边界条件
 * 没有思路就试试逆向思维
 * wdnmd!我柜子动了不打了
 * 能不能把我掉的分还给我
 ************     Backlight     **********/

using namespace Backlight;
const int N = 5e5 + 5;
const int M = 5e5 + 5;
const int V = 5e7 + 5;
const ll MOD = 1e9+ 7 ;              // 998244353 1e9+7
const int INF = 0x3f3f3f3f;             // 1e9+7 0x3f3f3f3f
const ll LLINF = 0x3f3f3f3f3f3f3f3f;    // 1e18+9 0x3f3f3f3f3f3f3f3f
const double eps = 1e-8;

namespace Backlight {

namespace Cipolla {
    mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
    ll W, P;
    struct complex {
        ll r, i;
        complex(ll _r, ll _i) : r(_r), i(_i) {}
        inline complex operator * (const complex& c) const { return complex((r * c.r % P + i * c.i % P * W) % P, (r * c.i % P + i * c.r % P) % P); }
    };

    inline complex pow(complex a, int b) {
        complex res(1, 0);
        while(b) {
            if (b & 1) res = res * a;
            a = a * a;
            b >>= 1;
        }
        return res;
    }

    inline ll pow(ll a, ll b, ll p) {
        ll res = 1;
        while(b) {
            if (b & 1) res = res * a % p;
            a = a * a % p;
            b >>= 1;
        }
        return res;
    }
    
    // solve x for x^2 = a (mod p)
    ll solve(ll a, ll p) {
        P = p; a %= p;
        if (a == 0) return 0;

        ll t = pow(a, (p - 1) / 2, p);
        if (t != 1) return -1;
        while(true) {
            t = rnd() % p;
            ll c = (t * t % p + p - a) % p;
            if (pow(c, (p - 1) / 2, p) == p - 1) break;
        }

        W = (t * t % p + p - a) % p;
        ll x = pow(complex(t, 1), (p + 1) / 2).r;
        return x;
    }

} // namespace Cipolla

} // namespace Backlight


void solve(int Case) { // printf("Case #%d: ", Case);
    ll a, p;
    read(a, p);
    ll a1 = Cipolla::solve(a, p), a2;
    if (a1 == -1) puts("Hola!");
    else {
        a2 = (-a1 + p) % p;
        if (a1 > a2) swap(a1, a2);
        if (a1 == a2) println(a1);
        else printf("%lld %lld\n", a1, a2);
    }
}

int main() {
#ifdef BACKLIGHT
    freopen("in.txt", "r", stdin);
#endif
    // ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int T = 1;
    read(T);
    for (int _ = 1; _ <= T; _++) solve(_);
    return 0;
}
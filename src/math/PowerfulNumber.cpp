#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;

constexpr int MOD = 1e9 + 7;  // 998244353 1e9 + 7
template < typename T >
inline int mint( T x )
{
    x %= MOD;
    if ( x < 0 )
        x += MOD;
    return x;
}
inline int add( int x, int y )
{
    return x + y >= MOD ? x + y - MOD : x + y;
}
inline int mul( int x, int y )
{
    return 1ll * x * y % MOD;
}
inline int sub( int x, int y )
{
    return x < y ? x - y + MOD : x - y;
}
inline int qp( int x, int y )
{
    int r = 1;
    for ( ; y; y >>= 1 )
    {
        if ( y & 1 )
            r = mul( r, x );
        x = mul( x, x );
    }
    return r;
}
inline int inv( int x )
{
    return qp( x, MOD - 2 );
}

namespace PNS
{
    const int N = 2e6 + 5;
    const int M = 35;

    ll global_n;

    int g[ N ], sg[ N ];

    int  h[ N ][ M ];
    bool vis_h[ N ][ M ];

    int ans;

    int  pcnt, prime[ N ], phi[ N ];
    bool isp[ N ];

    void sieve( int n )
    {
        pcnt = 0;
        for ( int i = 2; i <= n; ++i )
            isp[ i ] = true;
        phi[ 1 ] = 1;
        for ( int i = 2; i <= n; ++i )
        {
            if ( isp[ i ] )
            {
                ++pcnt;
                prime[ pcnt ] = i;
                phi[ i ]      = i - 1;
            }
            for ( int j = 1; j <= pcnt; ++j )
            {
                ll nxt = 1ll * i * prime[ j ];
                if ( nxt > n )
                    break;
                isp[ nxt ] = false;
                if ( i % prime[ j ] == 0 )
                {
                    phi[ nxt ] = phi[ i ] * prime[ j ];
                    break;
                }
                phi[ nxt ] = phi[ i ] * phi[ prime[ j ] ];
            }
        }

        for ( int i = 1; i <= n; ++i )
            g[ i ] = mul( i, phi[ i ] );

        sg[ 0 ] = 0;
        for ( int i = 1; i <= n; ++i )
            sg[ i ] = add( sg[ i - 1 ], g[ i ] );
    }

    int  inv2, inv6;
    void init()
    {
        sieve( N - 1 );
        for ( int i = 1; i <= pcnt; ++i )
            h[ i ][ 0 ] = 1, h[ i ][ 1 ] = 0;
        for ( int i = 1; i <= pcnt; ++i )
            vis_h[ i ][ 0 ] = vis_h[ i ][ 1 ] = true;
        inv2 = inv( 2 );
        inv6 = inv( 6 );
    }

    int S1( ll n )
    {
        return mul( mul( mint( n ), mint( n + 1 ) ), inv2 );
    }

    int S2( ll n )
    {
        return mul( mul( mint( n ), mul( mint( n + 1 ), mint( n * 2 + 1 ) ) ), inv6 );
    }

    map< ll, int > mp_g;

    int G( ll n )
    {
        if ( n < N )
            return sg[ n ];
        if ( mp_g.count( n ) )
            return mp_g[ n ];

        int ret = S2( n );
        for ( ll i = 2, j; i <= n; i = j + 1 )
        {
            j   = n / ( n / i );
            ret = sub( ret, mul( sub( S1( j ), S1( i - 1 ) ), G( n / i ) ) );
        }
        mp_g[ n ] = ret;
        return ret;
    }

    void dfs( ll d, int hd, int pid )
    {
        ans = add( ans, mul( hd, G( global_n / d ) ) );

        for ( int i = pid, p; i <= pcnt; ++i )
        {
            if ( i > 1 && d > global_n / prime[ i ] / prime[ i ] )
                break;

            int c = 2;
            for ( ll x = d * prime[ i ] * prime[ i ]; x <= global_n; x *= prime[ i ], ++c )
            {
                if ( !vis_h[ i ][ c ] )
                {
                    int f = qp( prime[ i ], c );
                    f     = mul( f, sub( f, 1 ) );
                    int g = mul( prime[ i ], prime[ i ] - 1 );
                    int t = mul( prime[ i ], prime[ i ] );

                    for ( int j = 1; j <= c; ++j )
                    {
                        f = sub( f, mul( g, h[ i ][ c - j ] ) );
                        g = mul( g, t );
                    }
                    h[ i ][ c ]     = f;
                    vis_h[ i ][ c ] = true;
                }

                if ( h[ i ][ c ] )
                    dfs( x, mul( hd, h[ i ][ c ] ), i + 1 );
            }
        }
    }

    int solve( ll n )
    {
        global_n = n;
        ans      = 0;
        dfs( 1, 1, 1 );
        return ans;
    }
}  // namespace PNS

int main()
{
    PNS::init();
    ll n;
    scanf( "%lld", &n );
    printf( "%d\n", PNS::solve( n ) );
    return 0;
}
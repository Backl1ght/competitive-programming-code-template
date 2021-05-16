#include <bits/stdc++.h>

using namespace std;
using ll = int64_t;

const int MOD = 1e9 + 7;

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
inline int dvd( int x, int y )
{
    return 1ll * x * qp( y, MOD - 2 ) % MOD;
}
inline void inc( int &x, int y )
{
    x += y;
    if ( x >= MOD )
        x -= MOD;
}
inline void dec( int &x, int y )
{
    x -= y;
    if ( x < 0 )
        x += MOD;
}

namespace min25
{
    /*
    calc the prefix sum of  multiplicative function.
    Requirements:
        Assume $p$ is a prime number.
        1. $f(p)$ is a polynomial with small deg or can be calc quickly.
        2. $f(p^e)$ can be calc quickly.
    Time complexity: $O(\frac{n^{0.75}}{\log n})$
    Steps: assume $deg(f(p)) = n$.
        1. split $f(p)$ into $n$ parts by it exponent.
        2. calc them separately.
        3. then sum them up.
    e.g.: $f(p) = \phi(p) = p - 1$
    1. calc $ans_0$ for $f(p) = p$.
    2. calc $ans_1$ for $f(p) = 1$.
    3. $ans = ans_0 - ans_1$.
    */
    const int LIM = 2e5 + 9;

    ll  gn, w[ LIM ];
    int rt, lim, id1[ LIM ], id2[ LIM ];
#define idx( v ) ( v <= rt ? id1[ v ] : id2[ gn / v ] )

    int  pcnt, prime[ LIM ];
    bool isp[ LIM ];

    // $ sp_{i, j} = \sum_{k=1}^j [\text{p is a prime}]p^{i} $
    int sp1[ LIM ];

    void seive( const int &n )
    {
        pcnt = 0;
        fill(isp, isp + n + 1, true);
        for ( int i = 2; i <= n; ++i )
        {
            if ( isp[ i ] )
            {
                ++pcnt;
                prime[ pcnt ] = i;
            }
            for ( int j = 1; j <= pcnt; ++j )
            {
                ll nxt = 1ll * i * prime[ j ];
                if ( nxt > n )
                    break;
                isp[ nxt ] = false;
                if ( i % prime[ j ] == 0 )
                    break;
            }
        }
        for ( int i = 1; i <= pcnt; ++i )
            sp1[ i ] = add( sp1[ i - 1 ], prime[ i ] );
    }

    int G[ LIM ][ 2 ], H[ LIM ];

    void initG0( const ll &n )
    {
        lim      = 0;
        int inv2 = inv( 2 );
        for ( ll i = 1, j, v; i <= n; i = n / j + 1 )
        {
            j = n / i;

            w[ ++lim ] = j;
            idx( j )   = lim;

            v = j % MOD;

            // init $G_0 = \sum_{i=2}^n g(i)$
            G[ lim ][ 0 ] = sub( v, 1 );
            G[ lim ][ 1 ] = mul( mul( mint( v + 2 ), mint( v - 1 ) ), inv2 );
        }
    }

    void calcH()
    {
        for ( int k = 1; k <= pcnt; ++k )
        {
            const int p  = prime[ k ];
            const ll  p2 = 1ll * p * p;
            for ( int i = 1; w[ i ] >= p2; ++i )
            {
                const ll v  = w[ i ] / p;
                int      id = idx( v );
                dec( G[ i ][ 0 ], sub( G[ id ][ 0 ], k - 1 ) );
                dec( G[ i ][ 1 ], mul( p, sub( G[ id ][ 1 ], sp1[ k - 1 ] ) ) );
            }
        }
        for ( int i = 1; i <= lim; ++i )
            H[ i ] = sub( G[ i ][ 1 ], G[ i ][ 0 ] );
    }

    // $f(p^{e})$
    inline int fpe( const int &p, const int &e )
    {
        return  p xor e ;
    }

    int F( const int &k, const ll &n )
    {
        if ( n < prime[ k ] || n <= 1 )
            return 0;

        int r1 = 0;
        for ( int i = k; i <= pcnt; ++i )
        {
            ll pi = prime[ i ];
            if ( 1ll * pi * pi > n )
                break;
            ll pc = pi, pc2 = pi * pi;
            for ( int c = 1; pc2 <= n; ++c )
            {
                inc( r1, add( mul( fpe( pi, c ), F( i + 1, n / pc ) ), fpe( pi, c + 1 ) ) );
                pc  = pc2;
                pc2 = pc2 * pi;
            }
        }

        // $H(n) - H(p_{k-1})$
        const int id = idx( n );
        int       r2 = sub( H[ id ], sub( sp1[ k - 1 ], k - 1 ) );
        if ( k == 1 )
            inc( r2, 2 );
        int ans = add( r1, r2 );
        return ans;
    }

    int solve( ll n )
    {
        gn = n;
        rt = sqrt( gn );
        seive( rt + 5 );
        initG0( gn );
        calcH();
        return add( F( 1, n ), 1 );
    }
}  // namespace min25

int main()
{
    ll n;
    scanf( "%lld", &n );
    printf( "%d\n", min25::solve( n ) );
    return 0;
}
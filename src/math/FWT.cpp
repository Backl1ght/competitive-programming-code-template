#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

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
    return x - y < 0 ? x - y + MOD : x - y;
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

namespace FWT
{
    void OR( int* a, int n )
    {
        for ( int o = 2, k = 1; o <= n; o <<= 1, k <<= 1 )
            for ( int i = 0; i < n; i += o )
                for ( int j = 0; j < k; ++j )
                    a[ i + j + k ] = add( a[ i + j + k ], a[ i + j ] );
    }

    void IOR( int* a, int n )
    {
        for ( int o = 2, k = 1; o <= n; o <<= 1, k <<= 1 )
            for ( int i = 0; i < n; i += o )
                for ( int j = 0; j < k; ++j )
                    a[ i + j + k ] = sub( a[ i + j + k ], a[ i + j ] );
    }

    void AND( int* a, int n )
    {
        for ( int o = 2, k = 1; o <= n; o <<= 1, k <<= 1 )
            for ( int i = 0; i < n; i += o )
                for ( int j = 0; j < k; ++j )
                    a[ i + j ] = add( a[ i + j ], a[ i + j + k ] );
    }

    void IAND( int* a, int n )
    {
        for ( int o = 2, k = 1; o <= n; o <<= 1, k <<= 1 )
            for ( int i = 0; i < n; i += o )
                for ( int j = 0; j < k; ++j )
                    a[ i + j ] = sub( a[ i + j ], a[ i + j + k ] );
    }

    void XOR( int* a, int n )
    {
        int x, y;
        for ( int o = 2, k = 1; o <= n; o <<= 1, k <<= 1 )
            for ( int i = 0; i < n; i += o )
                for ( int j = 0; j < k; ++j )
                {
                    x = a[ i + j ], y = a[ i + j + k ];
                    a[ i + j ]     = add( x, y );
                    a[ i + j + k ] = sub( x, y );
                }
    }

    int  inv2 = inv( 2 );
    void IXOR( int* a, int n )
    {
        int x, y;
        for ( int o = 2, k = 1; o <= n; o <<= 1, k <<= 1 )
            for ( int i = 0; i < n; i += o )
                for ( int j = 0; j < k; ++j )
                {
                    x = a[ i + j ], y = a[ i + j + k ];
                    a[ i + j ]     = mul( add( x, y ), inv2 );
                    a[ i + j + k ] = mul( sub( x, y ), inv2 );
                }
    }
}  // namespace FWT

const int N = ( 1 << 17 ) + 5;

int n;
int A[ N ], B[ N ], a[ N ], b[ N ], c[N];

int main()
{
    scanf( "%d", &n );
    n = 1 << n;

    int x;
    for ( int i = 0; i < n; ++i )
        scanf( "%d", &A[ i ] );
    for ( int i = 0; i < n; ++i )
        scanf( "%d", &B[ i ] );

    // OR
    for ( int i = 0; i < n; ++i )
        a[ i ] = A[ i ], b[ i ] = B[ i ];
    FWT::OR( a, n );
    FWT::OR( b, n );
    for ( int i = 0; i < n; ++i )
        c[ i ] = mul( a[ i ], b[ i ] );
    FWT::IOR( c, n );

    for ( int i = 0; i < n - 1; ++i )
        printf( "%d ", c[ i ] );
    printf( "%d\n", c[ n - 1 ] );

    // AND
    for ( int i = 0; i < n; ++i )
        a[ i ] = A[ i ], b[ i ] = B[ i ];
    FWT::AND( a, n );
    FWT::AND( b, n );
    for ( int i = 0; i < n; ++i )
        c[ i ] = mul( a[ i ], b[ i ] );
    FWT::IAND( c, n );
    for ( int i = 0; i < n - 1; ++i )
        printf( "%d ", c[ i ] );
    printf( "%d\n", c[ n - 1 ] );

    // XOR
    for ( int i = 0; i < n; ++i )
        a[ i ] = A[ i ], b[ i ] = B[ i ];
    FWT::XOR( a, n );
    FWT::XOR( b, n );
    for ( int i = 0; i < n; ++i )
        c[ i ] = mul( a[ i ], b[ i ] );
    FWT::IXOR( c, n );
    for ( int i = 0; i < n - 1; ++i )
        printf( "%d ", c[ i ] );
    printf( "%d\n", c[ n - 1 ] );
    return 0;
}
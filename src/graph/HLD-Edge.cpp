#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, q;

struct edge
{
    int v, w, nxt;
} e[ N << 1 ];
int  tot, head[ N ];
void init_graph( int n )
{
    tot = 0;
    fill( head + 1, head + 1 + n, 0 );
}
void add( int u, int v, int w )
{
    ++tot;
    e[ tot ]  = ( edge ){ v, w, head[ u ] };
    head[ u ] = tot;
}

int  sz[ N ], son[ N ], h[ N ], f[ N ], w[ N ];
void dfs1( int u, int fa )
{
    h[ u ]   = h[ fa ] + 1;
    f[ u ]   = fa;
    sz[ u ]  = 1;
    son[ u ] = 0;
    for ( int i = head[ u ]; i; i = e[ i ].nxt )
    {
        int v = e[ i ].v;
        if ( v == fa )
            continue;
        w[ v ] = e[ i ].w;
        dfs1( v, u );
        sz[ u ] += sz[ v ];
        if ( sz[ v ] > sz[ son[ u ] ] )
            son[ u ] = v;
    }
}
int  dfs_clock, dfn[ N ], rk[ N ], top[ N ];
void dfs2( int u, int fa, int tp )
{
    ++dfs_clock;
    dfn[ dfs_clock ] = w[ u ];
    rk[ u ]          = dfs_clock;
    top[ u ]         = tp;
    if ( son[ u ] )
        dfs2( son[ u ], u, tp );
    for ( int i = head[ u ]; i; i = e[ i ].nxt )
    {
        int v = e[ i ].v;
        if ( v == fa || v == son[ u ] )
            continue;
        dfs2( v, u, v );
    }
}

#define mid ( ( l + r ) >> 1 )
#define lc ( x << 1 )
#define rc ( x << 1 | 1 )
#define lson lc, l, mid
#define rson rc, mid + 1, r
int  sum[ N << 2 ], ma[ N << 2 ], mi[ N << 2 ], tag_inv[ N << 2 ];
void push_up( int x )
{
    sum[ x ] = sum[ lc ] + sum[ rc ];
    ma[ x ]  = max( ma[ lc ], ma[ rc ] );
    mi[ x ]  = min( mi[ lc ], mi[ rc ] );
}
void push_down( int x )
{
    if ( tag_inv[ x ] != 1 )
    {
        sum[ lc ] = -sum[ lc ];
        swap( ma[ lc ], mi[ lc ] );
        ma[ lc ]      = -ma[ lc ];
        mi[ lc ]      = -mi[ lc ];
        tag_inv[ lc ] = -tag_inv[ lc ];

        sum[ rc ] = -sum[ rc ];
        swap( ma[ rc ], mi[ rc ] );
        ma[ rc ]      = -ma[ rc ];
        mi[ rc ]      = -mi[ rc ];
        tag_inv[ rc ] = -tag_inv[ rc ];

        tag_inv[ x ] = 1;
    }
}
void build( int x, int l, int r )
{
    tag_inv[ x ] = 1;
    if ( l == r )
    {
        sum[ x ] = ma[ x ] = mi[ x ] = dfn[ l ];
        return;
    }
    build( lson );
    build( rson );
    push_up( x );
}

void update( int x, int l, int r, int p, int w )
{
    if ( l == r )
    {
        sum[ x ] = ma[ x ] = mi[ x ] = w;
        return;
    }
    push_down( x );
    if ( p <= mid )
        update( lson, p, w );
    else
        update( rson, p, w );
    push_up( x );
}

void inverse( int x, int l, int r, int L, int R )
{
    if ( l == L && r == R )
    {
        sum[ x ] = -sum[ x ];
        swap( ma[ x ], mi[ x ] );
        ma[ x ]      = -ma[ x ];
        mi[ x ]      = -mi[ x ];
        tag_inv[ x ] = -tag_inv[ x ];
        return;
    }
    push_down( x );
    if ( R <= mid )
        inverse( lson, L, R );
    else if ( L > mid )
        inverse( rson, L, R );
    else
    {
        inverse( lson, L, mid );
        inverse( rson, mid + 1, R );
    }
    push_up( x );
}

int getsum( int x, int l, int r, int L, int R )
{
    if ( l == L && r == R )
        return sum[ x ];
    push_down( x );
    if ( R <= mid )
        return getsum( lson, L, R );
    else if ( L > mid )
        return getsum( rson, L, R );
    return getsum( lson, L, mid ) + getsum( rson, mid + 1, R );
}

int getmax( int x, int l, int r, int L, int R )
{
    if ( l == L && r == R )
        return ma[ x ];
    push_down( x );
    if ( R <= mid )
        return getmax( lson, L, R );
    else if ( L > mid )
        return getmax( rson, L, R );
    return max( getmax( lson, L, mid ), getmax( rson, mid + 1, R ) );
}

int getmin( int x, int l, int r, int L, int R )
{
    if ( l == L && r == R )
        return mi[ x ];
    push_down( x );
    if ( R <= mid )
        return getmin( lson, L, R );
    else if ( L > mid )
        return getmin( rson, L, R );
    return min( getmin( lson, L, mid ), getmin( rson, mid + 1, R ) );
}

void INVERSE( int u, int v )
{
    while ( top[ u ] != top[ v ] )
    {
        if ( h[ top[ u ] ] < h[ top[ v ] ] )
            swap( u, v );
        inverse( 1, 1, n, rk[ top[ u ] ], rk[ u ] );
        u = f[ top[ u ] ];
    }
    if ( h[ u ] != h[ v ] )
    {
        if ( h[ u ] > h[ v ] )
            swap( u, v );
        inverse( 1, 1, n, rk[ son[ u ] ], rk[ v ] );
    }
}

int QSUM( int u, int v )
{
    int res = 0;
    while ( top[ u ] != top[ v ] )
    {
        if ( h[ top[ u ] ] < h[ top[ v ] ] )
            swap( u, v );
        res += getsum( 1, 1, n, rk[ top[ u ] ], rk[ u ] );
        u = f[ top[ u ] ];
    }
    if ( h[ u ] != h[ v ] )
    {
        if ( h[ u ] > h[ v ] )
            swap( u, v );
        res += getsum( 1, 1, n, rk[ son[ u ] ], rk[ v ] );
    }
    return res;
}

int QMAX( int u, int v )
{
    int res = INT_MIN;
    while ( top[ u ] != top[ v ] )
    {
        if ( h[ top[ u ] ] < h[ top[ v ] ] )
            swap( u, v );
        res = max( res, getmax( 1, 1, n, rk[ top[ u ] ], rk[ u ] ) );
        u   = f[ top[ u ] ];
    }
    if ( h[ u ] != h[ v ] )
    {
        if ( h[ u ] > h[ v ] )
            swap( u, v );
        res = max( res, getmax( 1, 1, n, rk[ son[ u ] ], rk[ v ] ) );
    }
    return res;
}

int QMIN( int u, int v )
{
    int res = INT_MAX;
    while ( top[ u ] != top[ v ] )
    {
        if ( h[ top[ u ] ] < h[ top[ v ] ] )
            swap( u, v );
        res = min( res, getmin( 1, 1, n, rk[ top[ u ] ], rk[ u ] ) );
        u   = f[ top[ u ] ];
    }
    if ( h[ u ] != h[ v ] )
    {
        if ( h[ u ] > h[ v ] )
            swap( u, v );
        res = min( res, getmin( 1, 1, n, rk[ son[ u ] ], rk[ v ] ) );
    }
    return res;
}

int  tu[ N ], tv[ N ];
void solve( int Case )
{
    /* write code here */
    /* gl & hf */
    scanf( "%d", &n );
    int u, v, w;
    for ( int i = 1; i <= n - 1; ++i )
    {
        scanf( "%d %d %d", &u, &v, &w );
        ++u, ++v;
        add( u, v, w );
        add( v, u, w );

        tu[ i ] = u;
        tv[ i ] = v;
    }

    dfs1( 1, 1 );
    dfs2( 1, 1, 1 );

    build( 1, 1, n );

    scanf( "%d", &q );
    char op[ 5 ];
    int  x, y;
    for ( int i = 1; i <= q; ++i )
    {
        scanf( "%s %d %d", op, &x, &y );
        ++x, ++y;
        if ( op[ 0 ] == 'C' )
        {
            --x, --y;
            int id = h[ tu[ x ] ] > h[ tv[ x ] ] ? tu[ x ] : tv[ x ];
            update( 1, 1, n, rk[ id ], y );
        }
        else if ( op[ 0 ] == 'N' )
        {
            INVERSE( x, y );
        }
        else if ( op[ 0 ] == 'S' )
        {
            printf( "%d\n", QSUM( x, y ) );
        }
        else if ( op[ 1 ] == 'A' )
        {
            printf( "%d\n", QMAX( x, y ) );
        }
        else if ( op[ 1 ] == 'I' )
        {
            printf( "%d\n", QMIN( x, y ) );
        }
    }
}

int main()
{
    int T = 1;
    for ( int _ = 1; _ <= T; _++ )
        solve( _ );
    return 0;
}
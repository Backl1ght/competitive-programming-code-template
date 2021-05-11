// KDTree
// 平面最近点对
template < typename T, int K = 2 >
struct KDTree
{
    using node = array< double, K >;
    int    n;
    node   p[ N ], ma[ N ], mi[ N ];
    double L[ N ], R[ N ], D[ N ], U[ N ];
    int    sd[ N ], lc[ N ], rc[ N ];

    KDTree( int _n ) : n( _n )
    {
    }

    double dist( const node& nd1, const node& nd2 )
    {
        double res = 0;
        for ( int j = 0; j < K; ++j )
        {
            res += ( nd1[ j ] - nd2[ j ] ) * ( nd1[ j ] - nd2[ j ] );
        }
        return res;
    }

    double dist( int x, int y )
    {
        return dist( p[ x ], p[ y ] );
    }

    double cost( int x, int y )
    {
        double res = 0;
        for ( int j = 0; j < K; ++j )
        {
            if ( mi[ y ][ j ] > p[ x ][ j ] )
                res += ( mi[ y ][ j ] - p[ x ][ j ] ) * ( mi[ y ][ j ] - p[ x ][ j ] );
            if ( ma[ y ][ j ] < p[ x ][ j ] )
                res += ( ma[ y ][ j ] - p[ x ][ j ] ) * ( ma[ y ][ j ] - p[ x ][ j ] );
        }
        return res;
    }

    struct cmp
    {
        int s;
        cmp( int _s ) : s( _s )
        {
        }
        bool operator()( const node& nd1, const node& nd2 ) const
        {
            return nd1[ s ] < nd2[ s ];
        }
    };

    void maintain( int x )
    {
        ma[ x ] = mi[ x ] = p[ x ];
        if ( lc[ x ] )
        {
            for ( int j = 0; j < K; ++j )
            {
                ma[ x ][ j ] = max( ma[ x ][ j ], ma[ lc[ x ] ][ j ] );
                mi[ x ][ j ] = min( mi[ x ][ j ], mi[ lc[ x ] ][ j ] );
            }
        }
        if ( rc[ x ] )
        {
            for ( int j = 0; j < K; ++j )
            {
                ma[ x ][ j ] = max( ma[ x ][ j ], ma[ rc[ x ] ][ j ] );
                mi[ x ][ j ] = min( mi[ x ][ j ], mi[ rc[ x ] ][ j ] );
            }
        }
    }

    int build( int l, int r )
    {
        if ( l >= r )
            return 0;
        int mid = ( l + r ) >> 1;

        array< double, K > avg;
        for ( int i = l; i <= r; ++i )
            for ( int j = 0; j < K; ++j )
                avg[ j ] += p[ i ][ j ];
        for ( int j = 0; j < K; ++j )
            avg[ j ] /= ( r - l + 1 );

        array< double, K > var;
        for ( int i = l; i <= r; ++i )
            for ( int j = 0; j < K; ++j )
                var[ j ] += ( p[ i ][ j ] - avg[ j ] ) * ( p[ i ][ j ] - avg[ j ] );

        sd[ mid ] = 0;
        for ( int j = 0; j < K; ++j )
            if ( var[ j ] > var[ sd[ mid ] ] )
                sd[ mid ] = j;

        nth_element( p + l, p + mid, p + r + 1, cmp( sd[ mid ] ) );

        lc[ mid ] = build( l, mid - 1 );
        rc[ mid ] = build( mid + 1, r );

        maintain( mid );

        return mid;
    }

    double min_dist;

    void query( int l, int r, int x )
    {
        if ( l > r )
            return;
        int mid = ( l + r ) >> 1;
        if ( mid != x )
            min_dist = min( min_dist, dist( x, mid ) );
        if ( l == r )
            return;

        double dl = cost( x, lc[ mid ] );
        double dr = cost( x, rc[ mid ] );

        if ( dl < min_dist && dr < min_dist )
        {
            if ( dl < dr )
            {
                query( l, mid - 1, x );
                if ( dr < min_dist )
                    query( mid + 1, r, x );
            }
            else
            {
                query( mid + 1, r, x );
                if ( dl < min_dist )
                    query( l, mid - 1, x );
            }
        }
        else
        {
            if ( dl < min_dist )
                query( l, mid - 1, x );
            if ( dr < min_dist )
                query( mid + 1, r, x );
        }
    }

    double getMindis() {
        min_dist = 2e18;
        for ( int i = 1; i <= n; ++i )
            query( 1, n, i );
        min_dist = sqrt(min_dist);
        return min_dist;
    }
};
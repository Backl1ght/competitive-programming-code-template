// Source: https://github.com/Baobaobear/MiniBigInteger/blob/main/bigint_tiny.h
// Author: https://github.com/Baobaobear
struct BigInt
{
    int                sign;
    std::vector< int > v;

    BigInt() : sign( 1 )
    {
    }
    BigInt( const std::string &s )
    {
        *this = s;
    }
    BigInt( int v )
    {
        char buf[ 21 ];
        sprintf( buf, "%d", v );
        *this = buf;
    }
    void zip( int unzip )
    {
        if ( unzip == 0 )
        {
            for ( int i = 0; i < ( int ) v.size(); i++ )
                v[ i ] = get_pos( i * 4 ) + get_pos( i * 4 + 1 ) * 10 + get_pos( i * 4 + 2 ) * 100 +
                         get_pos( i * 4 + 3 ) * 1000;
        }
        else
            for ( int i = ( v.resize( v.size() * 4 ), ( int ) v.size() - 1 ), a; i >= 0; i-- )
                a      = ( i % 4 >= 2 ) ? v[ i / 4 ] / 100 : v[ i / 4 ] % 100,
                v[ i ] = ( i & 1 ) ? a / 10 : a % 10;
        setsign( 1, 1 );
    }
    int get_pos( unsigned pos ) const
    {
        return pos >= v.size() ? 0 : v[ pos ];
    }
    BigInt &setsign( int newsign, int rev )
    {
        for ( int i = ( int ) v.size() - 1; i > 0 && v[ i ] == 0; i-- )
            v.erase( v.begin() + i );
        sign = ( v.size() == 0 || ( v.size() == 1 && v[ 0 ] == 0 ) )
                   ? 1
                   : ( rev ? newsign * sign : newsign );
        return *this;
    }
    std::string to_str() const
    {
        BigInt      b = *this;
        std::string s;
        for ( int i = ( b.zip( 1 ), 0 ); i < ( int ) b.v.size(); ++i )
            s += char( *( b.v.rbegin() + i ) + '0' );
        return ( sign < 0 ? "-" : "" ) + ( s.empty() ? std::string( "0" ) : s );
    }
    bool absless( const BigInt &b ) const
    {
        if ( v.size() != b.v.size() )
            return v.size() < b.v.size();
        for ( int i = ( int ) v.size() - 1; i >= 0; i-- )
            if ( v[ i ] != b.v[ i ] )
                return v[ i ] < b.v[ i ];
        return false;
    }
    BigInt operator-() const
    {
        BigInt c = *this;
        c.sign   = ( v.size() > 1 || v[ 0 ] ) ? -c.sign : 1;
        return c;
    }
    BigInt &operator=( const std::string &s )
    {
        if ( s[ 0 ] == '-' )
            *this = s.substr( 1 );
        else
        {
            for ( int i = ( v.clear(), 0 ); i < ( int ) s.size(); ++i )
                v.push_back( *( s.rbegin() + i ) - '0' );
            zip( 0 );
        }
        return setsign( s[ 0 ] == '-' ? -1 : 1, sign = 1 );
    }
    bool operator<( const BigInt &b ) const
    {
        return sign != b.sign ? sign < b.sign : ( sign == 1 ? absless( b ) : !absless( b ) );
    }
    bool operator==( const BigInt &b ) const
    {
        return v == b.v && sign == b.sign;
    }
    BigInt &operator+=( const BigInt &b )
    {
        if ( sign != b.sign )
            return *this = ( *this ) - -b;
        v.resize( std::max( v.size(), b.v.size() ) + 1 );
        for ( int i = 0, carry = 0; i < ( int ) b.v.size() || carry; i++ )
        {
            carry += v[ i ] + b.get_pos( i );
            v[ i ] = carry % 10000, carry /= 10000;
        }
        return setsign( sign, 0 );
    }
    BigInt operator+( const BigInt &b ) const
    {
        BigInt c = *this;
        return c += b;
    }
    void add_mul( const BigInt &b, int mul )
    {
        v.resize( std::max( v.size(), b.v.size() ) + 2 );
        for ( int i = 0, carry = 0; i < ( int ) b.v.size() || carry; i++ )
        {
            carry += v[ i ] + b.get_pos( i ) * mul;
            v[ i ] = carry % 10000, carry /= 10000;
        }
    }
    BigInt operator-( const BigInt &b ) const
    {
        if ( sign != b.sign )
            return ( *this ) + -b;
        if ( absless( b ) )
            return -( b - *this );
        BigInt c;
        for ( int i = 0, borrow = 0; i < ( int ) v.size(); i++ )
        {
            borrow += v[ i ] - b.get_pos( i );
            c.v.push_back( borrow );
            c.v.back() -= 10000 * ( borrow >>= 31 );
        }
        return c.setsign( sign, 0 );
    }
    BigInt operator*( const BigInt &b ) const
    {
        if ( b < *this )
            return b * *this;
        BigInt c, d = b;
        for ( int i = 0; i < ( int ) v.size(); i++, d.v.insert( d.v.begin(), 0 ) )
            c.add_mul( d, v[ i ] );
        return c.setsign( sign * b.sign, 0 );
    }
    BigInt operator/( const BigInt &b ) const
    {
        BigInt c, d;
        d.v.resize( v.size() );
        double db = 1.0 / ( b.v.back() + ( b.get_pos( ( unsigned ) b.v.size() - 2 ) / 1e4 ) +
                            ( b.get_pos( ( unsigned ) b.v.size() - 3 ) + 1 ) / 1e8 );
        for ( int i = ( int ) v.size() - 1; i >= 0; i-- )
        {
            c.v.insert( c.v.begin(), v[ i ] );
            int m = ( int ) ( ( c.get_pos( ( int ) b.v.size() ) * 10000 +
                                c.get_pos( ( int ) b.v.size() - 1 ) ) *
                              db );
            c     = c - b * m, d.v[ i ] += m;
            while ( !( c < b ) )
                c = c - b, d.v[ i ] += 1;
        }
        return d.setsign( sign * b.sign, 0 );
    }
    BigInt operator%( const BigInt &b ) const
    {
        return *this - *this / b * b;
    }
    bool operator>( const BigInt &b ) const
    {
        return b < *this;
    }
    bool operator<=( const BigInt &b ) const
    {
        return !( b < *this );
    }
    bool operator>=( const BigInt &b ) const
    {
        return !( *this < b );
    }
    bool operator!=( const BigInt &b ) const
    {
        return !( *this == b );
    }
};
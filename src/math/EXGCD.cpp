namespace Backlight {

// get x_0, y_0 for ax + by = GCD(a, b)
// x = x_0 + bt
// y = y_0 - at
//    for all interger t
#define EXGCD
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = 1; y = 0; 
        return a; 
    }
    ll d = exgcd(b, a % b, x, y);
    ll z = x; x = y; y = z - y * (a / b);
    return d;
}

}
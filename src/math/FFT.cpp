namespace Backlight {

namespace FFT {
    const double PI = acos(-1.0);
    struct Complex {
        double r, i;
        Complex() : r(0), i(0) {}
        Complex(double _r, double _i) : r(_r), i(_i) {}
        inline Complex operator-(const Complex &c) const { return Complex(r - c.r, i - c.i); }
        inline Complex operator+(const Complex &c) const { return Complex(r + c.r, i + c.i); }
        inline Complex operator*(const Complex &c) const { return Complex(r * c.r - i * c.i, r * c.i + i * c.r); }
    };
    ostream& operator << (ostream& os, Complex& c)  { return os << "(" << c.r << ", " << c.i << ")"; }

    int N;
    void init(vector<Complex>& a, vector<Complex>& b) {
        int l1 = a.size(), l2 = b.size(), ma = max(l1, l2) << 1;
        N = 1; while(N < ma) N <<= 1;
        a.resize(N); b.resize(N);
    }

    void prework(vector<Complex>& a) {
        for(int i = 1, j = N / 2, k; i < N - 1; ++i) {
            if(i < j) swap(a[i],a[j]);
            k = N / 2;
            while(j >= k) j -= k, k /= 2;
            if(j < k) j += k;
        }
    }

    void work(vector<Complex>& a, int flag) {
        Complex x, y;
        prework(a);
        for(int i = 2; i <= N; i <<= 1){
            Complex w, wk(cos(-2 * PI * flag / i), sin(-2 * PI * flag / i));
            for(int j = 0; j < N; j += i) {
                w = Complex(1,0);
                for(int k = j; k < j + i / 2; k++) {
                    x = a[k];
                    y = w * a[k + i / 2];
                    a[k] = x + y;
                    a[k + i / 2] = x - y;
                    w = w * wk;
                }
            }
        }
        if(flag == -1) 
            for(int i = 0 ; i < N; i++) a[i].r /= N;
    }

    inline void FFT(vector<Complex>& a) { work(a, 1); }
    inline void IFT(vector<Complex>& a) { work(a, -1); }

    vector<Complex> convolution(vector<Complex> a, vector<Complex> b) {
        init(a, b);
        FFT(a); FFT(b);
        for (int i = 0; i < N; ++i) a[i] = a[i] * b[i];
        IFT(a);
        return a;
    }
} // namespace FFT

} // namespace Backlight
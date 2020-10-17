template<int MD>
struct Modular {
    ll value, md;

    Modular() : value(0), md(MD) {}

    Modular(ll value) : value((value % MD + MD) % MD), md(MD) {}

    Modular& norm() { value = (value % MD + MD) % MD; return *this; }

    Modular operator - () const { return Modular(0) - *this; }

    Modular operator ~ () const { return Modular(1) / *this; }

    Modular operator ^ (const Modular& a) const { return pow(*this, a); }

    Modular& operator += (const Modular& a) { value += a.value; if(value >= MD) value -= MD; return *this; }

    Modular& operator -= (const Modular& a) { value += MD - a.value; if(value >= MD) value -= MD; return *this; }
  
    Modular& operator *= (const Modular& a) { (value *= a.value) %= MD; return *this; }
  
    Modular& operator /= (const Modular& a) { (value *= a.pow(MD - 2).value) %= MD; return *this; }
    
    Modular operator + (const Modular& a) const { return Modular(this->value) += a; }
    
    Modular operator - (const Modular& a) const { return Modular(this->value) -= a; }
    
    Modular operator * (const Modular& a) const { return Modular(this->value) *= a; }
    
    Modular operator / (const Modular& a) const { return Modular(this->value) /= a; }

    bool operator < (const Modular& a) const { return value < a.value; }

    bool operator == (const Modular& a) const { return value == a.value; }

    bool operator != (const Modular& a) const { return value != a.value; }
    
    Modular pow(ll b) const {
        Modular a = value, res = 1;
        while(b) {
            if (b & 1) res *= a;
            a *= a;
            b >>= 1;
        }
        return res;
    }

    Modular inv() const { return this->pow(MD - 2); }
};
using Mint = Modular<MOD>;
istream& operator >> (istream& is, Mint& a) { return is >> a.value; }
ostream& operator << (ostream& os, const Mint& a) { return os << a.value; }
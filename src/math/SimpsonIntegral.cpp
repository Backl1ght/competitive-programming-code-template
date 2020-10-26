namespace SimpsonIntegral {
    // calculate \int_l^r f(x) dx
    
    double f(double x) {
        return (c * x + d) / (a * x + b);
    }

    double simpson(double l, double r) {
        double mid = (l + r) / 2;
        return (r - l) * (f(l) + 4 * f(mid) + f(r)) / 6;
    }

    double integral(double l, double r, double eps, double ans) {
        double mid = (l + r) / 2;
        double fl = simpson(l, mid), fr = simpson(mid, r);
        if (abs(fl + fr - ans) <= 15 * eps)
            return fl + fr + (fl + fr - ans) / 15;
        return integral(l, mid, eps / 2, fl) + integral(mid, r, eps / 2, fr);
    }

    double integral(double l, double r, double eps = 1e-8) {
        return integral(l, r, eps, simpson(l, r));
    }
}
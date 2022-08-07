// Problem: P4718 【模板】Pollard-Rho算法
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P4718
// Memory Limit: 125 MB
// Time Limit: 2000 ms
//
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>

#define CPPIO std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
#define freep(p) p ? delete p, p = nullptr, void(1) : void(0)

#ifdef BACKLIGHT
#include "debug.h"
#else
#define logd(...) ;
#endif

using i64 = int64_t;
using u64 = uint64_t;
using i128 = __int128_t;

void solve_case(int Case);

int main(int argc, char* argv[]) {
  CPPIO;
  int T = 1;
  std::cin >> T;
  for (int t = 1; t <= T; ++t) {
    solve_case(t);
  }
  return 0;
}

namespace PollardRho {

/**
 * @TODO: __int128_t is ugly, try optimize it.
 */

static std::mt19937_64 rng_(std::chrono::steady_clock::now().time_since_epoch().count());

inline int64_t Rand(int64_t l, int64_t r) {
  return l + rng_() % (r - l + 1);
}

inline int64_t Add(int64_t a, int64_t b, int64_t mod) {
  return ((__int128_t)a + b) % mod;
}

inline int64_t Substract(int64_t a, int64_t b, int64_t mod) {
  return (((__int128_t)a - b) % mod + mod) % mod;
}

inline int64_t Multiply(int64_t a, int64_t b, int64_t mod) {
  return (__int128_t)a * b % mod;
}

inline int64_t Power(int64_t a, int64_t b, int64_t mod) {
  int64_t r = 1;
  while (b) {
    if (b & 1)
      r = Multiply(r, a, mod);
    a = Multiply(a, a, mod);
    b >>= 1;
  }
  return r;
}

// Time Complexity: $O(k \log^{3} n)$
bool MillerRabinTest(int64_t n) {
  // Strong enough for $n < 2^64$, see https://oeis.org/A014233.
  constexpr static int kTestRounds = 12;
  constexpr static int kTestBase[kTestRounds] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

  if (n <= kTestBase[kTestRounds - 1]) {
    return *std::lower_bound(kTestBase, kTestBase + kTestRounds, n) == n;
  }

  int64_t d = n - 1, r = 0;
  while (d % 2 == 0) {
    d >>= 1;
    r = r + 1;
  }

  for (int round = 0; round < kTestRounds; ++round) {
    int64_t a = kTestBase[round];

    // Fermet primality test.
    int64_t x = Power(a, d, n);
    if (x == 1 || x == n - 1)
      continue;

    // Witness primality test.
    for (int i = 0; i < r - 1; ++i) {
      x = Multiply(x, x, n);
      if (x == n - 1)
        break;
    }
    if (x != n - 1)
      return false;
  }

  return true;
}

int64_t Rho(int64_t n) {
  // Can not factor 4 because the faster step gap is 2.
  if (n == 4)
    return 2;

  const static int kMaxStepSize = 1 << 7;

  int64_t c;
  std::function<int64_t(int64_t)> f = [&n, &c](int64_t x) { return Add(Multiply(x, x, n), c, n); };

  // Since n is not a prime, there must be a non-trivial factor of n.
  for (;;) {
    /**
     * Brent's cycle finding method, and replace k gcd steps with k - 1 multiplications
     * modulo n and a single gcd.
     * reference: https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm#Variants.
     */
    c = Rand(3, n - 1);

    int64_t x = Rand(0, n - 1);
    int64_t t = f(x), r = f(t);
    int64_t goal = 1, curr = 1;
    int64_t d1, d2 = 1;
    while (t != r) {
      d1 = Multiply(d2, std::abs(r - t), n);
      if (d1 == 0) {
        break;
      } else {
        d2 = d1;
      }

      if (curr % kMaxStepSize == 0) {
        int64_t d = std::gcd(d2, n);
        if (d != 1 && d != n)
          return d;
      }

      if (curr == goal) {
        int64_t d = std::gcd(d2, n);
        if (d != 1 && d != n)
          return d;

        t = r;
        goal = goal * 2;
        curr = 0;
      }

      r = f(r);
      ++curr;
    }
  }
}

std::vector<int64_t> Factor(int64_t n) {
  std::vector<int64_t> factors;
  std::function<void(int64_t)> factor = [&](int64_t n) {
    if (n < 2)
      return;

    if (MillerRabinTest(n)) {
      factors.push_back(n);
    } else {
      int64_t x = Rho(n);
      while (n % x == 0)
        n /= x;
      factor(x);
      factor(n);
    }
  };
  factor(n);
  std::sort(factors.begin(), factors.end());
  return factors;
}

};  // namespace PollardRho

void solve_case(int Case) {
  int64_t n;
  std::cin >> n;
  auto f = PollardRho::Factor(n);

  if (f.size() == 1 and n == f.back())
    std::cout << "Prime\n";
  else
    std::cout << f.back() << "\n";
}
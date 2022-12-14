#include <iostream>
#include <algorithm>
#include <math.h>
#include <array>
/*

        * x^n + y^n (n & 1)
        K = (n + 1) / 2
        E = (n - K)

        x^n + y^n = (x^K + y^K)(x^E + y^E) - (xy)^E(x + y)

        * x^n + y^n !(n & 1)
        K = n / 2
        x^n + y^n = (x^K + y^K)^2 - 2(xy)^K
*/

struct Fraction {
        int64_t numerator = 0;
        int64_t denominator = 1;
        Fraction() = default;
        Fraction(const int64_t& a) : numerator(a) {}
        Fraction(const int64_t& a, const int64_t& b) :
                numerator(a),
                denominator(b)
        {}

        Fraction& simplify() {
                int64_t GCD = std::__gcd(numerator, denominator);
                numerator /= GCD;
                denominator /= GCD;
                return *this;
        }

        Fraction simplify(Fraction&& fraction) {
                int64_t GCD = std::__gcd(fraction.numerator, fraction.denominator);
                fraction.numerator /= GCD;
                fraction.denominator /= GCD;
                return fraction;
        }

        Fraction operator-(const Fraction& other) {
                return simplify({ (numerator * other.denominator) - (denominator * other.numerator), denominator * other.denominator });
        }

        Fraction operator*(const Fraction& other) {
                return simplify({ numerator * other.numerator, denominator * other.denominator });
        }

        Fraction operator/(const int& scalar) {
                return simplify({ numerator, denominator * scalar });
        }

        friend Fraction pow(Fraction fraction, const int& n) {
                fraction.numerator = std::pow(fraction.numerator, n);
                fraction.denominator = std::pow(fraction.denominator, n);
                return fraction.simplify();
        }

        friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {
                if (fraction.denominator != 1) os << fraction.numerator << '/' << fraction.denominator;
                else os << fraction.numerator;
                return os;
        }
};

int main() {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(0);
        std::cout.tie(0);

        Fraction a, b;
        unsigned m;
        std::array<Fraction, 100> dp;
        std::cin >> a.numerator >> b.numerator >> m;

        dp[0] = 2; // x^0 + y^0 = 1 + 1 = 2
        dp[1] = a; // x + y
        dp[2] = b; // x^2 + y^2

        auto xy = (b - pow(a, 2)) / (-2);

        for (unsigned n = 3; n <= m; ++n) {
                if (n & 1) {
                        unsigned K = (n + 1) / 2;
                        unsigned E = n - K;

                        dp[n] = (dp[K] * dp[E]) - (pow(xy, E) * a);
                } else {
                        unsigned K = n / 2;
                        dp[n] = pow(dp[K], 2) - (Fraction(2) * pow(xy, K));
                }
        }

        #define DEBUG 1
        #ifdef DEBUG
                std::cout << "xy = " << xy << '\n';
                for (unsigned n = 1; n <= m; ++n) {
                        std::cout << "x^" << n << " + y^" << n << " = " << dp[n] << '\n';
                }
        #else
                std::cout << "x + y = " << a << '\n';
                std::cout << "x^2 + y^2 = " << b << '\n';
                std::cout << "x^" << m << " + y^" << m << " = " << dp[m] << '\n';
        #endif

        return 0;
}

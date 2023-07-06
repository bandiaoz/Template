#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T, T MOD>
struct ModInt {
    using prod_type = std::conditional_t<std::is_same_v<T, int>, long long, __int128>;
    T val;
    ModInt(const prod_type v = 0) : val(v % MOD) { if (val < 0) val += MOD; };
    ModInt operator+() const { return ModInt(val); }
    ModInt operator-() const { return ModInt(MOD - val); }
    ModInt inv() const {
        auto a = val, m = MOD, u = 0, v = 1;
        while (a != 0) {
            auto t = m / a;
            m -= t * a;
            std::swap(a, m);
            u -= t * v;
            std::swap(u, v);
        }
        assert(m == 1);
        return u;
    }
    ModInt pow(prod_type n) const {
        auto x = ModInt(1);
        auto b = *this;
        while (n > 0) {
            if (n & 1)
                x *= b;
            n >>= 1;
            b *= b;
        }
        return x;
    }
    friend ModInt operator+(ModInt lhs, const ModInt &rhs) { return lhs += rhs; }
    friend ModInt operator-(ModInt lhs, const ModInt &rhs) { return lhs -= rhs; }
    friend ModInt operator*(ModInt lhs, const ModInt &rhs) { return lhs *= rhs; }
    friend ModInt operator/(ModInt lhs, const ModInt &rhs) { return lhs /= rhs; }
    ModInt &operator+=(const ModInt &x) {
        if ((val += x.val) >= MOD)
            val -= MOD;
        return *this;
    }
    ModInt &operator-=(const ModInt &x) {
        if ((val -= x.val) < 0)
            val += MOD;
        return *this;
    }
    ModInt &operator*=(const ModInt &x) {
        val = prod_type(val) * x.val % MOD;
        return *this;
    }
    ModInt &operator/=(const ModInt &x) { return *this *= x.inv(); }
    bool operator==(const ModInt &b) const { return val == b.val; }
    bool operator!=(const ModInt &b) const { return val != b.val; }
    friend std::istream &operator>>(std::istream &is, ModInt &x) noexcept {
        return is >> x.val;
    }
    friend std::ostream &operator<<(std::ostream &os, const ModInt &x) noexcept {
        return os << x.val;
    }
};
using Z = ModInt<int, 998244353>;

/**
 * @brief Lagrange Interpolation
 * @param p: polynomial determined by n points, \forall p[i] = (x[i], y[i]), satisfy f(x[i]) = y[i]
 * @param k: the x value of the interpolation point
 * @return the value of poly in x = k, f(k)
 * @note f(k) = \sum_{i = 0}^{n} y_{i} \prod_{i \neq j} \frac{k - x[j]}{x[i] - x[j]}
*/
template<typename T>
Z lagrange(const std::vector<std::pair<T, T>> &p, Z k) {
    int n = p.size();
    Z ans = 0;
    for (int i = 0; i < n; ++i) {
        Z s1 = p[i].second, s2 = 1;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                s1 *= k - p[j].first;
                s2 *= p[i].first - p[j].first;
            }
        }
        ans += s1 * s2.inv();
    }
    return ans;
}
// test problem: https://www.luogu.com.cn/problem/P4781
#include <bits/stdc++.h>

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
using Z = ModInt<int, 1'000'000'007>;

struct Comb {
    int n;
    std::vector<Z> _fac, _invfac, _inv;
    
    Comb() : n{0}, _fac{1}, _invfac{1}, _inv{0} {}
    Comb(int n) : Comb() {
        init(n);
    }
    
    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _invfac.resize(m + 1);
        _inv.resize(m + 1);
        
        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i;
        }
        _invfac[m] = _fac[m].inv();
        for (int i = m; i > n; i--) {
            _invfac[i - 1] = _invfac[i] * i;
            _inv[i] = _invfac[i] * _fac[i - 1];
        }
        n = m;
    }
    
    Z fac(int m) {
        if (m > n) init(2 * m);
        return _fac[m];
    }
    Z invfac(int m) {
        if (m > n) init(2 * m);
        return _invfac[m];
    }
    Z inv(int m) {
        if (m > n) init(2 * m);
        return _inv[m];
    }
    Z binom(int n, int m) {
        if (n < m || m < 0) return 0;
        return fac(n) * invfac(m) * invfac(n - m);
    }
} comb;
#pragma once

#include <cassert>
#include <functional>
#include <iostream>

template <typename T>
constexpr static T power(T a, int64_t b) {
    assert(b >= 0);
    T res;
    if constexpr (std::is_arithmetic_v<T>) {
        res = 1;
    } else {
        res = a.identity();
    }
    while (b) {
        if (b & 1) {
            res = res * a;
        }
        b >>= 1;
        a = a * a;
    }
    return res;
}

template <typename MOD>
struct ModInt {
    using T = typename std::decay<decltype(MOD::value)>::type;
    using prod_type = std::conditional_t<std::numeric_limits<T>::digits <= 32, uint64_t, __uint128_t>;
    T val;
    constexpr static T mod() { return MOD::value; }
    constexpr ModInt(const int64_t v = 0) : val(v % mod()) { if (val < 0) val += mod(); }
    constexpr ModInt operator+() const { return ModInt(val); }
    constexpr ModInt operator-() const { return ModInt(mod() - val); }
    constexpr ModInt inv() const { return power(*this, mod() - 2); }
    constexpr friend ModInt operator+(ModInt lhs, const ModInt rhs) { return lhs += rhs; }
    constexpr friend ModInt operator-(ModInt lhs, const ModInt rhs) { return lhs -= rhs; }
    constexpr friend ModInt operator*(ModInt lhs, const ModInt rhs) { return lhs *= rhs; }
    constexpr friend ModInt operator/(ModInt lhs, const ModInt rhs) { return lhs /= rhs; }
    constexpr ModInt &operator+=(const ModInt x) {
        if ((val += x.val) >= mod()) val -= mod();
        return *this;
    }
    constexpr ModInt &operator-=(const ModInt x) {
        if ((val -= x.val) < 0) val += mod();
        return *this;
    }
    constexpr ModInt &operator*=(const ModInt x) {
        val = prod_type(val) * x.val % mod();
        return *this;
    }
    constexpr ModInt &operator/=(const ModInt x) { return *this *= x.inv(); }
    bool operator==(const ModInt b) const { return val == b.val; }
    bool operator!=(const ModInt b) const { return val != b.val; }
    friend std::istream &operator>>(std::istream &is, ModInt &x) noexcept { return is >> x.val; }
    friend std::ostream &operator<<(std::ostream &os, const ModInt x) noexcept { return os << x.val; }
    constexpr static ModInt identity() { return 1; }
    constexpr ModInt pow(int64_t p) { return power(*this, p); }
};
struct MOD { static int value; };
int MOD::value = 1'000'000'007;
int MOD::value = 998'244'353;
using Z = ModInt<MOD>;
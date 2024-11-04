#ifndef __OY_DYNAMICMODINT32__
#define __OY_DYNAMICMODINT32__

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>

/**
 * @brief 动态 32 位模数取余
 * @example using mint = OY::DynamicModInt32<0>; 
 *          mint::set_mod(10000019, true);
 */
namespace OY {
    template <size_t Id>
    struct DynamicModInt32 {
        using mint = DynamicModInt32<Id>;
        using mod_type = uint32_t;
        mod_type m_val;
        static mod_type s_mod;
        static uint64_t s_inv;
        static bool s_is_prime;
        static mod_type _mod(uint64_t val) {
            mod_type res = val - uint64_t((__uint128_t(val) * s_inv) >> 64) * mod();
            if (res >= mod()) res += mod();
            return res;
        }
        static mod_type _mul(mod_type a, mod_type b) { return _mod(uint64_t(a) * b); }
        DynamicModInt32() : m_val{} {}
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        DynamicModInt32(Tp val) : m_val{} {
            auto x = val % int32_t(mod());
            if (x < 0) x += mod();
            m_val = x;
        }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        DynamicModInt32(Tp val) : m_val(_mod(val)) {}
        static mint raw(mod_type val) { mint res; res.m_val = val; return res; }
        static void set_mod(mod_type P, bool is_prime = false) {
            assert(P > 1 && P < mod_type(1) << 31);
            s_mod = P, s_inv = uint64_t(-1) / P + 1, s_is_prime = is_prime;
        }
        static mod_type mod() { return s_mod; }
        mod_type val() const { return m_val; }
        mint pow(uint64_t n) const {
            mod_type res = 1, b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return raw(res);
        }
        mint inv() const { return s_is_prime ? inv_Fermat() : inv_exgcd(); }
        mint inv_exgcd() const {
            mod_type x = mod(), y = m_val, m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return raw(m0);
        }
        mint inv_Fermat() const { return pow(mod() - 2); }
        mint &operator++() { if (++m_val == mod()) m_val = 0; return *this; }
        mint &operator--() { if (!m_val) m_val = mod(); m_val--; return *this; }
        mint operator++(int) { mint old(*this); ++*this; return old; }
        mint operator--(int) { mint old(*this); --*this; return old; }
        mint &operator+=(const mint &rhs) { m_val += rhs.m_val; if (m_val >= mod()) m_val -= mod(); return *this; }
        mint &operator-=(const mint &rhs) { m_val += mod() - rhs.m_val; if (m_val >= mod()) m_val -= mod(); return *this; }
        mint &operator*=(const mint &rhs) { m_val = _mul(m_val, rhs.m_val); return *this; }
        mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        mint operator+() const { return *this; }
        mint operator-() const { return raw(m_val ? mod() - m_val : 0); }
        bool operator==(const mint &rhs) const { return m_val == rhs.m_val; }
        bool operator!=(const mint &rhs) const { return m_val != rhs.m_val; }
        bool operator<(const mint &rhs) const { return m_val < rhs.m_val; }
        bool operator>(const mint &rhs) const { return m_val > rhs.m_val; }
        bool operator<=(const mint &rhs) const { return m_val <= rhs.m_val; }
        bool operator>=(const mint &rhs) const { return m_val <= rhs.m_val; }
        template <typename Tp>
        explicit operator Tp() const { return Tp(m_val); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
        constexpr static mint identity() { return 1; }
    };
    template <size_t Id>
    uint32_t DynamicModInt32<Id>::s_mod;
    template <size_t Id>
    uint64_t DynamicModInt32<Id>::s_inv;
    template <size_t Id>
    bool DynamicModInt32<Id>::s_is_prime;
    template <typename Istream, size_t Id>
    Istream &operator>>(Istream &is, DynamicModInt32<Id> &x) { return is >> x.m_val; }
    template <typename Ostream, size_t Id>
    Ostream &operator<<(Ostream &os, const DynamicModInt32<Id> &x) { return os << x.m_val; }
}

#endif
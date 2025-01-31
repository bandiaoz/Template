#ifndef __OY_STATICMODINT32__
#define __OY_STATICMODINT32__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>

/**
 * @brief 静态 32 位模数取余
 * @example using Z = OY::mint1000000007;
 */
namespace OY {
    template <uint32_t P, bool IsPrime, typename = typename std::enable_if<(P > 1 && P < uint32_t(1) << 31)>::type>
    struct StaticModInt32 {
        using mint = StaticModInt32<P, IsPrime>;
        using mod_type = uint32_t;
        mod_type m_val;
        static constexpr mod_type _reduce_norm(int32_t x) { return x < 0 ? x + mod() : x; }
        static constexpr mod_type _mul(mod_type a, mod_type b) { return uint64_t(a) * b % mod(); }
        constexpr StaticModInt32() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        constexpr StaticModInt32(Tp val) : m_val(_reduce_norm(val % int32_t(mod()))) {}
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        constexpr StaticModInt32(Tp val) : m_val(val % mod()) {}
        static constexpr mint raw(mod_type val) { mint res{}; res.m_val = val; return res; }
        static constexpr mod_type mod() { return P; }
        constexpr mod_type val() const { return m_val; }
        constexpr mint pow(uint64_t n) const {
            mod_type res = 1, b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return raw(res);
        }
        constexpr mint inv() const {
            if constexpr (IsPrime) return inv_Fermat();
            else return inv_exgcd();
        }
        constexpr mint inv_exgcd() const {
            mod_type x = mod(), y = m_val, m0 = 0, m1 = 1;
            while (y) {
                mod_type z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return raw(m0);
        }
        constexpr mint inv_Fermat() const { return pow(mod() - 2); }
        constexpr mint &operator++() { if (++m_val == mod()) m_val = 0; return *this; }
        constexpr mint &operator--() { if (!m_val) m_val = mod(); m_val--; return *this; }
        constexpr mint operator++(int) { mint old(*this); ++*this; return old; }
        constexpr mint operator--(int) { mint old(*this); --*this; return old; }
        constexpr mint &operator+=(const mint &rhs) { m_val += rhs.m_val; if (m_val >= mod()) m_val -= mod(); return *this; }
        constexpr mint &operator-=(const mint &rhs) { m_val += mod() - rhs.m_val; if (m_val >= mod()) m_val -= mod(); return *this; }
        constexpr mint &operator*=(const mint &rhs) { m_val = _mul(m_val, rhs.m_val); return *this; }
        constexpr mint &operator/=(const mint &rhs) { return *this *= rhs.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return raw(m_val ? mod() - m_val : 0); }
        constexpr bool operator==(const mint &rhs) const { return m_val == rhs.m_val; }
        constexpr bool operator!=(const mint &rhs) const { return m_val != rhs.m_val; }
        constexpr bool operator<(const mint &rhs) const { return m_val < rhs.m_val; }
        constexpr bool operator>(const mint &rhs) const { return m_val > rhs.m_val; }
        constexpr bool operator<=(const mint &rhs) const { return m_val <= rhs.m_val; }
        constexpr bool operator>=(const mint &rhs) const { return m_val >= rhs.m_val; }
        template <typename Tp>
        constexpr explicit operator Tp() const { return Tp(m_val); }
        friend constexpr mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend constexpr mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend constexpr mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend constexpr mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
        constexpr static mint identity() { return 1; }
    };
    template <typename Istream, uint32_t P, bool IsPrime>
    Istream &operator>>(Istream &is, StaticModInt32<P, IsPrime> &x) { return is >> x.m_val; }
    template <typename Ostream, uint32_t P, bool IsPrime>
    Ostream &operator<<(Ostream &os, const StaticModInt32<P, IsPrime> &x) { return os << x.val(); }
    using mint998244353 = StaticModInt32<998244353, true>;
    using mint1000000007 = StaticModInt32<1'000'000'007, true>;
}

#endif
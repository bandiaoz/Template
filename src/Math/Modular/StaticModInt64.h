#ifndef __OY_STATICMODINT64__
#define __OY_STATICMODINT64__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>

/**
 * @brief 静态 64 位模数取余
 * @example using Z = OY::mint4611686018427387847;
 */
namespace OY {
    template <uint64_t P, bool IsPrime, typename = typename std::enable_if<(P > 1 && P < uint64_t(1) << 63)>::type>
    struct StaticModInt64 {
        using mint = StaticModInt64<P, IsPrime>;
        using mod_type = uint64_t;
        mod_type m_val;
        static mod_type _mul(mod_type a, mod_type b) {
            return __int128(a) * b % mod();
        }
        StaticModInt64() = default;
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
        StaticModInt64(Tp val) : m_val{} { auto x = val % int64_t(mod()); if (x < 0) x += mod(); m_val = x; }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
        StaticModInt64(Tp val) : m_val(val % mod()) {}
        static mint raw(mod_type val) { mint res; res.m_val = val; return res; }
        static constexpr mod_type mod() { return P; }
        mod_type val() const { return m_val; }
        mint pow(uint64_t n) const {
            mod_type res = 1, b = m_val;
            while (n) {
                if (n & 1) res = _mul(res, b);
                b = _mul(b, b), n >>= 1;
            }
            return raw(res);
        }
        mint inv() const {
            if (IsPrime) return inv_Fermat();
            else return inv_exgcd();
        }
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
        bool operator>=(const mint &rhs) const { return m_val >= rhs.m_val; }
        template <typename Tp>
        explicit operator Tp() const { return Tp(m_val); }
        friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
        constexpr static mint identity() { return 1; }
    };
    template <typename Istream, uint64_t P, bool IsPrime>
    Istream &operator>>(Istream &is, StaticModInt64<P, IsPrime> &x) { return is >> x.m_val; }
    template <typename Ostream, uint64_t P, bool IsPrime>
    Ostream &operator<<(Ostream &os, const StaticModInt64<P, IsPrime> &x) { return os << x.val(); }
    using mint4611686018427387847 = StaticModInt64<4611686018427387847, true>;
    using mint9223372036854775783 = StaticModInt64<9223372036854775783, true>;
    using mint1000000000000000003 = StaticModInt64<1'000'000'000'000'000'003, true>;
}

#endif
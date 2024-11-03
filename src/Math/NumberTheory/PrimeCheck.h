#ifndef __OY_PRIMECHECK0__
#define __OY_PRIMECHECK0__

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <numeric>

/**
 * @brief 素数检测
 */
namespace OY {
    bool is_prime32(uint32_t n) {
        if (n < 64) return 0x28208a20a08a28ac >> n & 1;
        if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;
        struct Info {
            uint32_t m_mod, m_mod2, m_pinv, m_ninv;
            uint32_t m_one;
            uint64_t m_inv;
            void set_mod(uint32_t P) {
                m_mod = m_pinv = P, m_mod2 = P * 2, m_ninv = -uint64_t(P) % P, m_inv = uint64_t(-1) / P + 1;
                for (size_t i = 0; i != 4; i++) m_pinv *= uint32_t(2) - mod() * m_pinv;
                m_pinv = -m_pinv, m_one = strict_reduce(raw_init(1));
            }
            uint32_t mod(uint64_t val) const {
                uint32_t res = val - uint64_t((__uint128_t(val) * m_inv) >> 64) * mod();
                if (res >= mod()) res += mod();
                return res;
            }
            uint32_t mod() const { return m_mod; }
            uint32_t reduce(uint64_t val) const { return (val + uint64_t(uint32_t(val) * m_pinv) * mod()) >> 32; }
            uint32_t strict_reduce(uint32_t val) const { return val >= mod() ? val - mod() : val; }
            uint32_t mul(uint32_t a, uint32_t b) const { return reduce(uint64_t(a) * b); }
            uint32_t pow(uint32_t a, uint32_t n) const {
                uint32_t res = one(), b = a;
                while (n) {
                    if (n & 1) res = mul(res, b);
                    b = mul(b, b), n >>= 1;
                }
                return res;
            }
            uint32_t one() const { return m_one; }
            uint32_t raw_init(uint32_t val) const { return mul(val, m_ninv); }
        };
        Info info;
        info.set_mod(n);
        uint32_t d = (n - 1) >> std::countr_zero(n - 1), one = info.one(), minus_one = info.mod() - one;
        auto mr = [&](uint32_t a) {
            uint32_t s = d, y = info.pow(info.raw_init(a), s);
            while (s != n - 1 && info.strict_reduce(y) != one && info.strict_reduce(y) != minus_one) y = info.mul(y, y), s <<= 1;
            return info.strict_reduce(y) == minus_one || s % 2;
        };
        return mr(2) && mr(7) && mr(61);
    }
    bool is_prime64(uint64_t n) {
        if (n < 64) return 0x28208a20a08a28ac >> n & 1;
        if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;
        struct Info {
            uint64_t m_mod, m_mod2, m_pinv, m_ninv, m_inv, m_one;
            void set_mod(uint64_t P) {
                m_ninv = -__uint128_t(P) % P;
                m_mod = m_pinv = P, m_mod2 = P * 2, m_inv = uint64_t(-1) / P + 1;
                for (size_t i = 0; i != 5; i++) m_pinv *= uint64_t(2) - mod() * m_pinv;
                m_pinv = -m_pinv, m_one = strict_reduce(raw_init(1));
            }
            uint64_t mod(uint64_t val) const {
                uint64_t res = val - uint64_t((__uint128_t(val) * m_inv) >> 64) * mod();
                if (res >= mod()) res += mod();
                return res;
            }
            uint64_t mod() const { return m_mod; }
            uint64_t reduce(__uint128_t val) const {
                return (val + __uint128_t(uint64_t(val) * m_pinv) * mod()) >> 64;
            }
            uint64_t strict_reduce(uint64_t val) const { return val >= mod() ? val - mod() : val; }
            uint64_t mul(uint64_t a, uint64_t b) const {
                return reduce(__uint128_t(a) * b);
            }
            uint64_t pow(uint64_t a, uint64_t n) const {
                uint64_t res = one(), b = a;
                while (n) {
                    if (n & 1) res = mul(res, b);
                    b = mul(b, b), n >>= 1;
                }
                return res;
            }
            uint64_t one() const { return m_one; }
            uint64_t raw_init(uint64_t val) const { return mul(val, m_ninv); }
        };
        Info info;
        info.set_mod(n);
        uint64_t d = (n - 1) >> std::countr_zero(n - 1), one = info.one(), minus_one = info.mod() - one;
        auto mr = [d, info](uint32_t a) {
            if (a >= info.mod()) return true;
            uint64_t s = d, y = info.pow(info.raw_init(a), s);
            while (s != info.mod() - 1 && info.strict_reduce(y) != info.one() && info.strict_reduce(y) != info.mod() - info.one()) y = info.mul(y, y), s <<= 1;
            return info.strict_reduce(y) == info.mod() - info.one() || s % 2;
        };
        if (!(n >> 32)) return mr(2) && mr(7) && mr(61);
        return mr(2) && mr(325) && mr(9375) && mr(28178) && mr(450775) && mr(9780504) && mr(1795265022);
    }
    template <typename Tp>
    bool is_prime(Tp n) {
        if constexpr (std::is_same<Tp, uint32_t>::value)
            return is_prime32(n);
        else
            return is_prime64(n);
    }
}

#endif
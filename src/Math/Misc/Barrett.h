#ifndef __OY_BARRETT__
#define __OY_BARRETT__

#include <cstdint>

/**
 * @brief Barrett 约减
 * @example OY::Barrett32 brt(1000000007);
 * @note 加速以某数为除数的运算，可用于加速取余、乘法取模等操作
 * @note 当模数在编译器未知时可以提交效率
 */
namespace OY {
    template <typename _ModType>
    struct Barrett {
        _ModType m_P;
        __uint128_t m_Pinv;
        constexpr Barrett() = default;
        constexpr explicit Barrett(_ModType __P) : m_P(__P), m_Pinv(-uint64_t(__P) / __P + 1) {}
        /**
         * @brief 查询模数
         */
        constexpr _ModType mod() const { return m_P; }
        /**
         * @brief 快速取余
         */
        constexpr _ModType mod(uint64_t __a) const {
            __a -= uint64_t(m_Pinv * __a >> 64) * m_P + m_P;
            if (__a >= m_P) __a += m_P;
            return __a;
        }
        /**
         * @brief 快速加法
         */
        constexpr _ModType plus(_ModType __a, _ModType __b) {
            if (__a += __b; __a >= m_P) __a -= m_P;
            return __a;
        }
        /**
         * @brief 快速减法
         */
        constexpr _ModType minus(_ModType __a, _ModType __b) {
            if (__a += m_P - __b; __a >= m_P) __a -= m_P;
            return __a;
        }
        /**
         * @brief 快速乘法
         */
        constexpr _ModType multiply(uint64_t __a, uint64_t __b) const {
            if constexpr (std::is_same_v<_ModType, uint64_t>)
                return multiply_128(__a, __b);
            else
                return multiply_64(__a, __b);
        }
        /**
         * @brief 快速乘法
         */
        constexpr _ModType multiply_64(uint64_t __a, uint64_t __b) const {
            // assert(__a * __b < 1ull << 64);
            return mod(__a * __b);
        }
        /**
         * @brief 快速乘法
         */
        constexpr _ModType multiply_128(uint64_t __a, uint64_t __b) const {
            return __uint128_t(__a) * __b % m_P;
        }
        /**
         * @brief 快速乘法
         * @note 需要 long double 支持较高精度不会出错
         */
        constexpr _ModType multiply_ld(uint64_t __a, uint64_t __b) const {
            // assert(m_P < 1ull << 63 && __a < m_P && __b < m_P);
            int64_t res = __a * __b - uint64_t(1.L / m_P * __a * __b) * m_P;
            if (res < 0)
                res += m_P;
            else if (res >= m_P)
                res -= m_P;
            return res;
        }
        /**
         * @brief 快速幂
         */
        constexpr _ModType pow(uint64_t __a, uint64_t __n) const {
            if constexpr (std::is_same_v<_ModType, uint64_t>)
                return pow_128(__a, __n);
            else
                return pow_64(__a, __n);
        }
        /**
         * @brief 快速幂
         */
        constexpr _ModType pow_64(uint64_t __a, uint64_t __n) const {
            // assert(m_P < 1ull << 32);
            _ModType res = 1, b = mod(__a);
            while (__n) {
                if (__n & 1) res = multiply_64(res, b);
                b = multiply_64(b, b);
                __n >>= 1;
            }
            return res;
        }
        /**
         * @brief 快速幂
         */
        constexpr _ModType pow_128(uint64_t __a, uint64_t __n) const {
            _ModType res = 1, b = mod(__a);
            while (__n) {
                if (__n & 1) res = multiply_128(res, b);
                b = multiply_128(b, b);
                __n >>= 1;
            }
            return res;
        }
        /**
         * @brief 快速幂
         * @note 需要 long double 支持较高精度不会出错
         */
        constexpr _ModType pow_ld(uint64_t __a, uint64_t __n) const {
            _ModType res = 1, b = mod(__a);
            while (__n) {
                if (__n & 1) res = multiply_ld(res, b);
                b = multiply_ld(b, b);
                __n >>= 1;
            }
            return res;
        }
        /**
         * @brief 快速除法
         */
        template <typename _Tp>
        constexpr _Tp divide(_Tp __a) const {
            if (__a < m_P) return 0;
            _Tp res = m_Pinv * __a >> 64;
            if (__a - res * m_P >= m_P) res++;
            return res;
        }
        /**
         * @brief 快速带余数除法
         * @return 商和余数
         */
        template <typename _Tp>
        constexpr std::pair<_Tp, _Tp> divmod(_Tp __a) const {
            _Tp quo = (__a * m_Pinv) >> 64, rem = __a - quo * m_P;
            if (rem >= m_P) {
                quo++;
                rem -= m_P;
            }
            return {quo, rem};
        }
    };
    using Barrett32 = Barrett<uint32_t>;
    using Barrett64 = Barrett<uint64_t>;
}

#endif
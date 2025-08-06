#ifndef __OY_CHINESEREMAINDERTHEOREM__
#define __OY_CHINESEREMAINDERTHEOREM__

#include <algorithm>
#include <cstdint>
#include <vector>

#ifdef _MSC_VER
#include <immintrin.h>
#endif

/**
 * @brief 中国剩余定理 Chinese Remainder Theorem
 * @example OY::ChineseRemainderTheorem crt(n);
 * @note 本模板可以解决除数之间互质的情况，互质时一定有解。
 *
 * @note 问题描述：给定 $$n$$ 个方程 $$x \equiv rem_i \pmod{div_i}$$，求满足所有方程的最小非负整数解，其中 $$div_i$$ 两两互质。
 *
 * @note 令 $$\alpha = \prod_{i=1}^n div_i$$，且 $$div_i$$ 两两互质，记 $$\alpha_i = \frac{\alpha}{div_i}$$，其在模 $$div_i$$ 下的逆元为 $$inv_i$$，此时一定存在 $$\alpha_i \cdot inv_i \equiv 1 \pmod{div_i}$$。
 *
 * @note 计算 $$c_i = \alpha_i \cdot inv_i$$，方程组有唯一解 $$x_0 \equiv \sum_{i=1}^n c_i \cdot rem_i \pmod{\alpha}$$
 */
namespace OY {
    struct ChineseRemainderTheorem {
        struct node {
            uint64_t m_rem, m_div;
        };
        std::vector<node> m_sub;
        uint64_t m_prod = 1;
        ChineseRemainderTheorem(uint32_t n = 0) { m_sub.reserve(n); }
        /**
         * @brief 添加一个余数方程 $$x \equiv rem \pmod{div}$$
         */
        void add(uint64_t rem, uint64_t div) {
            if (div > 1) m_sub.push_back({rem, div}), m_prod *= div;
        }
        /**
         * @brief 计算 a 在模 p 下的逆元
         */
        static uint64_t getInv(uint64_t a, uint64_t p) {
            uint64_t x = p, y = a, m0 = 0, m1 = 1;
            while (y) {
                uint64_t z = x / y;
                x -= y * z, m0 -= m1 * z, std::swap(x, y), std::swap(m0, m1);
            }
            if (m0 >= p) m0 += p / x;
            return m0;
        }
        /**
         * @brief 返回符合要求的最小非负整数解和答案周期
         * @note 返回的周期为 $$\alpha$$，即所有除数的最小公倍数（互质时为乘积）。
         */
        node query() const {
            uint64_t res = 0;
            for (auto &sub : m_sub) {
                const uint64_t rem = sub.m_rem, div = sub.m_div;
                uint64_t rest = m_prod / div, inv = getInv(rest, div);
                auto mul = [&](uint64_t a, uint64_t b) {
#ifdef _MSC_VER
                    uint64_t high, low, res;
                    low = _umul128(a, b, &high);
                    _udiv128(high, low, m_prod, &res);
#else
                    uint64_t res = __uint128_t(a) * b % m_prod;
#endif
                    return res;
                };
                res += mul(mul(rem, rest), inv);
                if (res >= m_prod) res -= m_prod;
            }
            return {res, m_prod};
        }
    };
}

#endif
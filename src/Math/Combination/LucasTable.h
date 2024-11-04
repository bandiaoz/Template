#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 卢卡斯定理
 * @example OY::LucasTable<mint> table;
 */
namespace OY {
    template <typename Tp>
    struct LucasTable {
        using mod_type = uint32_t;
        std::vector<Tp> m_factorial, m_factorial_inv;
        LucasTable() {
            m_factorial.reserve(Tp::mod());
            m_factorial.push_back(Tp::raw(1));
            for (mod_type i = 1; i != Tp::mod(); i++) m_factorial.push_back(m_factorial.back() * Tp::raw(i));
            m_factorial_inv.reserve(Tp::mod());
            m_factorial_inv.push_back(m_factorial.back().inv());
            for (mod_type i = Tp::mod() - 1; i; i--) m_factorial_inv.push_back(m_factorial_inv.back() * Tp::raw(i));
            std::reverse(m_factorial_inv.begin(), m_factorial_inv.end());
        }
        /**
         * @brief 计算组合数 `C(n, m)`，限制 `n, m < Tp::mod()`
         */
        Tp raw_query(uint32_t n, uint32_t m) const {
            if (!m || n == m) return Tp::raw(1);
            if (n < m) return Tp::raw(0);
            return m_factorial[n] * m_factorial_inv[m] * m_factorial_inv[n - m];
        }
        /**
         * @brief 计算组合数 `C(n, m)`，支持 `n, m >= Tp::mod()`
         */
        Tp query(uint64_t n, uint64_t m) const {
            if (n < m) return 0;
            auto res = Tp::raw(1);
            while (m && n != m) {
                uint64_t nq = n / Tp::mod(), nr = n - nq * Tp::mod(), mq = m / Tp::mod(), mr = m - mq * Tp::mod();
                res *= raw_query(nr, mr), n = nq, m = mq;
            }
            return res;
        }
    };
}
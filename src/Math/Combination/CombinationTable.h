#ifndef __OY_COMBINATIONTABLE__
#define __OY_COMBINATIONTABLE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 排列组合表
 * @example OY::CombinationTable<Z, 100000> comb;
 */
namespace OY {
    template <typename Tp, uint32_t MAX_RANGE>
    class CombinationTable {
        Tp m_factorial[MAX_RANGE + 1], m_factorial_inv[MAX_RANGE + 1];
    public:
        CombinationTable(uint32_t range = MAX_RANGE) {
            m_factorial[0] = Tp::raw(1);
            for (uint32_t i = 1; i <= range; i++) m_factorial[i] = m_factorial[i - 1] * Tp::raw(i);
            m_factorial_inv[range] = m_factorial[range].inv();
            for (uint32_t i = range - 1; ~i; i--) m_factorial_inv[i] = m_factorial_inv[i + 1] * Tp::raw(i + 1);
        }
        /**
         * @brief 计算组合数
         */
        Tp binom(uint32_t n, uint32_t m) const {
            if (n < m) return Tp::raw(0);
            if (n == m) return Tp::raw(1);
            return m_factorial[n] * m_factorial_inv[m] * m_factorial_inv[n - m];
        }
        /**
         * @brief 计算全排列数，即 n!
         */
        Tp perm(uint32_t n) const { return m_factorial[n]; }
        /**
         * @brief 计算排列数
         */
        Tp perm(uint32_t n, uint32_t m) const {
            if (n < m) return Tp::raw(0);
            if (n == m) return m_factorial[n];
            return m_factorial[n] * m_factorial_inv[n - m];
        }
        /**
         * @brief 计算多重集的全排列数
         */
        template <typename Iterator>
        Tp perm_multi(Iterator first, Iterator last) const {
            Tp res(m_factorial[std::accumulate(first, last, 0)]);
            for (auto it = first; it != last; ++it) res *= m_factorial_inv[*it];
            return res;
        }
        /**
         * @brief 计算不相邻的组合数，即 binom(n - m + 1, m)
         */
        Tp comb_nonadjacent(uint32_t n, uint32_t m) const {
            if (n < m) return Tp::raw(0);
            return binom(n - m + 1, m);
        }
        /**
         * @brief 计算全错排列数
         */
        Tp perm_staggered(uint32_t n) const {
            static std::vector<Tp> s_res{1, 0};
            while (s_res.size() < n + 1) s_res.push_back((s_res[s_res.size() - 2] + s_res.back()) * Tp::raw(s_res.size() - 1));
            return s_res[n];
        }
        /**
         * @brief 计算圆上全排列，即 (n - 1)!
         */
        Tp perm_circular(uint32_t n) const { return m_factorial[n - 1]; }
        /**
         * @brief 计算圆上排列数，即 binom(n, m) * (m - 1)!
         */
        Tp perm_circular(uint32_t n, uint32_t m) const {
            if (!m) return Tp::raw(1);
            return binom(n, m) * m_factorial[m - 1];
        }
    };
}

#endif
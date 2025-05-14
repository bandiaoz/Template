#ifndef __OY_EULERSIEVE__
#define __OY_EULERSIEVE__

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <limits>
#include <vector>

#if __has_include(<bit>)
#include <bit>
#else
#include "src/Misc/std_bit.h"
#endif

/**
 * @brief 欧拉筛
 * @tparam MAX_RANGE 筛选的最大范围
 * @tparam GetPhi 是否获取欧拉函数值
 * @tparam GetSmallFactor 是否获取最小质因子
 * @tparam GetBigFactor 是否获取最大质因子
 * @example OY::EulerSieve::Sieve<1'000'000, false, true, false> sieve;
 */
namespace OY {
    namespace EulerSieve {
        using size_type = uint32_t;
        template <size_type MAX_RANGE, bool B>
        struct SieveArray {
            size_type m_val[MAX_RANGE + 1];
            void set(size_type i, size_type val) { m_val[i] = val; }
            size_type operator[](size_type i) const { return m_val[i]; }
        };
        template <size_type MAX_RANGE>
        struct SieveArray<MAX_RANGE, false> {
            void set(size_type i, size_type val) {}
            size_type operator[](size_type i) const { return 1; }
        };
        struct SievePair {
            size_type m_prime, m_count;
            bool operator<(const SievePair &rhs) const { return m_prime < rhs.m_prime; }
        };
        constexpr size_type get_estimated_ln(size_type x) {
            return x <= 7            ? 1
                   : x <= 32         ? 2
                   : x <= 119        ? 3
                   : x <= 359        ? 4
                   : x <= 1133       ? 5
                   : x <= 3093       ? 6
                   : x <= 8471       ? 7
                   : x <= 24299      ? 8
                   : x <= 64719      ? 9
                   : x <= 175196     ? 10
                   : x <= 481451     ? 11
                   : x <= 1304718    ? 12
                   : x <= 3524653    ? 13
                   : x <= 9560099    ? 14
                   : x <= 25874783   ? 15
                   : x <= 70119984   ? 16
                   : x <= 189969353  ? 17
                   : x <= 514278262  ? 18
                   : x <= 1394199299 ? 19
                                     : 20;
        }
        constexpr size_type get_estimated_Pi(size_type x) { return x / get_estimated_ln(x); }
        template <size_type MAX_RANGE, bool GetPhi = false, bool GetSmallFactor = false, bool GetBigFactor = false>
        class Sieve {
            static constexpr size_type max_pi = get_estimated_Pi(MAX_RANGE);
            SieveArray<MAX_RANGE, GetPhi> m_phi;
            SieveArray<MAX_RANGE, GetSmallFactor> m_smallest_factor;
            SieveArray<MAX_RANGE, GetBigFactor> m_biggest_factor;
            std::bitset<MAX_RANGE + 1> m_isprime;
            size_type m_primes[max_pi + 1], m_prime_cnt;
            template <typename Callback>
            void _dfs(size_type index, size_type prod, const std::vector<SievePair> &pairs, Callback &&call) const {
                if (index == pairs.size())
                    call(prod);
                else {
                    auto &&pair = pairs[index];
                    size_type p = pair.m_prime, c = pair.m_count;
                    _dfs(index + 1, prod, pairs, call);
                    while (c--) _dfs(index + 1, prod *= p, pairs, call);
                }
            }
        public:
            Sieve(size_type range = MAX_RANGE) { resize(range); }
            void resize(size_type range) {
                if (!range) return;
                m_isprime.set();
                m_isprime.reset(0);
                m_prime_cnt = 0;
                if (range >= 1) m_isprime.reset(1), m_smallest_factor.set(1, 1), m_biggest_factor.set(1, 1), m_phi.set(1, 1);
                for (int i = 2; i <= range; i++) {
                    if (m_isprime[i]) m_smallest_factor.set(i, i), m_biggest_factor.set(i, i), m_phi.set(i, i - 1), m_primes[m_prime_cnt++] = i;
                    for (size_type *it = m_primes, *end = m_primes + m_prime_cnt; it != end; ++it) {
                        auto p = *it, q = i * p;
                        if (q > range) break;
                        m_isprime.reset(q), m_smallest_factor.set(q, p), m_biggest_factor.set(q, m_biggest_factor[i]);
                        if (i % p)
                            m_phi.set(q, m_phi[i] * (p - 1));
                        else {
                            m_phi.set(q, m_phi[i] * p);
                            break;
                        }
                    }
                }
                m_primes[m_prime_cnt] = std::numeric_limits<size_type>::max() / 2;
            }
            bool is_prime(size_type i) const { return (i & 1) || i == 2 ? m_isprime[i] : false; }
            /**
             * @brief 查询欧拉函数值
             */
            size_type get_Euler_Phi(size_type i) const {
                static_assert(GetPhi);
                return (i & 1) ? m_phi[i] : m_phi[i >> std::countr_zero(i)] << std::countr_zero(i) - 1;
            }
            /**
             * @brief 查询最小质因子
             */
            size_type query_smallest_factor(size_type i) const {
                static_assert(GetSmallFactor);
                return (i & 1) ? m_smallest_factor[i] : 2;
            }
            /**
             * @brief 查询最大质因子
             */
            size_type query_biggest_factor(size_type i) const {
                static_assert(GetBigFactor);
                return m_biggest_factor[i];
            }
            /**
             * @brief 查询第 k 个质数，k 从 0 开始
             */
            size_type query_kth_prime(size_type k) const { return m_primes[k]; }
            /**
             * @brief 查询质数数量
             */
            size_type count() const { return m_prime_cnt; }
            /**
             * @brief 分解质因数，返回 {质因子, 次数}
             */
            std::vector<SievePair> decomposite(size_type n) const {
                static_assert(GetSmallFactor);
                std::vector<SievePair> res;
                if (n % 2 == 0) {
                    size_type x = std::countr_zero(n);
                    res.push_back({2, x}), n >>= x;
                }
                while (n > 1) {
                    size_type cur = query_smallest_factor(n), cnt = 0;
                    do {
                        n /= cur, cnt++;
                    } while (query_smallest_factor(n) == cur);
                    res.push_back({cur, cnt});
                }
                return res;
            }
            template <typename Callback>
            void enumerate_factors(const std::vector<SievePair> &pairs, Callback &&call) const { _dfs(0, 1, pairs, call); }
            template <typename Callback>
            void enumerate_factors(size_type n, Callback &&call) const { enumerate_factors(decomposite(n), call); }
            /**
             * @brief 查询 n 的所有因子
             * @tparam Sorted 是否排序
             */
            template <bool Sorted = false>
            std::vector<size_type> get_factors(size_type n) const {
                static_assert(GetSmallFactor);
                std::vector<size_type> res;
                size_type count = 1;
                auto pairs = decomposite(n);
                for (auto &&pair : pairs) count *= pair.m_count + 1;
                res.reserve(count);
                enumerate_factors(pairs, [&](size_type f) { res.push_back(f); });
                if constexpr (Sorted) std::sort(res.begin(), res.end());
                return res;
            }
        };
    }
}

#endif
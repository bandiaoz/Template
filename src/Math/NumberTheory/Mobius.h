#ifndef __OY_MOBIUS__
#define __OY_MOBIUS__

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
 * @brief 莫比乌斯函数，积性函数
 * @example OY::MOBIUS::Table<MAX_RANGE, RangeQuery = false> mobius;
 * @note 数论函数（也称算数函数）指定义域为正整数的函数，也可以视为数列。
 *
 * @note 在数论中，若函数 $$f(n)$$ 满足 $$f(1) = 1$$ 且 $$\forall x, y \in \mathbb{N}^*, \gcd(x, y) = 1$$ 都有 $$f(xy) = f(x) f(y)$$，则 $$f(n)$$ 为积性函数。
 *       在数论中，若函数 $$f(n)$$ 满足 $$f(1) = 1$$ 且 $$\forall x, y \in \mathbb{N}^*$$ 都有 $$f(xy) = f(x) f(y)$$，则 $$f(n)$$ 为完全积性函数。
 * @note 积性函数的例子：
 *       单位函数：$$\varepsilon(n) = [n = 1]$$，完全积性，有 $$\varepsilon \ast f = f$$
 *       恒等函数：$$id_k(n) = n^k$$，通常地，将 $$id_1(n)$$ 记为 $$id(n)$$，完全积性
 *       常数函数：$$1(n) = 1$$，完全积性，有 $$1^{-1} = \mu$$
 *
 *       除数函数：$$\sigma_k(n) = \sum_{d \mid n} d^k$$，通常地，将 $$\sigma_0(n)$$ 记为 $$d(n)$$ 或者 $$\tau(n)$$，$$\sigma_1(n)$$ 记为 $$\sigma(n)$$，并且有 $$\sigma_k = \mathrm{id}_k \ast 1$$
 *
 *       欧拉函数：$$\varphi(n) = \sum_{i = 1}^n [\gcd(i, n) = 1] = \mu \ast id$$，并且有 $$\varphi \ast 1 = \mathrm{id}$$
 *
 *       莫比乌斯函数：$$\mu(n)$$，$$\mu \ast 1 = \varepsilon$$
 *
 * @note Dirichlet 卷积：对于两个数论函数 $$f$$ 和 $$g$$，其 Dirichlet 卷积定义为：
 *
 *       $$h(n) = (f \ast g)(n) = \sum_{d \mid n} f(d) g(\frac{n}{d}) = \sum_{ab = n} f(a) g(b)$$
 * @note Dirichlet 性质：
 *      1. 交换律：$$f \ast g = g \ast f$$
 *      2. 结合律：$$(f \ast g) \ast h = f \ast (g \ast h)$$
 *      3. 分配律：$$(f + g) \ast h = f \ast h + g \ast h$$
 *      4. 等式的性质：$$f = g$$ 的充要条件是 $$f \ast h = g \ast h$$，其中数论函数 $$h(x)$$ 要满足 $$h(1) \neq 0$$。
 *      5. 单位元：$$f \ast \varepsilon = f$$
 *      6. 逆元：$$f \ast f^{-1} = \varepsilon$$，其中 $$f^{-1}$$ 为 $$f$$ 的逆元
 * @note 重要结论 1：若 $$f$$ 和 $$g$$ 为积性函数，则其 Dirichlet 卷积也是积性函数
 *       证明如下：
 *       设两个积性函数 $$f(x)$$ 和 $$g(x)$$，记 $$h = f \ast g$$，$$(a, b) = 1$$，则有
 *
 *       $$h(a)h(b) = \sum_{d_1 \mid a} f(d_1) g(\frac{a}{d_1}) \sum_{d_2 \mid b} f(d_2) g(\frac{b}{d_2}) = \sum_{d \mid ab} f(d)g(\frac{ab}{d}) = h(ab)$$
 *
 * @note 重要结论 2：积性函数的逆元也是积性函数
 *
 * @note 引理 1：
 *       $$\forall a, b, c \in \mathbb{Z}, \left\lfloor \frac{a}{bc} \right\rfloor = \left\lfloor \frac{\left\lfloor \frac{a}{b} \right\rfloor}{c} \right\rfloor$$
 *
 *      证明如下：
 *      设 $$a = qc + r$$，其中 $$0 \leq r < c$$，则 $$\left\lfloor \frac{a}{c} \right\rfloor = q$$，
 *
 *      于是 $$\left\lfloor \frac{a}{bc} \right\rfloor = \left\lfloor \frac{qc + r}{bc} \right\rfloor = \left\lfloor \frac{q}{b} + \frac{r}{bc} \right\rfloor = \left\lfloor \frac{q}{b} \right\rfloor = \left\lfloor \frac{\left\lfloor \frac{a}{b} \right\rfloor}{c} \right\rfloor$$
 *
 *      （由于 $$\frac{r}{c} < 1$$，无法造成有效的贡献，故舍去）
 *
 * @note 定义莫比乌斯函数 $$\mu(x)$$：
 *       $$\mu(x) = \begin{cases}
        1, & x = 1 \\
        (-1)^k, & x = p_1p_2\cdots p_k \\
        0, & \text{otherwise}(p^2 \mid x)
        \end{cases}$$
 * @note $$\mu$$ 函数有个性质：$$\sum_{d \mid n} \mu(d) = [n = 1]$$，也可以写成 $$\mu \ast 1 = \varepsilon$$
 *       证明如下：
 *       考虑 $$n = \prod p_i^{c_i}$$，$$n' = \prod p_i$$，则
 *
 *       $$\sum_{d \mid n} \mu(d) = \sum_{d \mid n'} \mu(d) = \sum_{i = 0}^k \binom{k}{i}(-1)^i = (1 + (-1))^k = [n = 1]$$
 *       
 *       根据这个公式可以得到反演公式：$$[\gcd(i, j) = 1] = \sum_{d \mid \gcd(i, j)} \mu(d)$$
 *
 * @note 莫比乌斯反演：设 $$f(n)$$ 和 $$g(n)$$ 为两个数论函数
 *
 *       1. 若 $$f(n) = \sum_{d \mid n} g(d)$$，则有 $$g(n) = \sum_{d \mid n} \mu(d) f(\frac{n}{d})$$，也可以表达为 $$f = g \ast 1 \Leftrightarrow g = f \ast \mu$$
 *
 *       证明：$$f = g \ast 1$$，则 $$f \ast \mu = g \ast 1 \ast \mu = g \ast \varepsilon = g$$
 *
 *       2. 若 $$f(n) = \sum_{n \mid d} g(d)$$，则有 $$g(n) = \sum_{n \mid d} \mu(\frac{d}{n}) f(d)$$
 *
 * @note 记 $$d(x)$$ 为 $$x$$ 的因数个数，有公式 $$d(ij) = \sum_{x \mid i} \sum_{y \mid j} [(x, y) = 1]$$
 */
namespace OY {
    namespace MOBIUS {
        using size_type = uint32_t;
        struct CompoundPlus {
            template <typename Tp>
            void operator()(Tp &x, const Tp &y) const { x += y; }
        };
        struct CompoundMinus {
            template <typename Tp>
            void operator()(Tp &x, const Tp &y) const { x -= y; }
        };
        template <size_type MAX_RANGE, bool Flag>
        struct OptionalArray {
            size_type m_data[MAX_RANGE + 1];
            template <typename Mapping>
            void prepare_presum(size_type range, Mapping mapping) {
                m_data[0] = 0;
                for (size_type i = 1; i <= range; i++) m_data[i] = m_data[i - 1] + mapping(i);
            }
            void set(size_type i, size_type val) { m_data[i] = val; }
            size_type query(size_type left, size_type right) const { return m_data[right - 1] - m_data[left - 1]; }
            size_type operator[](size_type i) const { return m_data[i]; }
        };
        template <size_type MAX_RANGE>
        struct OptionalArray<MAX_RANGE, false> {};
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
        template <size_type MAX_RANGE, bool RangeQuery = false>
        struct Table {
            static constexpr size_type max_pi = get_estimated_Pi(MAX_RANGE);
            char m_val[MAX_RANGE + 1];
            std::bitset<MAX_RANGE + 1> m_notp;
            size_type m_primes[max_pi + 1], m_pcnt;
            OptionalArray<MAX_RANGE, RangeQuery> m_mobius_presum;
            Table(size_type range = MAX_RANGE) { resize(range); }
            void resize(size_type range) {
                if (!range) return;
                m_val[1] = 1;
                m_notp.set(1);
                m_pcnt = 0;
                m_primes[m_pcnt++] = 2;
                for (size_type i = 3; i <= range; i += 2) {
                    if (!m_notp[i]) m_primes[m_pcnt++] = i, m_val[i] = -1;
                    for (size_type j = 1; j != m_pcnt && i * m_primes[j] <= range; j++) {
                        m_notp.set(i * m_primes[j]);
                        if (i % m_primes[j] == 0) break;
                        m_val[i * m_primes[j]] = -m_val[i];
                    }
                }
                if constexpr (RangeQuery) m_mobius_presum.prepare_presum(range, [&](size_type i) { return query_mobius(i); });
                m_primes[m_pcnt] = std::numeric_limits<size_type>::max() / 2;
            }
            /**
             * @brief 查询莫比乌斯函数值 `μ(x)`
             */
            int query_mobius(size_type x) const { return (x & 3) ? ((x & 1) ? m_val[x] : -m_val[x >> 1]) : 0; }
            /**
             * @brief 查询区间 `[left, right)` 莫比乌斯函数的和
             */
            int query_mobius(size_type left, size_type right) const {
                static_assert(RangeQuery, "RangeQuery Must Be True");
                return m_mobius_presum.query(left, right);
            }
            /**
             * @brief 查询 `x` 是否为质数
             */
            bool is_prime(size_type x) const { return (x & 1) ? !m_notp[x] : x == 2; }
            /**
             * @brief 查询第 `k` 个质数，`k` 从 `0` 开始
             */
            size_type query_kth_prime(size_type k) const { return m_primes[k]; }
        };
        template <size_type MAX_RANGE>
        struct Multiplicative {
            static constexpr size_type max_pi = get_estimated_Pi(MAX_RANGE);
            struct node {
                size_type m_val, m_cnt, m_low, m_prod;
            };
            node m_minp[MAX_RANGE / 2];
            size_type m_primes[max_pi];
            template <typename Tp, typename CalcPrime, typename CalcPrimePow>
            void solve(size_type range, Tp *array, CalcPrime &&calc_prime, CalcPrimePow &&calc_prime_pow) {
                if (range >= 1) array[1] = Tp(1);
                if (range >= 2) array[2] = calc_prime(2);
                for (size_type x = 4, c = 2; x <= range; x <<= 1, c++) array[x] = calc_prime_pow(2, c, x >> 1);
                for (size_type i = 3, odd_pcnt = 0; i <= range; i += 2) {
                    if (!m_minp[i / 2].m_val) {
                        m_primes[odd_pcnt++] = i;
                        m_minp[i / 2] = {i, 1, 1, i};
                        array[i] = calc_prime(i);
                    } else
                        array[i] = m_minp[i / 2].m_prod == i ? calc_prime_pow(m_minp[i / 2].m_val, m_minp[i / 2].m_cnt, m_minp[i / 2].m_low) : array[m_minp[i / 2].m_prod] * array[m_minp[i / 2].m_low];
                    for (size_type j = 0; j != odd_pcnt && i * m_primes[j] <= range; j++) {
                        size_type p = m_primes[j];
                        m_minp[i * p / 2].m_val = p;
                        if (i % p == 0) {
                            m_minp[i * p / 2].m_cnt = m_minp[i / 2].m_cnt + 1;
                            m_minp[i * p / 2].m_low = m_minp[i / 2].m_prod == i ? i : m_minp[i / 2].m_low;
                            m_minp[i * p / 2].m_prod = m_minp[i / 2].m_prod * p;
                            break;
                        }
                        m_minp[i * p / 2].m_cnt = 1;
                        m_minp[i * p / 2].m_low = i;
                        m_minp[i * p / 2].m_prod = p;
                    }
                    if (i + 1 <= range) {
                        size_type ctz = std::countr_zero(i + 1);
                        if (i + 1 != 1 << ctz) array[i + 1] = array[1 << ctz] * array[i + 1 >> ctz];
                    }
                }
            }
            /**
             * @brief 计算积性函数，返回一个数组
             * @note 对于积性函数 $$f$$，如果 $$\gcd(n, m) = 1$$ 有 $$f(n, m) = f(n) f(m)$$。
             * @param range 范围
             * @param calc_prime(p) 计算 $$f(p)$$ 的值，其中 $$p$$ 为质数
             * @param calc_prime_pow(p, c, low) 计算 $$f(p^c)$$ 的值，其中 $$p$$ 为质数，$$c$$ 为质数幂，$$low$$ 为 $$p^{c-1}$$
             * @return 积性函数数组
             */
            template <typename Tp, typename CalcPrime, typename CalcPrimePow>
            std::vector<Tp> solve(size_type range, CalcPrime &&calc_prime, CalcPrimePow &&calc_prime_pow) {
                std::vector<Tp> res(range + 1);
                solve(range, res.data(), calc_prime, calc_prime_pow);
                return res;
            }
        };
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundPlus>
        inline void partial_sum_Dirichlet_divisor(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = 1, k = p; k <= range; j++, k += p) op(array[k], array[j]);
            }
        }
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundMinus>
        inline void adjacent_difference_Dirichlet_divisor(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = range / p, k = j * p; k; j--, k -= p) op(array[k], array[j]);
            }
        }
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundPlus>
        inline void partial_sum_Dirichlet_multiple(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = range / p, k = j * p; k; j--, k -= p) op(array[j], array[k]);
            }
        }
        template <typename Tp, typename FindKthPrime, typename Operation = CompoundMinus>
        inline void adjacent_difference_Dirichlet_multiple(size_type range, Tp *array, FindKthPrime &&find_kth_prime, Operation &&op = Operation()) {
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p > range) break;
                for (size_type j = 1, k = p; k <= range; j++, k += p) op(array[j], array[k]);
            }
        }
    }
}

#endif
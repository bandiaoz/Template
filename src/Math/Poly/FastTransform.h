#ifndef __OY_FASTTRANSFORM__
#define __OY_FASTTRANSFORM__

#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 快速卷积变换
 * @note 用于解决形如 $$c_k = \sum_{op(i,j)=k}a_i\cdot b_j$$ 的卷积问题，其中 $$op$$ 是一种二元运算，例如按位与、按位或、按位异或、最大值、最小值、gcd、lcm 等
 *
 * @note OY::FASTTRANS::fast_xxx_transform<Forward>(Iterator first, Iterator last);
 *       Forward 为 true 表示从点值域变换为卷积域，为 false 表示从卷积域变换为点值域
 */
namespace OY {
    /**
     * 原理概述：将按某种二元运算定义的卷积，转化为适配该运算的线性变换（如 Hadamard、子集/超集 zeta、
     *  以及在约数-倍数偏序上的 Dirichlet zeta）。做两次变换（正变换与逆变换）之间即可逐点相乘，从而把卷积从“卷积域”转到“点值域”。不同运算对应不同的可逆线性变换。
     * 常见复杂度：
     *  XOR 变换（Hadamard/FWT-XOR）：O(n log n)
     *  OR / AND 变换（子集/超集 FWT）：O(n log n)
     *  max / min 变换（前缀和或后缀和形式）：O(n)
     *  gcd / lcm 变换（在约数-倍数偏序上的 zeta，按质数枚举）：O(n log log n)
     */
    namespace FASTTRANS {
        using size_type = uint32_t;
        struct DivideBy2_naive {
            template <typename Tp>
            Tp operator()(Tp x) const { return x / 2; }
        };
        struct FastTransformer {
            size_type m_length;
            std::vector<std::pair<size_type, size_type>> m_transfers;
            FastTransformer(size_type length, size_type transfer_cnt) : m_length(length) { m_transfers.reserve(transfer_cnt); }
            void add_transfer(size_type prev, size_type next) { m_transfers.emplace_back(prev, next); }
            template <bool Forward, typename Sequence>
            void transform(Sequence &sequence) {
                if constexpr (Forward)
                    for (auto &[prev, next] : m_transfers) {
                        sequence[next] += sequence[prev];
                    }
                else
                    for (auto it = m_transfers.rbegin(); it != m_transfers.rend(); ++it) {
                        auto &[prev, next] = *it;
                        sequence[next] -= sequence[prev];
                    }
            }
        };
        template <bool Forward, typename Iterator, typename DivideBy2 = DivideBy2_naive>
        void fast_bitxor_transform(Iterator first, Iterator last, DivideBy2 div_by2 = DivideBy2()) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it = x + y, *it2 = x - y;
                        else
                            *it = div_by2(x + y), *it2 = div_by2(x - y);
                    }
        }
        /**
         * @brief 按位或变换
         *
         * @note 正变换：$$A_i = \sum_{j \text{ or } i = i} A_j$$，
         *
         * @note 逆变换：
         *
         * @note 复杂度：$$O(n \log n)$$
         */
        template <bool Forward, typename Iterator>
        void fast_bitor_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it2 = x + y;
                        else
                            *it2 = y - x;
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_bitand_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it = x + y;
                        else
                            *it = x - y;
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_max_transform(Iterator first, Iterator last) {
            if constexpr (Forward)
                std::partial_sum(first, last, first);
            else
                std::adjacent_difference(first, last, first);
        }
        template <bool Forward, typename Iterator>
        void fast_min_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            if constexpr (Forward)
                for (size_type i = length - 2; ~i; i--) *(first + i) += *(first + i + 1);
            else
                for (size_type i = 0; i + 1 < length; i++) *(first + i) -= *(first + i + 1);
        }
        template <bool Forward, typename Iterator, typename FindKthPrime>
        void fast_gcd_transform(Iterator first, Iterator last, FindKthPrime &&find_kth_prime) {
            const size_type length = last - first;
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p >= length) break;
                if constexpr (Forward)
                    for (size_type j = (length - 1) / p; j; j--) *(first + j) += *(first + j * p);
                else
                    for (size_type j = 0, end = (length - 1) / p; j <= end; j++) *(first + j) -= *(first + j * p);
            }
        }
        template <bool Forward, typename Iterator, typename FindKthPrime>
        void fast_lcm_transform(Iterator first, Iterator last, FindKthPrime &&find_kth_prime) {
            const size_type length = last - first;
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p >= length) break;
                if constexpr (Forward)
                    for (size_type j = 0, end = (length - 1) / p; j <= end; j++) *(first + j * p) += *(first + j);
                else
                    for (size_type j = (length - 1) / p; j; j--) *(first + j * p) -= *(first + j);
            }
        }
    }
}

#endif
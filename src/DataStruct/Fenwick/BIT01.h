#ifndef __OY_BIT01__
#define __OY_BIT01__

#include <cstdint>
#include <vector>

#if __has_include(<bit>)
#include <bit>
#else
#include "src/Misc/std_bit.h"
#endif

/**
 * @brief 01 树状数组
 * @example OY::BIT01::Tree tree(length, mapping);
 * @note 本模板为 `BIT` 的特化，单点元素值只有 `0` 和 `1` 两种情况。
 */
namespace OY {
    namespace BIT01 {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        class Tree {
            size_type m_size, m_icap;
            std::vector<size_type> m_sum;
            std::vector<mask_type> m_bits;
        public:
            Tree() = default;
            Tree(size_type length) { resize(length); }
            template <typename InitMaping>
            Tree(size_type length, InitMaping mapping) { resize(length, mapping); }
            size_type size() const { return m_size; }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                size_type cnt = (length + MASK_SIZE - 1) / MASK_SIZE;
                m_icap = std::bit_ceil(cnt);
                m_sum.assign(m_icap, {}), m_bits.assign(m_icap + 1, {});
            }
            template <typename InitMaping>
            void resize(size_type length, InitMaping mapping) {
                if (!(m_size = length)) return;
                size_type cnt = (length + MASK_SIZE - 1) / MASK_SIZE;
                m_icap = std::bit_ceil(cnt);
                m_sum.resize(m_icap), m_bits.assign(m_icap + 1, {});
                for (size_type i = 0, j = 0; j != m_icap; i += MASK_SIZE, j++) {
                    for (size_type k = 0; k != MASK_SIZE && i + k < m_size; k++) m_bits[j] |= mask_type(mapping(i + k) ? 1 : 0) << k;
                    m_sum[j] = std::popcount(m_bits[j]);
                }
                for (size_type i = 0; i != m_icap; i++) {
                    size_type j = i + lowbit(i + 1);
                    if (j < m_icap) m_sum[j] += m_sum[i];
                }
            }
            /**
             * @brief 将下标为 `i` 的元素置为 1
             */
            void set(size_type i) {
                if (!(m_bits[i >> MASK_WIDTH] >> (i & (MASK_SIZE - 1)) & 1)) {
                    m_bits[i >> MASK_WIDTH] |= mask_type(1) << (i & (MASK_SIZE - 1));
                    for (i >>= MASK_WIDTH; i < m_icap; i += lowbit(i + 1)) ++m_sum[i];
                }
            }
            /**
             * @brief 将下标为 `i` 的元素置为 0
             */
            void reset(size_type i) {
                if (m_bits[i >> MASK_WIDTH] >> (i & (MASK_SIZE - 1)) & 1) {
                    m_bits[i >> MASK_WIDTH] &= ~(mask_type(1) << (i & (MASK_SIZE - 1)));
                    for (i >>= MASK_WIDTH; i < m_icap; i += lowbit(i + 1)) --m_sum[i];
                }
            }
            /**
             * @brief 全区间查询，返回区间 `[0, n)` 的和
             */
            size_type query_all() const { return m_sum[m_icap - 1]; }
            /**
             * @brief 前缀和查询，返回闭区间 `[0, i)` 的和
             */
            size_type presum(size_type i) const {
                i--;
                size_type res = std::popcount(m_bits[(i + 1) / MASK_SIZE] & ((mask_type(1) << ((i + 1) & (MASK_SIZE - 1))) - 1));
                for (size_type j = (i + 1) / MASK_SIZE - 1; ~j; j -= lowbit(j + 1)) res += m_sum[j];
                return res;
            }
            /**
             * @brief 单点查询，返回下标为 `i` 的元素
             */
            size_type query(size_type i) const { return m_bits[i >> MASK_WIDTH] >> (i & (MASK_SIZE - 1)) & 1; }
            /**
             * @brief 区间查询，返回区间 `[left, right)` 的和
             */
            size_type query(size_type left, size_type right) const { return presum(right) - presum(left); }
            /**
             * @brief 查询第 k 个元素的下标
             * @note 也可以解释为，本方法在求可令 `presum(i+1) > k` 成立的最小的 `i` 。
             *       `k` 必须位于 `[0, query_all())` 之间。
             */
            size_type kth(size_type k) const {
                size_type cursor = -1;
                for (size_type d = m_icap >> 1; d; d >>= 1)
                    if (m_sum[cursor + d] <= k) k -= m_sum[cursor += d];
                mask_type x = m_bits[cursor + 1];
                while (k--) x -= x & -x;
                return ((cursor + 1) << MASK_WIDTH) + std::countr_zero(x);
            }
            size_type smaller_bound(size_type i) const {
                mask_type low = (mask_type(1) << (i & (MASK_SIZE - 1))) - 1;
                if (m_bits[i >> MASK_WIDTH] & low) return (i & ~(MASK_SIZE - 1)) + std::bit_width(m_bits[i >> MASK_WIDTH] & low) - 1;
                size_type k{};
                for (size_type j = (i >> MASK_WIDTH) - 1; ~j; j -= lowbit(j + 1)) k += m_sum[j];
                if (!k--) return -1;
                size_type cursor = -1;
                for (size_type d = m_icap >> 1; d; d >>= 1)
                    if (m_sum[cursor + d] <= k) k -= m_sum[cursor += d];
                return ((cursor + 1) << MASK_WIDTH) + std::bit_width(m_bits[cursor + 1]) - 1;
            }
            size_type lower_bound(size_type i) const {
                mask_type high = -(mask_type(1) << (i & (MASK_SIZE - 1)));
                if (m_bits[i >> MASK_WIDTH] & high) return (i & ~(MASK_SIZE - 1)) + std::countr_zero(m_bits[i >> MASK_WIDTH] & high);
                size_type k{};
                for (size_type j = i >> MASK_WIDTH; ~j; j -= lowbit(j + 1)) k += m_sum[j];
                if (k == query_all()) return -1;
                size_type cursor = -1;
                for (size_type d = m_icap >> 1; d; d >>= 1)
                    if (m_sum[cursor + d] <= k) k -= m_sum[cursor += d];
                return ((cursor + 1) << MASK_WIDTH) + std::countr_zero(m_bits[cursor + 1]);
            }
            size_type upper_bound(size_type i) const { return i == m_size - 1 ? -1 : lower_bound(i + 1); }
        };
    }
}

#endif
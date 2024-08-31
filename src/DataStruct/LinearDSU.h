#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "src/Misc/std_bit.h"

/**
 * @brief 线性并查集
 * @tparam MaintainGroupSize 是否维护每个组的大小
 * @example OY::LDSU::Table<true> dsu(n);
 */
namespace OY {
    namespace LDSU {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        template <bool MaintainGroupSize>
        class Table {
            std::vector<mask_type> m_masks;
            mutable std::vector<size_type> m_tail, m_group_size;
            size_type m_size, m_group_cnt;
            size_type _find(size_type q) const { return m_masks[q] ? q : (m_tail[q] = _find(m_tail[q])); }
        public:
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = n)) return;
                m_group_cnt = m_size;
                m_masks.assign((m_size + MASK_SIZE - 1) >> MASK_WIDTH, -1), m_tail.resize((m_size + MASK_SIZE - 1) >> MASK_WIDTH);
                if constexpr (MaintainGroupSize) m_group_size.assign(m_size, 1);
                for (size_t i = 0; i != m_tail.size(); i++) m_tail[i] = i;
            }
            /**
             * @brief 查询 i 所在的组的头部
             */
            size_type find_head(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                size_type tail = find_tail(i);
                return tail - m_group_size[tail] + 1;
            }
            /**
             * @brief 查询 i 所在的组的尾部
             */
            size_type find_tail(size_type i) const {
                size_type quot = i >> MASK_WIDTH, rem = i & (MASK_SIZE - 1);
                if (m_masks[quot] >> rem & mask_type(1))
                    return i;
                else if (mask_type mask = m_masks[quot] & -(mask_type(1) << rem))
                    return (quot << MASK_WIDTH) ^ std::countr_zero(mask);
                else {
                    m_tail[quot] = _find(m_tail[quot]);
                    return (m_tail[quot] << MASK_WIDTH) ^ std::countr_zero(m_masks[m_tail[quot]]);
                }
            }
            size_type find_prev(size_type i) const { return find_head(i) - 1; }
            size_type find_next(size_type i) const { return find_tail(i) + 1; }
            /**
             * @brief 查询 i 所在的组的大小，即 m_group_size[find_tail(i)]
             * @tparam IsTail 是否已经是这个组的尾部
             */
            template <bool IsTail = false>
            size_type size(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                if constexpr (IsTail)
                    return m_group_size[i];
                else
                    return m_group_size[find_tail(i)];
            }
            /**
             * @brief 把 i 和 i + 1 合并
             */
            bool unite_after(size_type i) {
                size_type quot = i >> MASK_WIDTH, rem = i & (MASK_SIZE - 1);
                if (m_masks[quot] >> rem & mask_type(1)) {
                    m_masks[quot] &= ~(mask_type(1) << rem);
                    if (rem + 1 == MASK_SIZE) m_tail[quot] = m_masks[quot + 1] ? quot + 1 : m_tail[quot + 1];
                    if constexpr (MaintainGroupSize) m_group_size[find_tail(i)] += m_group_size[i];
                    m_group_cnt--;
                    return true;
                } else
                    return false;
            }
            /**
             * @brief 查询 left 和 right 是否在同一个组，需要保证 left < right
             */
            bool in_same_group(size_type left, size_type right) const { return find_tail(left) >= right; }
            /**
             * @brief 查询 i 是否是一个组的头部
             */
            bool is_head(size_type i) const { return !i || is_tail(i - 1); }
            /**
             * @brief 查询 i 是否是一个组的尾部
             */
            bool is_tail(size_type i) const { return m_masks[i >> MASK_WIDTH] >> (i & (MASK_SIZE - 1)) & mask_type(1); }
            /**
             * @brief 查询组的数量
             */
            size_type count() const { return m_group_cnt; }
            /**
             * @brief 返回所有组的头部
             */
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i != m_size; i = find_tail(i) + 1) ret.push_back(i);
                return ret;
            }
            /**
             * @brief 返回所有组的尾部
             */
            std::vector<size_type> tails() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                size_type i = find_tail(0);
                do ret.push_back(i);
                while ((i = find_tail(i + 1)) != m_size);
                return ret;
            }
        };
        template <typename Ostream, bool MaintainGroupSize>
        Ostream &operator<<(Ostream &out, const Table<MaintainGroupSize> &x) {
            out << '[';
            for (size_type l = 0, r; l != x.m_size; l = r + 1) {
                r = x.find_tail(l);
                if (l) out << ", ";
                out << "[" << l << ", " << r << "]";
            }
            return out << ']';
        }
    }
}
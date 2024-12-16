#ifndef __OY_FASTSEQUENCEAUTOMATON__
#define __OY_FASTSEQUENCEAUTOMATON__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 快速序列自动机
 * @tparam MAX_VALUE 字符集大小，区间中出现的最大值
 * @example OY::FastSequenceAutomaton<26> sa(n, [&](int i) { return a[i] - 'a'; });
 * @example OY::FastSequenceAutomaton<100> sa(a.begin(), a.end());
 */
namespace OY {
    template <uint32_t MAX_VALUE>
    class FastSequenceAutomaton {
        struct node {
            uint32_t m_pos[MAX_VALUE + 1];
        };
        /**
         * @brief `m_next[i].m_pos[j]` 表示在原序列中，下标大于等于 `i` 的范围内出现的第一个 `j` 的下标。
         *          如果不存在，则 `m_next[i].m_pos[j] = length`，其中 `length` 为原序列长度。
         */
        std::vector<node> m_next;
    public:
        FastSequenceAutomaton() = default;
        /**
         * @brief 初始化序列自动机
         * @param length 序列长度
         * @param mapping 表示区间下标到元素值的映射
         * @note 时间复杂度 $O(length \cdot MAX\_VALUE)$
         */
        template <typename InitMapping>
        FastSequenceAutomaton(uint32_t length, InitMapping mapping) { resize(length, mapping); }
        /**
         * @brief 根据迭代器范围初始化序列自动机
         */
        template <typename Iterator>
        FastSequenceAutomaton(Iterator first, Iterator last) { reset(first, last); }
        template <typename InitMapping>
        void resize(uint32_t length, InitMapping mapping) {
            m_next.resize(length + 2);
            std::fill_n(m_next[length + 1].m_pos, MAX_VALUE + 1, length);
            std::fill_n(m_next[length].m_pos, MAX_VALUE + 1, length);
            for (uint32_t i = length - 1; ~i; i--) {
                std::copy_n(m_next[i + 1].m_pos, MAX_VALUE + 1, m_next[i].m_pos);
                m_next[i].m_pos[mapping(i)] = i;
            }
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](uint32_t i) { return *(first + i); });
        }
        /**
         * @brief 查询在原序列中，下标严格大于 `last_pos` 的范围内出现的第一个 `elem` 的下标。
         */
        uint32_t next(uint32_t last_pos, uint32_t elem) const { return m_next[last_pos + 1].m_pos[elem]; }
        /**
         * @brief 判断是否包含子序列
         * @param first, last 子序列的迭代器范围
         * @note 时间复杂度 $O(last - first)$
         */
        template <typename Iterator>
        bool contains(Iterator first, Iterator last) const {
            uint32_t last_pos = -1, p = m_next.size() - 2 - (last - first);
            for (auto it = first; it != last; ++it) {
                last_pos = next(last_pos, *it);
                if (last_pos > p++) return false;
            }
            return true;
        }
    };
}

#endif
#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 马拉车算法
 */
namespace OY {
    template <bool Even = true, bool Odd = true>
    struct Manacher {
        uint32_t m_length;
        std::vector<uint32_t> m_odd_arm, m_even_arm;
        Manacher() = default;
        template <typename InitMapping>
        Manacher(uint32_t length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        Manacher(Iterator first, Iterator last) { reset(first, last); }
        template <typename Sequence>
        Manacher(const Sequence &seq) : Manacher(seq.begin(), seq.end()) {}
        template <typename InitMapping>
        void resize(uint32_t length, InitMapping mapping) {
            m_length = length;
            if constexpr (Odd) {
                m_odd_arm.clear(), m_odd_arm.reserve(m_length);
                for (int l = -1, r = -1, i = 0; i != m_length; i++) {
                    int k = i <= r ? std::min<int>(m_odd_arm[l + r - i], r - i) + 1 : 1;
                    while (k <= i && k < m_length - i && mapping(i - k) == mapping(i + k)) k++;
                    m_odd_arm.push_back(k - 1);
                    if (i + k - 1 > r) l = i - k + 1, r = i + k - 1;
                }
            }
            if constexpr (Even) {
                m_even_arm.clear(), m_even_arm.reserve(m_length);
                for (int l = 0, r = -1, i = 0; i != m_length; i++) {
                    int k = i <= r ? std::min<int>(m_even_arm[l + r - i + 1], r - i + 1) + 1 : 1;
                    while (k <= i && k < m_length - i + 1 && mapping(i - k) == mapping(i + k - 1)) k++;
                    m_even_arm.push_back(k - 1);
                    if (i + k - 2 > r) l = i - k + 1, r = i + k - 2;
                }
            }
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](uint32_t i) { return *(first + i); });
        }
        /**
         * @brief 询问以 center 为中心的回文串的最大长度
         * @note 若返回值为 `x` ，则从 `center` 扩展所得的最长对称子串长度为 `x * 2 + 1` 。
         */
        uint32_t query_max_odd(uint32_t center) const {
            static_assert(Odd, "Odd Must Be True");
            return m_odd_arm[center];
        }
        /**
         * @brief 询问以 center_l 和 center_r 为中心的回文串的最大长度
         * @note 需要满足 `center_l + 1 == center_r`
         *       若返回值为 `x` ，则从 `center_l` 和 `center_r` 扩展所得的最长对称子串长度为 `x * 2` 。
         */
        uint32_t query_max_even(uint32_t center_l, uint32_t center_r) const {
            static_assert(Even, "Even Must Be True");
            return m_even_arm[center_r];
        }
        /**
         * @brief 查询区间 `[left, right)` 是否是回文串
         */
        bool query(uint32_t left, uint32_t right) const {
            right--;
            if ((left ^ right) & 1) {
                return m_even_arm[(left + right + 1) >> 1] > (right - left) >> 1;
            } else {
                return m_odd_arm[(left + right) >> 1] >= (right - left) >> 1;
            }
        }
    };
}
#ifndef __OY_ZALGORITHM__
#define __OY_ZALGORITHM__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>
#include <string>

/**
 * @brief Z Algorithm
 * @example OY::ZAlgorithm_string Z(std::string s);
 * @note Z[i] 表示模式串 s 和 s[i:]（即以 s[i] 开头的后缀）的最长公共前缀（LCP）的长度。
 *       特别地，Z[0] = 0。
 */
namespace OY {
    template <typename Sequence>
    struct ZAlgorithm {
        using value_type = typename Sequence::value_type;
        Sequence m_pattern;
        std::vector<uint32_t> m_z;
        ZAlgorithm() = default;
        template <typename InitMapping>
        ZAlgorithm(uint32_t length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        ZAlgorithm(Iterator first, Iterator last) { reset(first, last); }
        ZAlgorithm(const Sequence &seq) : ZAlgorithm(seq.begin(), seq.end()) {}
        template <typename InitMapping>
        void resize(uint32_t length, InitMapping mapping) {
            m_pattern.reserve(length);
            for (uint32_t i = 0; i != length; i++) m_pattern.push_back(mapping(i));
            m_z.clear(), m_z.reserve(length), m_z.push_back(0);
            for (uint32_t l = 0, r = 0, i = 1; i != length; i++) m_z.push_back(adjust(l, r, i, length, m_pattern));
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            m_pattern.assign(first, last);
            const uint32_t length = last - first;
            m_z.clear(), m_z.reserve(length), m_z.push_back(0);
            for (uint32_t l = 0, r = 0, i = 1; i != length; i++) m_z.push_back(adjust(l, r, i, length, m_pattern));
        }
        /**
         * @brief 查询是否被给定序列包含，返回文本串中第一次出现模式串的下标
         */
        template <typename Iterator>
        uint32_t contained_by(Iterator first, Iterator last) const {
            if (m_z.empty()) return 0;
            for (uint32_t length = last - first, l = -1, r = -1, i = 0; i != length; i++)
                if (adjust(l, r, i, length, first) == m_pattern.size()) return i;
            return -1;
        }
        /**
         * @brief 查询 Z 函数值
         */
        uint32_t query_Z(uint32_t i) const { return m_z[i]; }
        template <typename Ptr, typename Iter>
        uint32_t adjust(Ptr &l, Ptr &r, Ptr i, const uint32_t &length, const Iter &iter) const {
            if (~r && i <= r && m_z[i - l] < r - i + 1)
                return m_z[i - l];
            else if (iter[i] != m_pattern[0])
                return 0;
            l = i;
            if (!~r || r < i) r = i;
            while (r + 1 < length && iter[r + 1] == m_pattern[r + 1 - i]) r++;
            return r - i + 1;
        }
    };
    using ZAlgorithm_string = ZAlgorithm<std::string>;
    template <typename ValueType>
    using ZAlgorithm_vector = ZAlgorithm<std::vector<ValueType>>;
}

#endif
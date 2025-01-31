#ifndef __OY_KMP__
#define __OY_KMP__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>
#include <string>

/**
 * @brief KMP
 * @example OY::KMP_string kmp(std::string s);
 * @example OY::KMP_vector<int> kmp(std::vector<int> v);
 * @note 前缀函数 `pi[i]` 表示模式串 `s[0~i]` 的最长真前后缀长度，也就是前后缀子串必须是真子串；
 *       特别地，规定 `pi[0] = 0`。
 * @note border：一个字符串的 border 是它的一个真子串，并且这个真子串是它的前缀和后缀。
 * @note 字符串存在长度为 `k` 的 border，则 `|s| - k` 是它的周期。
 */
namespace OY {
    template <typename Sequence>
    struct KMP {
        using value_type = typename Sequence::value_type;
        using size_type = uint32_t;
        Sequence m_seq;
        std::vector<size_type> m_pi;
        void _init() { m_seq.push_back({}), m_pi.push_back(0); }
        KMP() { clear(); }
        template <typename InitMapping>
        KMP(size_type length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        KMP(Iterator first, Iterator last) { reset(first, last); }
        KMP(const Sequence &seq) : KMP(seq.begin(), seq.end()) {}
        template <typename InitMapping>
        void resize(size_type length, InitMapping mapping) {
            reserve(length);
            for (size_type i = 0; i != length; i++) push_back(mapping(i));
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](size_type i) { return *(first + i); });
        }
        void reserve(size_type length) { clear(), m_seq.reserve(length), m_pi.reserve(length); }
        void clear() { m_seq.clear(), m_pi.clear(), _init(); }
        /**
         * @brief 在模式串末尾添加一个元素
         */
        void push_back(const value_type &elem) {
            m_seq.push_back(elem);
            if (size() > 1) {
                size_type pi = jump(m_pi.back(), elem);
                m_pi.push_back(pi + (m_seq[pi + 1] == elem));
            } else
                m_pi.push_back(0);
        }
        /**
         * @brief 移除模式串末尾的一个元素
         */
        void pop_back() { m_seq.pop_back(), m_pi.pop_back(); }
        /**
         * @brief 查询模式串的长度
         */
        size_type size() const { return m_seq.size() - 1; }
        /**
         * @brief 查询前缀函数值的失配转移值，即找到可以接 `elem` 元素的 `pi` 值
         * @note 如果 `elem` 元素可以接上，即 `s[pi] == elem`，则返回 `pi`，否则返回 `0`
         * @note 注意如果 `pi` 已经等于 `size()`，也会继续跳转，找到可以接 `elem` 元素的 `pi` 值
         */
        size_type jump(size_type last_pi, const value_type &elem) const {
            size_type len = last_pi;
            while (len && (len == size() || m_seq[len + 1] != elem)) len = m_pi[len];
            return len;
        }
        /**
         * @brief 查询当前模式串 `s` 是否被某文本序列 `t` 包含
         * @return 返回包含的起始位置，即 `t.substr(pos, size()) == s`，
         *         如果未包含，则返回 `-1`
         */
        template <typename Iterator>
        size_type contained_by(Iterator first, Iterator last) const {
            if (!size()) return 0;
            size_type len = 0;
            for (auto it = first; it != last; ++it) {
                const value_type &elem = *it;
                while (len && m_seq[len + 1] != elem) len = m_pi[len];
                if (m_seq[len + 1] == elem) len++;
                if (len == size()) return (it - first) - len + 1;
            }
            return -1;
        }
        /**
         * @brief 对某个位置的所有 border 调用回调
         * @param call 回调函数，参数为 `call(p)`
         * @note 串 `s` 的所有 border 的长度分别为 `m_pi[n], m_pi[m_pi[n]], ...`
         */
        template <typename Callback>
        void do_for_each_border(size_type init_border, Callback &&call) {
            size_type pi = init_border;
            while (pi) call(pi), pi = query_Pi(pi - 1);
        }
        /**
         * @brief 询问模式串相应位置的前缀函数值，即长度为 `i + 1` 的前缀的最长 border 长度
         */
        size_type query_Pi(size_type i) const { return m_pi[i + 1]; }
    };
    using KMP_string = KMP<std::string>;
    template <typename ValueType>
    using KMP_vector = KMP<std::vector<ValueType>>;
}

#endif
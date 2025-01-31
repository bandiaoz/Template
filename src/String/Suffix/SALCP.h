#ifndef __OY_SALCP__
#define __OY_SALCP__

#include "src/DataStruct/Segtree/SqrtTree.h"
#include "src/String/Suffix/SuffixArray.h"

/**
 * @brief 最长公共前缀助手-后缀数组版
 * @tparam MAX_LEN 字符集大小
 * @example OY::make_SA_LCP<MAX_LEN>(s);
 */
namespace OY {
    namespace SALCP {
        using size_type = uint32_t;
        template <size_type MAX_LEN>
        struct LCP {
            size_type m_length;
            SA::SuffixArray<true, true, MAX_LEN> m_table;
            /**
             * @brief 维护 height 数组的区间最小值
             */
            SqrtMinTable<size_type, SQRT::RandomController<>, 15> m_inner_table;
            template <typename Iterator>
            LCP(Iterator first, Iterator last) : m_length(last - first), m_table(first, last), m_inner_table(m_length, [&](size_type i) { return m_table.query_height(i); }) {}
            LCP(const std::vector<int> &seq) : LCP(seq.begin(), seq.end()) {}
            LCP(const std::string &seq) : LCP(seq.begin(), seq.end()) {}
            /**
             * @brief 查询子串最长公共前缀 lcp
             * @param a 第一个子串的起始位置
             * @param b 第二个子串的起始位置
             * @param limit 最长公共前缀的最大长度，`limit` 为可能的子串长度
             * @note 需要保证 `std::max(a, b) + limit <= m_length`
             */
            size_type lcp(size_type a, size_type b, size_type limit) const {
                if (a == b) return limit;
                size_type ra = m_table.query_rank(a), rb = m_table.query_rank(b);
                if (ra > rb) std::swap(ra, rb);
                return std::min<size_type>(limit, m_inner_table.query(ra + 1, rb + 1));
            }
            /**
             * @brief 查询子串最长公共后缀 lcs
             * @param a 第一个子串的结束位置
             * @param b 第二个子串的结束位置
             * @param limit 最长公共后缀的最大长度，`limit` 为可能的子串长度
             * @note 需要保证 `std::min(a, b) + 1 >= limit`
             */
            size_type lcs(size_type a, size_type b, size_type limit) const {
                if (a == b) return a + 1;
                size_type low = 0, high = limit;
                while (low < high) {
                    size_type mid = (low + high + 1) / 2;
                    size_type ra = m_table.query_rank(a - mid + 1), rb = m_table.query_rank(b - mid + 1);
                    if (ra > rb) std::swap(ra, rb);
                    if (m_inner_table.query(ra + 1, rb + 1) >= mid)
                        low = mid;
                    else
                        high = mid - 1;
                }
                return low;
            }
            /**
             * @brief 查询子串最长公共前缀 lcp
             * @param a 第一个子串的起始位置
             * @param b 第二个子串的起始位置
             */
            size_type lcp(size_type a, size_type b) const {
                if (a == b) return m_length - a;
                return lcp(a, b, m_length - std::max(a, b));
            }
            /**
             * @brief 查询子串最长公共后缀 lcs
             * @param a 第一个子串的结束位置
             * @param b 第二个子串的结束位置
             */
            size_type lcs(size_type a, size_type b) const {
                if (a == b) return a + 1;
                return lcs(a, b, std::min(a, b) + 1);
            }
            /**
             * @brief 比较子串 `[l1, r1)` 和 `[l2, r2)` 的大小
             * @return 两个子串的比较结果可能为小于、等于、大于，分别对应返回值 `-1`、`0`、`1` 。
             */
            int compare(size_type l1, size_type r1, size_type l2, size_type r2) const {
                r1--, r2--;
                if (l1 == l2) return r1 < r2 ? -1 : (r1 == r2 ? 0 : 1);
                size_type len1 = r1 - l1 + 1, len2 = r2 - l2 + 1, ra = m_table.query_rank(l1), rb = m_table.query_rank(l2);
                if (ra < rb)
                    if (len1 < len2 || m_inner_table.query(ra + 1, rb + 1) < len2)
                        return -1;
                    else
                        return len1 > len2;
                else if (len2 < len1 || m_inner_table.query(rb + 1, ra + 1) < len1)
                    return 1;
                else
                    return len1 < len2 ? -1 : 0;
            }
        };
    }
    template <SALCP::size_type MAX_LEN, typename TableType = SALCP::LCP<MAX_LEN>>
    auto make_SA_LCP(const std::vector<int> &seq) -> TableType { return TableType(seq.begin(), seq.end()); }
    template <SALCP::size_type MAX_LEN, typename TableType = SALCP::LCP<MAX_LEN>>
    auto make_SA_LCP(const std::string &seq) -> TableType { return TableType(seq.begin(), seq.end()); }
    template <SALCP::size_type MAX_LEN, typename ValueType, typename TableType = SALCP::LCP<MAX_LEN>>
    auto make_SA_LCP(ValueType *first, ValueType *last) -> TableType { return TableType(first, last); }
}

#endif
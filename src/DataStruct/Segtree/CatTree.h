#ifndef __OY_CATTREE__
#define __OY_CATTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

/**
 * @brief 猫树，可以看做不支持修改的线段树
 */
namespace OY {
    namespace CAT {
        using size_type = uint32_t;
        template <typename Tp, typename Operation>
        struct BaseSemiGroup {
            using value_type = Tp;
            static value_type op(const value_type &x, const value_type &y) { return Operation()(x, y); }
        };
        template <typename Tp, typename Compare>
        struct ChoiceByCompare {
            Tp operator()(const Tp &x, const Tp &y) const { return Compare()(x, y) ? y : x; }
        };
        template <typename Tp, Tp (*Fp)(Tp, Tp)>
        struct FpTransfer {
            Tp operator()(const Tp &x, const Tp &y) const { return Fp(x, y); }
        };
        template <typename SemiGroup, size_t MAX_LEVEL = 30>
        class Table {
        public:
            using group = SemiGroup;
            using value_type = typename group::value_type;
        private:
            std::vector<value_type> m_sub[MAX_LEVEL];
            size_type m_size, m_depth;
            void _update(size_type i) {
                auto sub = m_sub[0].data();
                for (size_type j = 1, k = 4; j != m_depth; j++, k <<= 1) {
                    auto cur = m_sub[j].data();
                    size_type l = i & -(1 << (j + 1));
                    if (i >> j & 1) {
                        size_type j = i, end = std::min(l + k, m_size);
                        cur[j] = (j == l + (k >> 1)) ? sub[j] : group::op(cur[j - 1], sub[j]);
                        while (++j != end) cur[j] = group::op(cur[j - 1], sub[j]);
                    } else {
                        if (m_size <= l + k / 2) continue;
                        size_type j = i + 1;
                        cur[j - 1] = (j == l + (k >> 1)) ? sub[j - 1] : group::op(sub[j - 1], cur[j]);
                        while (--j != l) cur[j - 1] = group::op(sub[j - 1], cur[j]);
                    }
                }
            }
        public:
            Table() = default;
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                for (size_type i = 0; i != m_depth; i++) m_sub[i].resize(m_size);
                auto sub = m_sub[0].data();
                for (size_type i = 0; i != m_size; i++) sub[i] = mapping(i);
                for (size_type j = 1, k = 4, l; j != m_depth; j++, k <<= 1) {
                    auto cur = m_sub[j].data();
                    for (l = 0; l + k <= m_size; l += k) {
                        size_type i = l + (k >> 1);
                        cur[i - 1] = sub[i - 1];
                        while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                        i = l + (k >> 1);
                        cur[i] = sub[i];
                        while (++i != l + k) cur[i] = group::op(cur[i - 1], sub[i]);
                    }
                    if (l != m_size && (l + (k >> 1) < m_size)) {
                        size_type i = l + (k >> 1);
                        cur[i - 1] = sub[i - 1];
                        while (--i != l) cur[i - 1] = group::op(sub[i - 1], cur[i]);
                        i = l + (k >> 1);
                        cur[i] = sub[i];
                        while (++i != m_size) cur[i] = group::op(cur[i - 1], sub[i]);
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_size; }
            /**
             * @brief 单点修改，将第 `i` 个元素修改为 `val`
             * @note 复杂度为 `O(n)`
             */
            void modify(size_type i, value_type val) { m_sub[0][i] = val, _update(i); }
            /**
             * @brief 单点查询，查询第 `i` 个元素
             */
            value_type query(size_type i) const { return m_sub[0][i]; }
            /**
             * @brief 区间查询，查询区间 `[left, right)` 的元素
             */
            value_type query(size_type left, size_type right) const {
                right--;
                if (left == right) return m_sub[0][left];
                size_type d = std::bit_width(left ^ right) - 1;
                return group::op(m_sub[d][left], m_sub[d][right]);
            }
            /**
             * @brief 询问所有元素
             */
            value_type query_all() const { return query(0, m_size); }
            /**
             * @brief 树上二分查询右边界
             * @note 假设本函数返回 `r` ，则表示，对于 `i∈[left, r]`  ，均有 `judge(query(left, i))` 为真。而当 `i>r` 时，有 `judge(query(left, i))` 为假。显然，`r` 的最大值为 `m_size-1` 。
             *       如果从 `left` 开始，即使长度为一的区间也不能满足判断条件，那么返回 `left-1`  。所以 `r` 的最小值为 `left-1` 。
             */
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left];
                if (!judge(val)) return left - 1;
                if (++left == m_size) return left - 1;
                size_type d = std::bit_width(left ^ (m_size - 1));
                while (d && left < m_size) {
                    size_type split = (left & -(1 << (d - 1))) | (1 << (d - 1));
                    if (m_size <= split)
                        while (--d && (left >> (d - 1) & 1)) {}
                    else {
                        value_type a = group::op(val, m_sub[d - 1][left]);
                        if (judge(a))
                            val = a, --d, left = split;
                        else
                            while (--d && (left >> (d - 1) & 1)) {}
                    }
                }
                if (left < m_size && judge(group::op(val, m_sub[0][left]))) left++;
                return std::min(left, m_size) - 1;
            }
            /**
             * @brief 树上二分查询左边界
             * @note 假设本函数返回 `l` ，则表示，对于 `i∈[l, right]`  ，均有 `judge(query(i, right))` 为真。而当 `i<l` 时，有 `judge(query(i, right))` 为假。显然，`l` 的最小值为 `0` 。
             *       如果从 `right` 开始往左走，即使长度为一的区间也不能满足判断条件，那么返回 `right+1`  。所以 `l` 的最大值为 `right+1` 。
             */
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right];
                if (!judge(val)) return right + 1;
                if (!right--) return right + 1;
                size_type d = std::bit_width(right);
                while (d) {
                    value_type a = group::op(m_sub[d - 1][right], val);
                    if (judge(a))
                        val = a, --d, right = (right & -(1 << d)) - 1;
                    else
                        while (--d && !(right >> (d - 1) & 1)) {}
                }
                if (!(right & 1) && judge(group::op(m_sub[0][right], val))) right--;
                return right + 1;
            }
        };
        template <typename Ostream, typename SemiGroup, size_t MAX_LEVEL>
        Ostream &operator<<(Ostream &out, const Table<SemiGroup, MAX_LEVEL> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    /**
     * @brief 根据长度和 op 函数构造猫树，可以使用 mapping 函数映射初始值
     * @tparam Tp 猫树节点类型
     */
    template <typename Tp, size_t MAX_LEVEL = 30, typename Operation, typename InitMapping, typename TreeType = CAT::Table<CAT::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(CAT::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    /**
     * @brief 根据迭代器范围和 op 函数构造猫树
     */
    template <size_t MAX_LEVEL = 30, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = CAT::Table<CAT::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_CatTree(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatMaxTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::ChoiceByCompare<Tp, std::less<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatMinTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::ChoiceByCompare<Tp, std::greater<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatGcdTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::FpTransfer<Tp, std::gcd<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatLcmTable = CAT::Table<CAT::BaseSemiGroup<Tp, CAT::FpTransfer<Tp, std::lcm<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatBitAndTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::bit_and<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatBitOrTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::bit_or<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatBitXorTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::bit_xor<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using CatSumTable = CAT::Table<CAT::BaseSemiGroup<Tp, std::plus<Tp>>, MAX_LEVEL>;
}

#endif
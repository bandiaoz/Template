#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

/**
 * @brief Sparse Table
 */
namespace OY {
    namespace ST {
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
            size_type m_size;
            void _update(size_type idx) {
                size_type depth = std::bit_width(m_size - 1);
                for (size_type i = 1; i < depth; i++) {
                    auto pre = m_sub[i - 1].data(), cur = m_sub[i].data();
                    for (size_type j1 = idx < 1 << i ? 0 : (idx - (1 << i) + 1), j2 = j1 + (1 << (i - 1)), k = std::min(idx + 1, m_size - (1 << i) + 1); j1 != k; j1++, j2++) cur[j1] = group::op(pre[j1], pre[j2]);
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
                size_type depth = m_size == 1 ? 1 : std::bit_width(m_size - 1);
                for (size_type i = 0; i != depth; i++) m_sub[i].resize(m_size - (1 << i) + 1);
                auto sub = m_sub[0].data();
                for (size_type i = 0; i != m_size; i++) sub[i] = mapping(i);
                for (size_type i = 1; i != depth; i++) {
                    auto pre = m_sub[i - 1].data(), cur = m_sub[i].data();
                    for (size_type j1 = 0, j2 = 1 << (i - 1), k = m_size - (1 << i) + 1; j1 != k; j1++, j2++) cur[j1] = group::op(pre[j1], pre[j2]);
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_size; }
            /**
             * @brief 单点修改，将第 `i` 个元素修改为 `val`
             */
            void modify(size_type i, value_type val) { m_sub[0][i] = val, _update(i); }
            /**
             * @brief 单点查询，返回第 `i` 个元素的值
             */
            value_type query(size_type i) const { return m_sub[0][i]; }
            /**
             * @brief 区间查询，返回 `[left, right)` 开区间内的值
             */
            value_type query(size_type left, size_type right) const {
                right--;
                size_type d = std::bit_width((right - left) >> 1);
                return group::op(m_sub[d][left], m_sub[d][right - (1 << d) + 1]);
            }
            /**
             * @brief 查询所有元素的值
             */
            value_type query_all() const { return query(0, m_size); }
            /**
             * @brief 假设本函数返回 `r` ，则表示，对于 `i∈[left, r]`，均有 `judge(query(left, i + 1))` 为真。而当 `i>r` 时，有 `judge(query(left, i + 1))` 为假。显然，`r` 的最大值为 `m_size-1` 。
             *        
             *        如果从 `left` 开始，即使长度为一的区间也不能满足判断条件，那么返回 `left-1`。所以 `r` 的最小值为 `left-1` 。
             */
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                value_type val = m_sub[0][left];
                if (!judge(val)) return left - 1;
                size_type d = std::bit_width(m_size - left - 1);
                if (d) {
                    value_type a = group::op(val, m_sub[d - 1][left + 1]);
                    if (judge(a))
                        val = a, d = std::bit_width(m_size - left - 1 - (1 << (d - 1))), left = m_size - (1 << d);
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = group::op(val, m_sub[d - 1][left + 1]);
                    if (judge(a)) val = a, left += 1 << (d - 1);
                }
                return left;
            }
            /**
             * @brief 假设本函数返回 `l` ，则表示，对于 `i∈[l, right]`  ，均有 `judge(query(i, right + 1))` 为真。而当 `i<l` 时，有 `judge(query(i, right + 1))` 为假。显然，`l` 的最小值为 `0` 。
             *        
             *        如果从 `right` 开始往左走，即使长度为一的区间也不能满足判断条件，那么返回 `right+1`。所以 `l` 的最大值为 `right+1` 。
             */
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                value_type val = m_sub[0][right];
                if (!judge(val)) return right + 1;
                size_type d = std::bit_width(right);
                if (d) {
                    value_type a = group::op(m_sub[d - 1][right - (1 << (d - 1))], val);
                    if (judge(a))
                        val = a, d = std::bit_width(right - (1 << (d - 1))), right = (1 << d) - 1;
                    else
                        d--;
                }
                for (; d; d--) {
                    value_type a = group::op(m_sub[d - 1][right - (1 << (d - 1))], val);
                    if (judge(a)) val = a, right -= 1 << (d - 1);
                }
                return right;
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
    template <typename Tp, size_t MAX_LEVEL = 30, typename Operation, typename InitMapping, typename TreeType = ST::Table<ST::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_STTable(ST::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <size_t MAX_LEVEL = 30, typename Iterator, typename Operation, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename TreeType = ST::Table<ST::BaseSemiGroup<Tp, Operation>, MAX_LEVEL>>
    auto make_STTable(Iterator first, Iterator last, Operation op) -> TreeType { return TreeType(first, last); }
    template <typename Tp, size_t MAX_LEVEL = 30>
    using STMaxTable = ST::Table<ST::BaseSemiGroup<Tp, ST::ChoiceByCompare<Tp, std::less<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using STMinTable = ST::Table<ST::BaseSemiGroup<Tp, ST::ChoiceByCompare<Tp, std::greater<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using STGcdTable = ST::Table<ST::BaseSemiGroup<Tp, ST::FpTransfer<Tp, std::gcd<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using STLcmTable = ST::Table<ST::BaseSemiGroup<Tp, ST::FpTransfer<Tp, std::lcm<Tp>>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using STBitAndTable = ST::Table<ST::BaseSemiGroup<Tp, std::bit_and<Tp>>, MAX_LEVEL>;
    template <typename Tp, size_t MAX_LEVEL = 30>
    using STBitOrTable = ST::Table<ST::BaseSemiGroup<Tp, std::bit_or<Tp>>, MAX_LEVEL>;
}
#ifndef __OY_MONOZKWTREE__
#define __OY_MONOZKWTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

#if __has_include(<bit>)
#include <bit>
#else
#include "src/Misc/std_bit.h"
#endif

/**
 * @brief MonoZkw 线段树，只支持单点修改的特化
 */
namespace OY {
    namespace MONOZKW {
        using size_type = uint32_t;
        template <typename Tp, Tp Identity, typename Operation>
        struct BaseMonoid {
            using value_type = Tp;
            static constexpr value_type identity() { return Identity; }
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
#ifdef __cpp_lib_void_t
        template <typename... Tp>
        using void_t = std::void_t<Tp...>;
#else
        template <typename... Tp>
        struct make_void {
            using type = void;
        };
        template <typename... Tp>
        using void_t = typename make_void<Tp...>::type;
#endif
        template <typename Tp, typename = void>
        struct Has_Not_Equal : std::false_type {};
        template <typename Tp>
        struct Has_Not_Equal<Tp, void_t<decltype(std::declval<Tp>() != std::declval<Tp>())>> : std::true_type {};
        template <typename Monoid>
        class Tree {
        public:
            using group = Monoid;
            using value_type = typename group::value_type;
        private:
            size_type m_size, m_cap;
            std::vector<value_type> m_sub;
            static bool _check_pushup(value_type &old, value_type val) {
                if constexpr (Has_Not_Equal<value_type>::value)
                    return old != val ? old = val, true : false;
                else
                    return old = val, true;
            }
        public:
            Tree(size_type length = 0) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            size_type size() const { return m_size; }
            void resize(size_type length) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_sub.assign(m_cap * 2, group::identity());
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_cap = 1 << std::max<size_type>(1, std::bit_width(m_size - 1));
                m_sub.resize(m_cap * 2);
                auto sub = m_sub.data();
                for (size_type i = 0; i != m_size; i++) sub[m_cap + i] = mapping(i);
                std::fill_n(sub + m_cap + m_size, m_cap - m_size, group::identity());
                for (size_type len = m_cap / 2, w = 2; len; len >>= 1, w <<= 1)
                    for (size_type i = len; i != len << 1; i++) sub[i] = group::op(sub[i * 2], sub[i * 2 + 1]);
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            /**
             * @brief 单点修改，将第 `i` 个元素修改为 `val`
             */
            void modify(size_type i, value_type val) {
                auto sub = m_sub.data();
                sub[i += m_cap] = val;
                while ((i >>= 1) && _check_pushup(sub[i], group::op(sub[i * 2], sub[i * 2 + 1]))) {}
            }
            /**
             * @brief 单点增值，将第 `i` 个元素增加 `inc`
             */
            void add(size_type i, value_type inc) { modify(i, group::op(inc, query(i))); }
            /**
             * @brief 单点询问，返回第 `i` 个元素的值
             */
            value_type query(size_type i) const { return m_sub[m_cap + i]; }
            /**
             * @brief 区间查询，返回区间 `[left, right)` 的聚合值
             */
            value_type query(size_type left, size_type right) const {
                right--;
                if (left == right) return query(left);
                auto sub = m_sub.data();
                value_type res = group::identity();
                right++;
                if (left)
                    while (true) {
                        size_type j = std::countr_zero(left), left2 = left + (size_type(1) << j);
                        if (left2 > right) break;
                        res = group::op(res, sub[(m_cap + left) >> j]);
                        left = left2;
                    }
                while (left < right) {
                    size_type j = std::bit_width(left ^ right), left2 = left + (size_type(1) << (j - 1));
                    res = group::op(res, sub[(m_cap + left) >> (j - 1)]);
                    left = left2;
                }
                return res;
            }
            /**
             * @brief 区间查询，返回所有元素的聚合值
             */
            value_type query_all() const { return m_sub[1]; }
            /**
             * @brief 树上二分查询右边界
             * @note 假设本函数返回 `r` ，则表示，对于 `i∈[left, r]`  ，均有 `judge(query(left, i))` 为真。而当 `i>r` 时，有 `judge(query(left, i))` 为假。显然，`r` 的最大值为 `m_size-1` 。
             *       如果从 `left` 开始，即使长度为一的区间也不能满足判断条件，那么返回 `left-1`  。所以 `r` 的最小值为 `left-1` 。
             */
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const {
                auto sub = m_sub.data();
                value_type val = group::identity();
                size_type cur = left, j = -1;
                if (cur)
                    while (true) {
                        j = std::countr_zero(cur);
                        size_type next = cur + (size_type(1) << j);
                        if (next > m_size) break;
                        value_type a = group::op(val, sub[(m_cap + cur) >> j]);
                        if (!judge(a)) break;
                        val = a, cur = next;
                    }
                if (cur != m_size) {
                    j = std::min<size_type>(j, std::bit_width(m_size - cur));
                    while (~--j) {
                        size_type next = cur + (size_type(1) << j);
                        if (next > m_size) continue;
                        value_type a = group::op(val, sub[(m_cap + cur) >> j]);
                        if (!judge(a)) continue;
                        val = a, cur = next;
                    }
                }
                return cur - 1;
            }
            /**
             * @brief 树上二分查询左边界
             * @note 假设本函数返回 `l` ，则表示，对于 `i∈[l, right]`  ，均有 `judge(query(i, right))` 为真。而当 `i<l` 时，有 `judge(query(i, right))` 为假。显然，`l` 的最小值为 `0` 。
             *       如果从 `right` 开始往左走，即使长度为一的区间也不能满足判断条件，那么返回 `right+1`  。所以 `l` 的最大值为 `right+1` 。
             */
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const {
                auto sub = m_sub.data();
                value_type val = group::identity();
                size_type cur = right, j = -1;
                do {
                    j = std::countr_zero(~cur);
                    size_type next = cur - (1 << j);
                    value_type a = group::op(sub[(m_cap + cur) >> j], val);
                    if (!judge(a)) break;
                    val = a, cur = next;
                } while (~cur);
                if (~cur) {
                    j = std::min<size_type>(j, std::bit_width(cur + 1));
                    while (~--j) {
                        size_type next = cur - (1 << j);
                        value_type a = group::op(sub[(m_cap + cur) >> j], val);
                        if (!judge(a)) continue;
                        val = a, cur = next;
                    }
                }
                return cur + 1;
            }
        };
        template <typename Ostream, typename Monoid>
        Ostream &operator<<(Ostream &out, const Tree<Monoid> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    /**
     * @brief 根据长度和 op 函数构造线段树，可以使用 mapping 函数映射初始值
     * @tparam Tp 线段树节点类型
     * @tparam Identity 线段树节点的单位元
     * @param length 线段树的长度
     * @param op 线段树的操作函数
     * @param mapping 映射函数
     */
    template <typename Tp, Tp Identity, typename Operation, typename InitMapping, typename TreeType = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, Identity, Operation>>>
    auto make_MonoZkw(MONOZKW::size_type length, Operation op, InitMapping mapping) -> TreeType { return TreeType(length, mapping); }
    template <typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MonoMaxTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, Minimum, MONOZKW::ChoiceByCompare<Tp, std::less<Tp>>>>;
    template <typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using MonoMinTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, Maximum, MONOZKW::ChoiceByCompare<Tp, std::greater<Tp>>>>;
    template <typename Tp>
    using MonoGcdTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, 0, MONOZKW::FpTransfer<Tp, std::gcd<Tp>>>>;
    template <typename Tp>
    using MonoLcmTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, 1, MONOZKW::FpTransfer<Tp, std::lcm<Tp>>>>;
    template <typename Tp, Tp OneMask = Tp(-1)>
    using MonoBitAndTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, OneMask, std::bit_and<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitOrTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, ZeroMask, std::bit_or<Tp>>>;
    template <typename Tp, Tp ZeroMask = 0>
    using MonoBitXorTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, ZeroMask, std::bit_xor<Tp>>>;
    template <typename Tp, Tp Zero = Tp()>
    using MonoSumTree = MONOZKW::Tree<MONOZKW::BaseMonoid<Tp, Zero, std::plus<Tp>>>;
}

#endif
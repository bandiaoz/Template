#ifndef __OY_CHTHOLLYZKWTREE__
#define __OY_CHTHOLLYZKWTREE__

#include "src/DataStruct/Segtree/AssignZkwTree.h"

/**
 * @brief 珂朵莉 `zkw` 线段树，底层为 `AssignZkwTree`
 * @example OY::ChthollyZkwTree<Tp> cho(length, init_val = Tp());
 * @note 其中 `Tp` 为同值区间的判断依据，需要维护额外信息可以继承 `BaseSegment`
 */
namespace OY {
    namespace ChthollyZkw {
        using size_type = ASZKW::size_type;
        template <typename Tp>
        struct BaseSegment {
            using value_type = Tp;
            Tp m_front, m_back;
            bool m_change;
            BaseSegment() = default;
            explicit BaseSegment(const Tp &val) : m_front(val), m_back(val), m_change(false) {}
            BaseSegment(const Tp &front, const Tp &back, bool change) : m_front(front), m_back(back), m_change(change) {}
            BaseSegment<Tp> operator+(const BaseSegment<Tp> &rhs) const { return BaseSegment<Tp>(m_front, rhs.m_back, m_change || rhs.m_change || m_back != rhs.m_front); }
            /**
             * @brief 判断区间是否为同值区间
             * @return 如果区间所有值都相同，则为 false
             */
            bool has_change() const { return m_change; }
            /**
             * @brief 获取区间任意一个点的值
             */
            Tp get() const { return m_front; }
        };
        template <typename Segment>
        struct ValLazySemiGroup {
            using value_type = Segment;
            static constexpr bool val_is_lazy = true;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
        };
        template <typename Segment>
        struct LazySemiGroup {
            using value_type = Segment;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
        };
        template <typename Segment>
        struct FastPowSemiGroup {
            using value_type = Segment;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type pow(const value_type &x, size_type n) { return x * n; }
        };
        template <typename ValueType>
        struct RangeCollector {
            struct range {
                size_type m_left, m_right;
                ValueType m_val;
            };
            std::vector<range> m_data;
            template <typename... Args>
            void operator()(Args &&...args) { m_data.push_back({std::forward<Args>(args)...}); }
        };
        template <typename SemiGroup, size_type BATCH = 1 << 17>
        class Tree {
        public:
            using group = SemiGroup;
            using segment = typename group::value_type;
            using value_type = typename segment::value_type;
            using inner_type = ASZKW::Tree<group, BATCH>;
        private:
            inner_type m_inner_tree;
            static segment _make_segment(const value_type &val) { return segment(val); }
        public:
            Tree() = default;
            Tree(size_type length, value_type init_val = value_type()) { resize(length, init_val); }
            /**
             * @brief 调整线段树大小，设定初始值
             * @param length 线段树大小
             * @param init_val 初始值
             */
            void resize(size_type length, value_type init_val = value_type()) { m_inner_tree.resize(length, _make_segment(init_val)); }
            size_type size() const { return m_inner_tree.size(); }
            /**
             * @brief 对区间 `[left, right)` 进行赋值
             */
            void modify(size_type left, size_type right, const value_type &val) { m_inner_tree.modify(left, right, _make_segment(val)); }
            /**
             * @brief 对节点 `i` 进行赋值
             */
            void modify(size_type i, const value_type &val) { m_inner_tree.modify(i, _make_segment(val)); }
            /**
             * @brief 查询节点 `i` 的值
             */
            value_type query(size_type i) const { return m_inner_tree.query(i).get(); }
            /**
             * @brief 查询区间 `[left, right)` 的值
             */
            segment query(size_type left, size_type right) const { return m_inner_tree.query(left, right); }
            /**
             * @brief 查询整个线段树的值
             */
            segment query_all() const { return m_inner_tree.query_all(); }
            /**
             * @brief 查询节点 `i` 所在的区间，返回 `[l, r)`
             */
            std::pair<size_type, size_type> belong(size_type i) const {
                size_type l = m_inner_tree.min_left(i, [&](const segment &seg) { return !seg.has_change(); });
                size_type r = m_inner_tree.max_right(i, [&](const segment &seg) { return !seg.has_change(); });
                return std::make_pair(l, r + 1);
            }
            /**
             * @brief 枚举区间 `[left, right)` 的所有区间
             * @param call 回调函数，参数为 `(left, right, val)`，表示区间 `[left, right)` 的值为 `val`
             */
            template <typename Callback>
            void enumerate(size_type left, size_type right, Callback &&call) {
                right--;
                for (size_type cur = left, next; cur != right + 1; cur = next) {
                    next = std::min(right, m_inner_tree.max_right(cur, [&](const segment &seg) { return !seg.has_change(); })) + 1;
                    call(cur, next, m_inner_tree.query(cur).get());
                }
            }
            /**
             * @brief 枚举整个线段树的所有区间
             * @param call 回调函数，参数为 `(left, right, val)`，表示区间 `[left, right)` 的值为 `val`
             */
            template <typename Callback>
            void enumerate_all(Callback &&call) {
                for (size_type cur = 0, next; cur != size(); cur = next) {
                    next = m_inner_tree.max_right(cur, [&](const segment &seg) { return !seg.has_change(); }) + 1;
                    call(cur, next, m_inner_tree.query(cur).get());
                }
            }
            /**
             * @brief 获取区间 `[left, right)` 的所有区间
             * @param left 左端点
             * @param right 右端点
             * @return 区间 `[left, right)` 的所有区间
             */
            std::vector<typename RangeCollector<value_type>::range> get_ranges(size_type left, size_type right) {
                RangeCollector<value_type> rc;
                enumerate(left, right, rc);
                return rc.m_data;
            }
            /**
             * @brief 获取整个线段树的所有区间
             * @return 整个线段树的所有区间
             */
            std::vector<typename RangeCollector<value_type>::range> get_ranges_all() {
                RangeCollector<value_type> rc;
                enumerate_all(rc);
                return rc.m_data;
            }
        };
        template <typename Ostream, typename SemiGroup, size_type BATCH>
        Ostream &operator<<(Ostream &out, const Tree<SemiGroup, BATCH> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyZkwTree = ChthollyZkw::Tree<ChthollyZkw::ValLazySemiGroup<ChthollyZkw::BaseSegment<Tp>>, BATCH>;
    template <typename Segment, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyValLazyZkwTree = ChthollyZkw::Tree<ChthollyZkw::ValLazySemiGroup<Segment>, BATCH>;
    template <typename Segment, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyLazyZkwTree = ChthollyZkw::Tree<ChthollyZkw::LazySemiGroup<Segment>, BATCH>;
    template <typename Segment, ChthollyZkw::size_type BATCH = 1 << 17>
    using ChthollyFastPowZkwTree = ChthollyZkw::Tree<ChthollyZkw::FastPowSemiGroup<Segment>, BATCH>;
}

#endif
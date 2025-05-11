#ifndef __OY_CHTHOLLYSEGTREE__
#define __OY_CHTHOLLYSEGTREE__

#include "src/DataStruct/Segtree/AssignSegTree.h"

/**
 * @brief 珂朵莉线段树，底层为 `AssignSegTree`
 * @example OY::VectorChthollySegTree<Tp, SizeType = uint64_t> cho(length, init_val = Tp());
 */
namespace OY {
    namespace ChthollySeg {
        using size_type = ASSEG::size_type;
        /**
         * @brief 节点信息，需要维护额外信息可以仿照 `BaseSegment` 重写 `Segment`
         * @tparam Tp 节点值类型，为同值区间的判断依据
         * @note 至少需要包含以下信息：
         *       1. 定义类型 `value_type`，作为单点的值类型
         *       2. 定义无参构造函数和从 `value_type` 转换而来的构造函数
         *       3. 定义加法运算，结果为两个相邻 `segment` 信息聚合的结果
         *       4. 定义成员函数 `has_change()` 。定义一个 `segment` 为同值区间，当且仅当该 `segment` 所表示的区间内的每个点的值都相等。同值区间的 `has_change` 返回 `false` ，非同值区间的 `has_change` 返回 `true`。
         *       5. 定义成员函数 `get()` ，表示区间任意一个点的值
         */
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
        /**
         * @brief 半群信息
         * @note 需要传递一个 `Segment` 类型，该类型需要满足 `BaseSegment` 的要求以外，加法运算要满足：如果两个同值区间的点值相等，则相加结果仍为其本身
         */
        template <typename Segment>
        struct ValLazySemiGroup {
            using value_type = Segment;
            static constexpr bool val_is_lazy = true;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
        };
        /**
         * @brief 半群信息
         * @note 需要传递一个 `Segment` 类型，满足 `BaseSegment` 的要求即可，无特殊要求。
         */
        template <typename Segment>
        struct LazySemiGroup {
            using value_type = Segment;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
        };
        /**
         * @brief 半群信息
         * @note 需要传递一个 `Segment` 类型，满足 `BaseSegment` 的要求以外，需要重载 `operator*` ，表示一个长度为 `1` 的 `segment` 乘以 `n` 后可以快速得到长度为 `n` 的同值区间。
         */
        template <typename Segment>
        struct FastPowSemiGroup {
            using value_type = Segment;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            template <typename SizeType>
            static value_type pow(const value_type &x, SizeType n) { return x * n; }
        };
        template <typename SizeType, typename ValueType>
        struct RangeCollector {
            std::vector<std::tuple<SizeType, SizeType, ValueType>> m_data;
            template <typename... Args>
            void operator()(Args &&...args) { m_data.emplace_back(std::forward<Args>(args)...); }
        };
        template <typename SemiGroup, typename SizeType = uint64_t, size_type BATCH = 1 << 17, template <typename> typename BufferType = VectorBufferWithoutCollect>
        class Tree {
        public:
            using group = SemiGroup;
            using segment = typename group::value_type;
            using value_type = typename segment::value_type;
            using inner_type = ASSEG::Tree<group, SizeType, BATCH, BufferType>;
            static void _reserve(size_type capacity) { inner_type::_reserve(capacity); }
        private:
            inner_type m_inner_tree;
            static segment _make_segment(const value_type &val) { return segment(val); }
        public:
            Tree() = default;
            Tree(SizeType length, value_type init_val = value_type()) { resize(length, init_val); }
            /**
             * @brief 调整线段树大小，设定初始值
             * @param length 线段树大小
             * @param init_val 初始值
             */
            void resize(SizeType length, value_type init_val = value_type()) { m_inner_tree.resize(length, _make_segment(init_val)); }
            SizeType size() const { return m_inner_tree.size(); }
            /**
             * @brief 对区间 `[left, right)` 进行赋值
             */
            void modify(SizeType left, SizeType right, const value_type &val) { m_inner_tree.modify(left, right, _make_segment(val)); }
            /**
             * @brief 对节点 `i` 进行赋值
             */
            void modify(SizeType i, const value_type &val) { m_inner_tree.modify(i, _make_segment(val)); }
            /**
             * @brief 查询节点 `i` 的值
             */
            value_type query(SizeType i) const { return m_inner_tree.query(i).get(); }
            /**
             * @brief 查询区间 `[left, right)` 的值
             */
            segment query(SizeType left, SizeType right) const { return m_inner_tree.query(left, right); }
            /**
             * @brief 查询整个线段树的值
             */
            segment query_all() const { return m_inner_tree.query_all(); }
            /**
             * @brief 查询节点 `i` 所在的区间，返回 `[l, r)`
             */
            std::pair<SizeType, SizeType> belong(SizeType i) const {
                SizeType l = m_inner_tree.min_left(i, [&](const segment &seg) { return !seg.has_change(); });
                SizeType r = m_inner_tree.max_right(i, [&](const segment &seg) { return !seg.has_change(); });
                return std::make_pair(l, r + 1);
            }
            /**
             * @brief 枚举区间 `[left, right)` 的所有区间
             * @param call 回调函数，参数为 `(left, right, val)`，表示区间 `[left, right)` 的值为 `val`
             */
            template <typename Callback>
            void enumerate(SizeType left, SizeType right, Callback &&call) {
                right--;
                for (SizeType cur = left, next; cur != right + 1; cur = next) {
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
                for (SizeType cur = 0, next; cur != size(); cur = next) {
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
            std::vector<std::tuple<SizeType, SizeType, value_type>> get_ranges(SizeType left, SizeType right) {
                RangeCollector<SizeType, value_type> rc;
                enumerate(left, right, rc);
                return rc.m_data;
            }
            /**
             * @brief 获取整个线段树的所有区间
             * @return 整个线段树的所有区间
             */
            std::vector<std::tuple<SizeType, SizeType, value_type>> get_ranges_all() {
                RangeCollector<SizeType, value_type> rc;
                enumerate_all(rc);
                return rc.m_data;
            }
        };
        template <typename Ostream, typename SemiGroup, typename SizeType, size_type BATCH, template <typename> typename BufferType>
        Ostream &operator<<(Ostream &out, const Tree<SemiGroup, SizeType, BATCH, BufferType> &x) {
            out << "[";
            for (SizeType i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        }
    }
    template <typename Tp, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollySegTree = ChthollySeg::Tree<ChthollySeg::ValLazySemiGroup<ChthollySeg::BaseSegment<Tp>>, SizeType, BATCH>;
    template <typename Segment, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollyValLazyTree = ChthollySeg::Tree<ChthollySeg::ValLazySemiGroup<Segment>, SizeType, BATCH>;
    template <typename Segment, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollyLazySegTree = ChthollySeg::Tree<ChthollySeg::LazySemiGroup<Segment>, SizeType, BATCH>;
    template <typename Segment, typename SizeType = uint64_t, ChthollySeg::size_type BATCH = 1 << 17>
    using VectorChthollyFastPowSegTree = ChthollySeg::Tree<ChthollySeg::FastPowSemiGroup<Segment>, SizeType, BATCH>;
}

#endif
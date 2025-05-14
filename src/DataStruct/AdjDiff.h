#ifndef __OY_ADJDIFF__
#define __OY_ADJDIFF__

#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 一维差分/前缀和表
 * @tparam Tp 数据类型
 * @tparam AutoSwitch 是否自动切换状态，默认为 true
 * @example OY::AdjSumTable<Tp> table(length);
 *          OY::AdjBitxorTable<Tp> table(length);
 * @note 交换群需要有幺元和逆元，且运算符需要满足结合律和交换律。
 */
namespace OY {
    namespace AdjDiff {
        using size_type = uint32_t;
        template <typename Tp>
        struct AddCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp inverse(const Tp &x) { return -x; }
        };
        template <typename Tp>
        struct BitxorCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp inverse(const Tp &x) { return x; }
        };
        template <typename CommutativeGroup, bool AutoSwitch = true>
        class Table {
        public:
            using group = CommutativeGroup;
            using value_type = typename group::value_type;
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE = 1,
                TABLE_VALUE = 2,
                TABLE_PRESUM = 3
            };
        private:
            mutable TableState m_state;
            mutable std::vector<value_type> m_v;
            void _auto_to_value() const {
                if constexpr (AutoSwitch) switch_to_value();
            }
            void _auto_to_difference() const {
                if constexpr (AutoSwitch) switch_to_difference();
            }
            void _auto_to_presum() const {
                if constexpr (AutoSwitch) switch_to_presum();
            }
            void _plus(size_type i, const value_type &inc) const { m_v[i] = group::op(m_v[i], inc); }
            void _minus(size_type i, const value_type &inc) const { m_v[i] = group::op(m_v[i], group::inverse(inc)); }
            value_type _get(size_type i) const { return ~i ? m_v[i] : group::identity(); }
            void _adjacent_difference() const {
                std::adjacent_difference(m_v.begin(), m_v.end(), m_v.begin(), [](const value_type &x, const value_type &y) { return group::op(x, group::inverse(y)); });
                m_state = TableState(m_state - 1);
            }
            void _partial_sum() const {
                std::partial_sum(m_v.begin(), m_v.end(), m_v.begin(), [](const value_type &x, const value_type &y) { return group::op(x, y); });
                m_state = TableState(m_state + 1);
            }
        public:
            Table() = default;
            Table(size_type length) { resize(length); }
            template <typename InitMapping>
            Table(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Table(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) { m_v.assign(length, group::identity()), m_state = TABLE_ANY; }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                m_v.resize(length);
                for (size_type i = 0; i != length; i++) m_v[i] = mapping(i);
                m_state = TABLE_VALUE;
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            size_type size() const { return m_v.size(); }
            /**
             * @brief 单点修改，将下标为 `i` 的值增加 `inc`
             */
            void add(size_type i, value_type inc) { _auto_to_value(), _plus(i, inc); }
            /**
             * @brief 单点修改，将下标为 `i` 的值修改为 `val`
             */
            void modify(size_type i, value_type val) { _auto_to_value(), m_v[i] = val; }
            /**
             * @brief 区间修改，将区间 `[left, right)` 的值增加 `inc`
             * @note 允许 `left = right`，此时不进行任何操作
             */
            void add(size_type left, size_type right, value_type inc) { right--, _auto_to_difference(), _plus(left, inc), (right + 1 != size() ? _minus(right + 1, inc) : void()); }
            /**
             * @brief 单点查询，返回下标为 `i` 的值
             */
            value_type query(size_type i) const { return _auto_to_value(), _get(i); }
            /**
             * @brief 区间查询，返回区间 `[left, right)` 的和
             * @note 允许 `left = right`，此时返回 `group::identity()`
             */
            value_type query(size_type left, size_type right) const { right--; return _auto_to_presum(), (left - 1 == right ? group::identity() : group::op(_get(right), group::inverse(_get(left - 1)))); }
            /**
             * @brief 区间查询，返回区间 `[0, n)` 的和
             */
            value_type query_all() const { return _auto_to_presum(), _get(size() - 1); }
            void switch_to_difference() const {
                if (m_state == TABLE_DIFFERENCE) return;
                if (m_state == TABLE_ANY) return (void)(m_state = TABLE_DIFFERENCE);
                if (m_state == TABLE_PRESUM) _adjacent_difference();
                if (m_state == TABLE_VALUE) _adjacent_difference();
            }
            void switch_to_value() const {
                if (m_state == TABLE_VALUE) return;
                if (m_state == TABLE_ANY) return (void)(m_state = TABLE_VALUE);
                if (m_state == TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TABLE_PRESUM) _adjacent_difference();
            }
            void switch_to_presum() const {
                if (m_state == TABLE_PRESUM) return;
                if (m_state == TABLE_ANY) return (void)(m_state = TABLE_PRESUM);
                if (m_state == TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TABLE_VALUE) _partial_sum();
            }
        };
        template <typename Ostream, typename CommutativeGroup, bool AutoSwitch>
        Ostream &operator<<(Ostream &out, const Table<CommutativeGroup, AutoSwitch> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.query(i);
            }
            return out << "]";
        };
    }
    template <typename Tp, bool AutoSwitch = true>
    using AdjSumTable = AdjDiff::Table<AdjDiff::AddCommutativeGroup<Tp>, AutoSwitch>;
    template <typename Tp, bool AutoSwitch = true>
    using AdjBitxorTable = AdjDiff::Table<AdjDiff::BitxorCommutativeGroup<Tp>, AutoSwitch>;
}

#endif
#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

/**
 * @brief 二维差分/前缀和表
 * @tparam Tp 数据类型
 * @tparam AutoSwitch 是否自动切换状态
 * @example OY::AdjDiff2D::Table<int, true> table(row, column);
 */
namespace OY {
    namespace AdjDiff2D {
        using size_type = uint32_t;
        template <typename Tp, bool AutoSwitch = true>
        class Table {
            enum TableState {
                TABLE_ANY = 0,
                TABLE_DIFFERENCE = 1,
                TABLE_VALUE = 2,
                TABLE_PRESUM = 3
            };
            size_type m_row, m_column;
            mutable TableState m_state;
            mutable std::vector<Tp> m_sum;
            void _plus(size_type i, size_type j, const Tp &inc) const {
                if (i != m_row && j != m_column) m_sum[i * m_column + j] += inc;
            }
            void _minus(size_type i, size_type j, const Tp &inc) const {
                if (i != m_row && j != m_column) m_sum[i * m_column + j] -= inc;
            }
            Tp _get(size_type i, size_type j) const { return ~i && ~j ? m_sum[i * m_column + j] : 0; }
            void _adjacent_difference() const {
                for (size_type i = m_row - 1; ~i; i--)
                    for (size_type j = m_column - 1; ~j; j--) 
                        _plus(i, j, _get(i - 1, j - 1) - _get(i - 1, j) - _get(i, j - 1));
                m_state = TableState(m_state - 1);
            }
            void _partial_sum() const {
                for (size_type i = 0; i != m_row; i++)
                    for (size_type j = 0; j != m_column; j++) 
                        _minus(i, j, _get(i - 1, j - 1) - _get(i - 1, j) - _get(i, j - 1));
                m_state = TableState(m_state + 1);
            }
        public:
            Table() = default;
            Table(size_type row, size_type column) { resize(row, column); }
            template <typename InitMapping>
            Table(size_type row, size_type column, InitMapping mapping) { resize(row, column, mapping); }
            void resize(size_type row, size_type column) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_sum.assign(m_row * m_column, {});
                m_state = TableState::TABLE_ANY;
            }
            template <typename InitMapping>
            void resize(size_type row, size_type column, InitMapping mapping) {
                if (!(m_row = row) || !(m_column = column)) return;
                m_sum.resize(m_row * m_column);
                for (size_type i = 0, k = 0; i != m_row; i++)
                    for (size_type j = 0; j != m_column; j++) m_sum[k++] = mapping(i, j);
                m_state = TableState::TABLE_VALUE;
            }
            size_type row() const { return m_row; }
            size_type column() const { return m_column; }
            /**
             * @brief 单点增值，将 `(i, j)` 的值增加 `inc`
             */
            void add(size_type i, size_type j, Tp inc) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, j, inc);
            }
            /**
             * @brief 单点修改，将 `(i, j)` 的值修改为 `val`
             */
            void modify(size_type i, size_type j, Tp val) {
                if constexpr (AutoSwitch) switch_to_value();
                _plus(i, j, val - _get(i, j));
            }
            /**
             * @brief 区间增值，将区间 `[r1, r2) x [c1, c2)` 的值增加 `inc`
             */
            void add(size_type r1, size_type r2, size_type c1, size_type c2, Tp inc) {
                r2--, c2--;
                if constexpr (AutoSwitch) switch_to_difference();
                _plus(r1, c1, inc), _minus(r1, c2 + 1, inc), _minus(r2 + 1, c1, inc), _plus(r2 + 1, c2 + 1, inc);
            }
            /**
             * @brief 单点查询，返回 `(i, j)` 的值
             */
            Tp query(size_type i, size_type j) const {
                if constexpr (AutoSwitch) switch_to_value();
                return _get(i, j);
            }
            /**
             * @brief 区间查询，返回区间 `[r1, r2) x [c1, c2)` 的和
             */
            Tp query(size_type r1, size_type r2, size_type c1, size_type c2) const {
                r2--, c2--;
                if constexpr (AutoSwitch) switch_to_presum();
                return _get(r1 - 1, c1 - 1) - _get(r1 - 1, c2) - _get(r2, c1 - 1) + _get(r2, c2);
            }
            Tp query_all() const {
                if constexpr (AutoSwitch) switch_to_presum();
                return _get(m_row - 1, m_column - 1);
            }
            void switch_to_difference() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_DIFFERENCE);
                if (m_state == TableState::TABLE_PRESUM) _adjacent_difference();
                if (m_state == TableState::TABLE_VALUE) _adjacent_difference();
            }
            void switch_to_value() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_VALUE);
                if (m_state == TableState::TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TableState::TABLE_PRESUM) _adjacent_difference();
            }
            void switch_to_presum() const {
                if (m_state == TableState::TABLE_ANY) return (void)(m_state = TableState::TABLE_PRESUM);
                if (m_state == TableState::TABLE_DIFFERENCE) _partial_sum();
                if (m_state == TableState::TABLE_VALUE) _partial_sum();
            }
        };
        template <typename Ostream, typename Tp, bool AutoSwitch>
        Ostream &operator<<(Ostream &out, const Table<Tp, AutoSwitch> &x) {
            out << "[";
            for (size_type i = 0; i != x.row(); i++)
                for (size_type j = 0; j != x.column(); j++) out << (j ? " " : (i ? ", [" : "[")) << x.query(i, j) << (j == x.column() - 1 ? ']' : ',');
            return out << "]";
        };
    }
}
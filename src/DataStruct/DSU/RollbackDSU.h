#ifndef __OY_ROLLBACKDISJOINTUNION__
#define __OY_ROLLBACKDISJOINTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 回滚并查集
 * @example OY::RollbackDSU::Table dsu(n);
 */
namespace OY {
    namespace RollbackDSU {
        using size_type = uint32_t;
        struct Table {
            struct Record {
                size_type m_head_a, m_head_b;
            };
            std::vector<size_type> m_parent, m_group_size;
            std::vector<Record> m_records;
            size_type m_size, m_group_cnt;
        public:
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size), m_group_size.resize(m_size, 1);
                std::iota(m_parent.begin(), m_parent.end(), 0);
            }
            size_type find(size_type i) const { return m_parent[i] == i ? i : find(m_parent[i]); }
            template <bool IsHead = false>
            size_type size(size_type i) const {
                if constexpr (IsHead)
                    return m_group_size[i];
                else
                    return m_group_size[find(i)];
            }
            /**
             * @brief 将 head_a 合并到 head_b，需要保证都是 head
             * @param MakeRecord 是否记录合并，便于后续撤销
             */
            template <bool MakeRecord = true>
            void unite_to(size_type head_a, size_type head_b) {
                m_parent[head_a] = head_b;
                m_group_size[head_b] += m_group_size[head_a];
                m_group_cnt--;
                if constexpr (MakeRecord) m_records.push_back({head_a, head_b});
            }
            /**
             * @brief 将 a 和 b 合并
             * @param MakeRecord 是否记录合并，便于后续撤销
             */
            template <bool MakeRecord = true>
            bool unite_by_size(size_type a, size_type b) {
                a = find(a), b = find(b);
                if (a == b) return false;
                if (m_group_size[a] > m_group_size[b]) std::swap(a, b);
                unite_to<MakeRecord>(a, b);
                return true;
            }
            void cancel(size_type head_a, size_type head_b) {
                m_parent[head_a] = head_a;
                m_group_size[head_b] -= m_group_size[head_a];
                m_group_cnt++;
            }
            /**
             * @brief 撤销上一次的合并
             */
            void cancel() {
                size_type head_a = m_records.back().m_head_a, head_b = m_records.back().m_head_b;
                m_records.pop_back();
                m_parent[head_a] = head_a;
                m_group_size[head_b] -= m_group_size[head_a];
                m_group_cnt++;
            }
            bool in_same_group(size_type a, size_type b) const { return find(a) == find(b); }
            bool is_head(size_type i) const { return i == m_parent[i]; }
            /**
             * @brief 返回当前有多少个组
             */
            size_type count() const { return m_group_cnt; }
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i < m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            /**
             * @brief 以二维数组的形式返回每个组的成员
             */
            std::vector<std::vector<size_type>> groups() const {
                std::vector<std::vector<size_type>> ret(m_group_cnt);
                std::vector<size_type> index(m_size);
                for (size_type i = 0, j = 0; i != m_size; i++)
                    if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                for (size_type i = 0; i != m_size; i++) ret[index[find(i)]].push_back(i);
                return ret;
            }
        };
        template <typename Ostream>
        Ostream &operator<<(Ostream &out, const Table &x) {
            out << "[";
            for (size_type i = 0; i != x.m_size; i++) {
                if (i) out << ", ";
                out << x.m_parent[i];
            }
            return out << "]";
        }
    }
}

#endif
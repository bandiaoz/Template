#ifndef __OY_DISJOINTUNION__
#define __OY_DISJOINTUNION__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>
#include <cassert>

/**
 * @brief Disjoint Set Union 并查集
 * @tparam MaintainGroupSize 是否维护每个组的大小
 * @example OY::DSUTable<true> dsu(n);
 */
namespace OY {
    namespace DSU {
        using size_type = uint32_t;
        template <bool MaintainGroupSize>
        class Table {
            mutable std::vector<size_type> m_parent, m_group_size;
            size_type m_size, m_group_cnt;
        public:
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size);
                std::iota(m_parent.begin(), m_parent.end(), 0);
                if constexpr (MaintainGroupSize) m_group_size.assign(m_size, 1);
            }
            size_type find(size_type i) const { return m_parent[i] == i ? i : m_parent[i] = find(m_parent[i]); }
            size_type size() const { return m_size; }
            template <bool IsHead = false>
            size_type size(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                if constexpr (IsHead)
                    return m_group_size[i];
                else
                    return m_group_size[find(i)];
            }
            /**
             * @brief 将 head_a 合并到 head_b，需要保证都是 head
             */
            void unite_to(size_type head_a, size_type head_b) {
                if (head_a == head_b) return;
                m_parent[head_a] = head_b;
                if constexpr (MaintainGroupSize) m_group_size[head_b] += m_group_size[head_a];
                m_group_cnt--;
            }
            bool unite_by_size(size_type a, size_type b) {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                a = find(a), b = find(b);
                if (a == b) return false;
                if (m_group_size[a] > m_group_size[b]) std::swap(a, b);
                unite_to(a, b);
                return true;
            }
            bool unite_by_ID(size_type a, size_type b) {
                a = find(a), b = find(b);
                if (a == b) return false;
                if (a < b) std::swap(a, b);
                unite_to(a, b);
                return true;
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
                for (size_type i = 0; i != m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            /**
             * @brief 以二维数组的形式返回每个组的成员
             */
            std::vector<std::vector<size_type>> groups() const {
                if constexpr (MaintainGroupSize) {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i)]].push_back(i);
                    return ret;
                } else {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size), cnt(m_group_cnt);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) cnt[index[find(i)]]++;
                    for (size_type i = 0; i != m_group_cnt; i++) ret[i].reserve(cnt[i]);
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i)]].push_back(i);
                    return ret;
                }
            }
        };
        /**
         * @brief 合并两个并查集并返回
         */
        template <bool MaintainGroupSize>
        Table<MaintainGroupSize> merge(const Table<MaintainGroupSize> &a, const Table<MaintainGroupSize> &b) {
            assert(a.size() == b.size());
            auto res = a;
            for (size_type i = 0; i < b.size(); i++) {
                res.unite_to(a.find(i), b.find(i));
            }
            return res;
        }
        template <typename Ostream, bool MaintainGroupSize>
        Ostream &operator<<(Ostream &out, const Table<MaintainGroupSize> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.find(i);
            }
            return out << "]";
        }
    }
    template <bool MaintainGroupSize = true>
    using DSUTable = DSU::Table<MaintainGroupSize>;
}

#endif
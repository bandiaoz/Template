#ifndef __OY_DOUBLELCA__
#define __OY_DOUBLELCA__

#include <algorithm>
#include <cstdint>
#include <vector>

#if __has_include(<bit>)
#include <bit>
#else
#include "src/Misc/std_bit.h"
#endif

/**
 * @brief 倍增法求 `LCA`
 * @example OY::DoubleLCA::Table<decltype(tree)> LCA(&tree);
 */
namespace OY {
    namespace DoubleLCA {
        using size_type = uint32_t;
        template <typename Tree, size_type MAX_LEVEL = 32>
        struct Table {
            Tree *m_rooted_tree;
            std::vector<size_type> m_fa[MAX_LEVEL], m_dep;
            size_type m_level;
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_fa[0].resize(m_rooted_tree->vertex_cnt()), m_dep.resize(m_rooted_tree->vertex_cnt());
                size_type *fa = m_fa[0].data();
                auto pre_work = [&](size_type a, size_type p) { fa[a] = p, m_dep[a] = ~p ? m_dep[p] + 1 : 0; };
                m_rooted_tree->tree_dp_vertex(m_rooted_tree->m_root, pre_work, {}, {});
                m_level = std::bit_width(*std::max_element(m_dep.begin(), m_dep.end()));
                for (size_type j = 1; j < m_level; j++) {
                    m_fa[j].resize(m_rooted_tree->vertex_cnt());
                    size_type *prev = m_fa[j - 1].data(), *cur = m_fa[j].data();
                    for (size_type i = 0; i != m_rooted_tree->vertex_cnt(); i++) cur[i] = ~prev[i] ? prev[prev[i]] : -1;
                }
            }
            /**
             * @brief 查询 `u` 的 `n` 级祖先，0 级祖先就是节点 `u`
             * @note 祖先不存在时返回 -1
             */
            size_type get_ancestor(size_type u, size_type n) const {
                if (n > m_dep[u]) return -1;
                while (n) {
                    size_type k = std::countr_zero(n);
                    u = m_fa[k][u], n -= 1 << k;
                }
                return u;
            }
            /**
             * @brief 找到 `u` 的父节点
             * @note 如果 `u` 是根节点则返回 -1
             */
            size_type find_parent(size_type u) const { return m_fa[0][u]; }
            /**
             * @brief 找到 `u` 在 `v` 方向上的一个孩子
             * @note 要求 `v` 是 `u` 的子孙
             */
            size_type find_son(size_type u, size_type v) const { return get_ancestor(v, m_dep[v] - m_dep[u] - 1); }
            /**
             * @brief 查询节点 `u` 的深度
             */
            size_type get_depth(size_type u) const { return m_dep[u]; }
            /**
             * @brief 查询 `u` 和 `v` 的最近公共祖先
             */
            size_type calc(size_type u, size_type v) const {
                const size_type *dep = m_dep.data();
                if (dep[u] > dep[v]) std::swap(u, v);
                size_type x = dep[v] - dep[u];
                while (x) {
                    size_type k = std::countr_zero(x);
                    v = m_fa[k][v], x -= 1 << k;
                }
                if (u == v) return u;
                for (size_type j = m_level - 1; ~j; j--) {
                    const size_type *cur = m_fa[j].data();
                    if (cur[u] != cur[v]) u = cur[u], v = cur[v];
                }
                return m_fa[0][u];
            }
        };
    }
}

#endif
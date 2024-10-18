#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

/**
 * @brief 轻重链剖分
 * @example OY::HLD::Table<decltype(tree)> hld(&tree);
 */
namespace OY {
    namespace HLD {
        using size_type = uint32_t;
        template <typename Tree>
        struct Table {
            /**
             * @param m_top_dfn 所在链的最高位节点的 dfs 序
             * @param m_top_dep 所在链的最高位节点的深度
             * @param m_parent 所在链的父节点编号
             * @param m_dfn 节点的 dfs 序
             * @param m_dep 节点的深度，根的深度为 0
             * @param m_size 节点的子树大小
             * @param m_heavy 节点的重孩子编号，叶子节点重孩子为-1
             * @param m_seq 所有节点按照 dfs 序形成的序列
             */
            struct node {
                size_type m_top_dfn, m_top_dep, m_parent, m_dfn, m_dep, m_size, m_heavy;
            };
            Tree *m_rooted_tree;
            std::vector<node> m_info;
            std::vector<size_type> m_seq;
            void _tree_dfs1(size_type u, size_type f) {
                m_info[u].m_size = 1, m_info[u].m_heavy = -1;
                m_rooted_tree->do_for_each_adj_vertex(u, [&](size_type v) {
                    if (v != f) {
                        _tree_dfs1(v, u);
                        size_type v_size = m_info[v].m_size;
                        m_info[u].m_size += v_size;
                        if (!~m_info[u].m_heavy || v_size > m_info[m_info[u].m_heavy].m_size) {
                            m_info[u].m_heavy = v;
                        }
                    }
                });
            }
            void _tree_dfs2(size_type u, size_type f, size_type &cursor) {
                m_seq[cursor] = u;
                if (~f) m_info[u].m_dep = m_info[f].m_dep + 1;
                m_info[u].m_dfn = cursor++;
                bool is_top = !~f || u != m_info[f].m_heavy;
                m_info[u].m_top_dfn = is_top ? m_info[u].m_dfn : m_info[f].m_top_dfn;
                m_info[u].m_top_dep = is_top ? m_info[u].m_dep : m_info[f].m_top_dep;
                m_info[u].m_parent = is_top ? f : m_info[f].m_parent;
                size_type heavy = m_info[u].m_heavy;
                if (~heavy) _tree_dfs2(heavy, u, cursor);
                m_rooted_tree->do_for_each_adj_vertex(u, [&](size_type v) { 
                    if (v != f && v != heavy) _tree_dfs2(v, u, cursor); 
                });
            }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_info.resize(m_rooted_tree->vertex_cnt()), m_seq.resize(m_rooted_tree->vertex_cnt());
                _tree_dfs1(m_rooted_tree->m_root, -1);
                size_type cursor = 0;
                _tree_dfs2(m_rooted_tree->m_root, -1, cursor);
            }
            /**
             * @brief 查询 `u` 的 `k` 级祖先，0 级祖先就是节点 `u`
             */
            size_type get_ancestor(size_type u, size_type k) const {
                auto info = m_info.data();
                if (k > info[u].m_dep) return -1;
                size_type dep = info[u].m_dep, target_dep = dep - k;
                while (target_dep < info[u].m_top_dep) {
                    dep = info[u].m_top_dep - 1, u = info[u].m_parent;
                }
                return m_seq[info[u].m_dfn - dep + target_dep];
            }
            /**
             * @brief 找到 `u` 的父节点，如果 `u` 是根节点则返回 -1
             */
            size_type find_parent(size_type u) const { return m_info[u].m_top_dep == m_info[u].m_dep ? m_info[u].m_parent : (m_info[u].m_dfn ? m_seq[m_info[u].m_dfn - 1] : -1); }
            /**
             * @brief 找到 `u` 在 `b` 方向上的一个孩子，`b` 在 `u` 的子树上
             */
            size_type find_son(size_type u, size_type b) const { return get_ancestor(b, m_info[b].m_dep - m_info[u].m_dep - 1); }
            size_type get_depth(size_type u) const { return m_info[u].m_dep; }
            template <typename Callback>
            auto do_for_vertex(size_type u, Callback &&call) const -> decltype(call(0)) { return call(m_info[u].m_dfn); }
            /**
             * @brief 对路径上的节点形成的序列区间依次调用回调
             * @tparam LCA 是否包含路径最高点
             * @param call 参数为区间的左右端点，左闭右开
             */
            template <bool LCA, typename Callback>
            void do_for_path(size_type u, size_type v, Callback &&call) const {
                auto info = m_info.data();
                while (info[u].m_top_dfn != info[v].m_top_dfn) {
                    if (info[u].m_top_dep < info[v].m_top_dep)
                        call(info[v].m_top_dfn, info[v].m_dfn + 1), v = info[v].m_parent;
                    else
                        call(info[u].m_top_dfn, info[u].m_dfn + 1), u = info[u].m_parent;
                }
                if (info[u].m_dep > info[v].m_dep) std::swap(u, v);
                if constexpr (LCA)
                    call(info[u].m_dfn, info[v].m_dfn + 1);
                else if (u != v)
                    call(info[u].m_dfn + 1, info[v].m_dfn + 1);
            }
            /**
             * @brief 对路径上的节点形成的有序区间调用回调
             * @param call  参数为区间的左右端点，左闭右开
             */
            template <typename Callback>
            void do_for_directed_path(size_type u, size_type v, Callback &&call) const {
                auto info = m_info.data();
                while (info[u].m_top_dfn != info[v].m_top_dfn) {
                    if (info[u].m_top_dep < info[v].m_top_dep) {
                        do_for_directed_path(u, info[v].m_parent, call);
                        call(info[v].m_top_dfn, info[v].m_dfn + 1);
                        return;
                    } else {
                        call(info[u].m_dfn, info[u].m_top_dfn + 1), u = info[u].m_parent;
                    }
                }
                call(info[u].m_dfn, info[v].m_dfn + 1);
            }
            /**
             * @brief 对子树形成的区间依次调用回调
             * @param call 参数为区间的左右端点，左闭右开
             */
            template <typename Callback>
            auto do_for_subtree(size_type u, Callback &&call) const -> decltype(call(0, 0)) { 
                return call(m_info[u].m_dfn, m_info[u].m_dfn + m_info[u].m_size); 
            }
            /**
             * @brief 查询 `u` 和 `v` 的 LCA
             */
            size_type lca(size_type u, size_type v) const {
                auto info = m_info.data();
                while (info[u].m_top_dfn != info[v].m_top_dfn)
                    if (info[u].m_top_dep < info[v].m_top_dep)
                        v = info[v].m_parent;
                    else
                        u = info[u].m_parent;
                return info[u].m_dep < info[v].m_dep ? u : v;
            }
            /**
             * @brief 判断 `u` 是否是 `v` 的祖先
             */
            bool isAncestor(int u, int v) {
                auto info = m_info.data();
                return info[u].m_dfn <= info[v].m_dfn && info[v].m_dfn < info[u].m_dfn + info[u].m_size;
            }
        };
    }
}
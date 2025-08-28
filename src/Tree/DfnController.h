#ifndef __OY_DFNCONTROLLER__
#define __OY_DFNCONTROLLER__

#include <cstdint>
#include <vector>

/**
 * @brief dfn 控制器
 * @example OY::DFN::Table<decltype(tree)> dfn(&tree);
 */
namespace OY {
    namespace DFN {
        using size_type = uint32_t;
        template <typename Tree>
        struct Table {
            struct node {
                size_type m_dfn, m_size;
            };
            Tree *m_rooted_tree;
            std::vector<node> m_info;
            std::vector<size_type> m_seq;
            void _tree_dfs(size_type u, size_type f, size_type &cursor) {
                m_seq[cursor] = u;
                m_info[u].m_dfn = cursor++;
                m_rooted_tree->do_for_each_adj_vertex(u, [&](size_type v) { if (v != f) _tree_dfs(v, u, cursor); });
                m_info[u].m_size = cursor - m_info[u].m_dfn;
            }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_rooted_tree = rooted_tree)) return;
                m_info.resize(m_rooted_tree->vertex_cnt()), m_seq.resize(m_rooted_tree->vertex_cnt());
                size_type cursor = 0;
                _tree_dfs(m_rooted_tree->m_root, -1, cursor);
            }
            /**
             * @brief 对子树进行遍历，参数为子树的 dfn 序区间
             */
            template <typename DfnCallback>
            auto do_for_subtree(size_type u, DfnCallback &&call) const -> decltype(call(0, 0)) { return call(m_info[u].m_dfn, m_info[u].m_dfn + m_info[u].m_size); }
            /**
             * @brief 对单个点进行遍历，参数为点的 dfn 序
             */
            template <typename DfnCallback>
            auto do_for_vertex(size_type u, DfnCallback &&call) const -> decltype(call(0)) { return call(m_info[u].m_dfn); }
        };
    }
}

#endif
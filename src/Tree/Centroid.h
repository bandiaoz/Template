#ifndef __OY_CENTROID__
#define __OY_CENTROID__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <vector>

/**
 * @brief 树的重心、树哈希、重心分治（点分树）
 */
namespace OY {
    namespace Centroid {
        using size_type = uint32_t;
        template <typename Key, typename Value>
        using map_type = std::unordered_map<Key, Value>;
        struct Ignore {
            template <typename... Args>
            void operator()(Args... args) const {}
        };
        template <typename Tree>
        struct Centroid {
            Tree *m_tree;
            std::vector<size_type> m_max_adj;
            std::pair<size_type, size_type> m_centroid;
            size_type _tree_dfs(size_type a, size_type p) {
                size_type size = 1;
                m_tree->do_for_each_adj_vertex(
                    a, [&](size_type to) {
                        if (to != p) {
                            size_type to_size = _tree_dfs(to, a);
                            size += to_size, m_max_adj[a] = std::max(m_max_adj[a], to_size);
                        }
                    });
                m_max_adj[a] = std::max(m_max_adj[a], m_tree->vertex_cnt() - size);
                if (m_max_adj[a] * 2 <= m_tree->vertex_cnt())
                    if (~m_centroid.first)
                        m_centroid.second = a;
                    else
                        m_centroid.first = a;
                return size;
            }
            Centroid(Tree *tree = nullptr) { reset(tree); }
            void reset(Tree *tree) {
                if (!(m_tree = tree)) return;
                m_max_adj.resize(m_tree->vertex_cnt());
                m_centroid.first = m_centroid.second = -1;
                _tree_dfs(0, -1);
                if (~m_centroid.second && m_centroid.first > m_centroid.second) std::swap(m_centroid.first, m_centroid.second);
            }
            /**
             * @brief 返回树的重心
             * @note 重心数量可能是 1 个或 2 个，如果只有一个重心，则 `second` 为 `-1`；否则 `first` 为较小的重心，`second` 为较大的重心
             */
            std::pair<size_type, size_type> centroid() const { return m_centroid; }
            size_type max_adjacent(size_type i) const { return m_max_adj[i]; }
        };
        struct TreeTrie {
            struct TreeTrieNode {
                size_type m_id;
                map_type<size_type, TreeTrieNode *> m_sons;
                TreeTrieNode(size_type id) : m_id(id) {}
            };
            static TreeTrieNode s_root;
            static size_type s_id_count;
            template <typename Tree>
            static void _tree_dfs(size_type a, size_type p, Tree &tree, std::vector<size_type> &parent, std::vector<size_type> &deg) {
                parent[a] = p;
                if (~p) deg[p]++;
                tree.do_for_each_adj_vertex(a, [&](size_type to) {if(to!=p)_tree_dfs(to,a,tree,parent,deg); });
            }
            template <typename Tree>
            static std::vector<size_type> get(Tree &tree, size_type root) {
                std::vector<size_type> res(tree.vertex_cnt()), parent(tree.vertex_cnt()), deg(tree.vertex_cnt());
                std::queue<size_type> Q;
                _tree_dfs(root, -1, tree, parent, deg);
                for (size_type i = 0; i != tree.vertex_cnt(); i++)
                    if (!deg[i]) Q.push(i);
                while (!Q.empty()) {
                    size_type a = Q.front();
                    Q.pop();
                    std::vector<size_type> children;
                    tree.do_for_each_adj_vertex(a, [&](size_type to) { if(to!=parent[a])children.push_back(res[to]); });
                    std::sort(children.begin(), children.end());
                    TreeTrieNode *cur = &s_root;
                    for (size_type id : children) {
                        auto it = cur->m_sons.emplace(id, nullptr).first;
                        if (!it->second) it->second = new TreeTrieNode(-1);
                        cur = it->second;
                    }
                    if (!~cur->m_id) cur->m_id = s_id_count++;
                    res[a] = cur->m_id;
                    size_type p = parent[a];
                    if (~p && !--deg[p]) Q.push(p);
                }
                return res;
            }
            template <typename Tree>
            static std::pair<size_type, size_type> get(Tree &tree) {
                auto centroid = Centroid<Tree>(&tree).centroid();
                centroid.first = get(tree, centroid.first)[centroid.first];
                if (~centroid.second) {
                    centroid.second = get(tree, centroid.second)[centroid.second];
                    if (centroid.second < centroid.first) std::swap(centroid.first, centroid.second);
                }
                return centroid;
            }
        };
        TreeTrie::TreeTrieNode TreeTrie::s_root(0);
        size_type TreeTrie::s_id_count = 1;
        /**
         * @brief 重心分治，解决树上路径问题
         * @tparam MAX_BUFFER 树的最大节点数量
         * @note 重心满足：删除重心后，剩余的树中最大子树的节点数量不超过总节点数量的一半
         * @note 分治每一层的复杂度为 `O(n)`，重心分治保证了分治的层数为 `O(logn)`。
         * @note 付出 `O(logn)` 的代价，换取「路径经过当前分治根节点」这一条件
         */
        template <size_type MAX_BUFFER>
        struct CentroidDecomposition {
            static size_type s_tag[MAX_BUFFER];
            template <typename Tree>
            static size_type _tree_dfs1(Tree &tree, size_type a) {
                size_type size = 1;
                s_tag[a] = 1;
                tree.do_for_each_adj_vertex(a, [&](size_type to) { if (!s_tag[to]) size += _tree_dfs1(tree,to); });
                s_tag[a] = 0;
                return size;
            }
            template <typename Tree>
            static size_type _tree_dfs2(Tree &tree, size_type a, size_type tot, size_type &centroid) {
                size_type size = 1;
                s_tag[a] = 1;
                tree.do_for_each_adj_vertex(a, [&](size_type to) {
                    if (!s_tag[to]) {
                        size_type to_size = _tree_dfs2(tree, to, tot, centroid);
                        size += to_size, s_tag[a] = std::max(s_tag[a], to_size);
                    }
                });
                s_tag[a] = std::max(s_tag[a], tot - size);
                if (s_tag[a] * 2 <= tot) centroid = a;
                s_tag[a] = 0;
                return size;
            }
            /**
             * @brief 重心分治，三个回调函数的参数都是当前分治的根节点
             * @param tree 存储树的结构和信息
             * @param pre_work 将当前分治根节点标记为已访问，`pre_work(centroid)`
             * @param report 一般不做任何事情，`report(child_centroid, centroid)`
             * @param after_work 执行当前分治范围内的计算，并解除当前分治根节点的阻塞标记，`after_work(centroid)`
             */
            template <typename Tree, typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            static size_type solve(Tree &tree, PreWork &&pre_work, Report &&report, AfterWork &&after_work) { return find_centroid(tree, 0, pre_work, report, after_work); }
            template <typename Tree, typename PreWork, typename Report, typename AfterWork>
            static size_type find_centroid(Tree &tree, size_type i, PreWork &&pre_work, Report &&report, AfterWork &&after_work) {
                size_type tot = _tree_dfs1(tree, i), centroid = i;
                _tree_dfs2(tree, i, tot, centroid);
                pre_work(centroid);
                s_tag[centroid] = 1;
                tree.do_for_each_adj_vertex(centroid, [&](size_type to) {
                    if (!s_tag[to]) report(find_centroid(tree, to, pre_work, report, after_work), centroid);
                });
                s_tag[centroid] = 0;
                after_work(centroid);
                return centroid;
            }
        };
        template <size_type MAX_BUFFER>
        size_type CentroidDecomposition<MAX_BUFFER>::s_tag[MAX_BUFFER];
    }
}

#endif

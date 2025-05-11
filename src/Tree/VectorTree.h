#ifndef __OY_VECTORTREE__
#define __OY_VECTORTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

/**
 * @brief VectorTree
 * @tparam Tp 边权类型，bool 代表无权图
 * @example OY::VectorTree::Tree<Tp> tree(5);
 * @note 注意设置 root
 */
namespace OY {
    namespace VectorTree {
        using size_type = uint32_t;
        struct Ignore {
            template <typename... Args>
            void operator()(Args... args) const {}
        };
        template <typename Tp>
        struct Edge {
            size_type m_to;
            Tp m_dis;
        };
        template <>
        struct Edge<bool> {
            size_type m_to;
        };
        template <typename Tp>
        struct Tree {
            std::vector<std::vector<Edge<Tp>>> m_adj;
            size_type m_root = -1, m_vertex_cnt;
            void _add(size_type a, size_type b) { m_adj[a].push_back({b}); }
            void _add(size_type a, size_type b, Tp dis) { m_adj[a].push_back({b, dis}); }
            template <typename PreWork, typename Report, typename AfterWork, bool IsBool = std::is_same<decltype(std::declval<PreWork>()(0, 0)), bool>::value>
            typename std::conditional<IsBool, bool, void>::type _tree_dp_vertex(size_type a, size_type p, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                if constexpr (!IsBool)
                    pre_work(a, p);
                else if (!pre_work(a, p))
                    return false;
                do_for_each_adj_vertex(a, [&](size_type to) {
                    if constexpr (IsBool) {
                        if (to != p && _tree_dp_vertex(to, a, pre_work, report, after_work)) report(a, to);
                    } else if (to != p)
                        _tree_dp_vertex(to, a, pre_work, report, after_work), report(a, to);
                });
                after_work(a);
                if constexpr (IsBool) return true;
            }
            template <typename PreWork, typename Report, typename AfterWork, bool IsBool = std::is_same<decltype(std::declval<PreWork>()(0, 0, std::declval<Tp>())), bool>::value>
            typename std::conditional<IsBool, bool, void>::type _tree_dp_edge(size_type a, size_type p, Tp up_dis, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const {
                if constexpr (!IsBool)
                    pre_work(a, p, up_dis);
                else if (!pre_work(a, p, up_dis))
                    return false;
                do_for_each_adj_edge(a, [&](size_type to, Tp dis) {
                    if constexpr (IsBool) {
                        if (to != p && _tree_dp_edge(to, a, dis, pre_work, report, after_work)) report(a, to, dis);
                    } else if (to != p)
                        _tree_dp_edge(to, a, dis, pre_work, report, after_work), report(a, to, dis);
                });
                after_work(a);
                if constexpr (IsBool) return true;
            }
            Tree(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                m_root = -1;
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_adj.assign(m_vertex_cnt, std::vector<Edge<Tp>>());
            }
            void add_edge(size_type a, size_type b, Tp dis = Tp()) {
                if constexpr (std::is_same<Tp, bool>::value)
                    _add(a, b), _add(b, a);
                else
                    _add(a, b, dis), _add(b, a, dis);
            }
            void prepare() {}
            size_type root() const { return m_root; }
            size_type vertex_cnt() const { return m_vertex_cnt; }
            void set_root(size_type root) { m_root = root; }
            /**
             * @brief 对当前点的所有邻接点进行操作
             */
            template <typename Callback>
            void do_for_each_adj_vertex(size_type a, Callback &&call) const {
                for (auto &adj : m_adj[a]) call(adj.m_to);
            }
            /**
             * @brief 对当前点的所有相邻边进行操作
             * @param call 回调函数，参数为 (to, dis)
             */
            template <typename Callback>
            void do_for_each_adj_edge(size_type a, Callback &&call) const {
                if constexpr (std::is_same<Tp, bool>::value)
                    for (auto &adj : m_adj[a]) call(adj.m_to, 1);
                else
                    for (auto &adj : m_adj[a]) call(adj.m_to, adj.m_dis);
            }
            /**
             * @brief 基于点的树形 dp
             * @param u 进行树形 dp 时的起始结点，或者说根结点
             * @param pre_work `pre_work(u, f)`，每当来到一个结点处时，最先调用的回调函数
             * @param report `report(u, v)`，每当来到一个结点处时，对其一个邻接点递归返回后，调用的回调函数
             * @param after_work `after_work(u)`，每当来到一个结点处时，对所有邻接点递归返回后，调用的回调函数
             */
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_vertex(size_type u, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { 
                _tree_dp_vertex(u, -1, pre_work, report, after_work); 
            }
            /**
             * @brief 基于边的树形 dp
             * @param u 进行树形 dp 时的起始结点，或者说根结点
             * @param pre_work `pre_work(u, f, up_dis)`，每当来到一个结点处时，最先调用的回调函数
             * @param report `report(u, v, dis)`，每当来到一个结点处时，对其一个邻接点递归返回后，调用的回调函数
             * @param after_work `after_work(u)`，每当来到一个结点处时，对所有邻接点递归返回后，调用的回调函数
             * @note 这里 `up_dis` 是 `u` 到 `f` 的父亲的边权， `dis` 是 `u -> v` 的边权，
             */
            template <typename PreWork = Ignore, typename Report = Ignore, typename AfterWork = Ignore>
            void tree_dp_edge(size_type u, PreWork &&pre_work, Report &&report, AfterWork &&after_work) const { 
                _tree_dp_edge(u, -1, {}, pre_work, report, after_work); 
            }
        };
        template <typename Ostream, typename Tp>
        Ostream &operator<<(Ostream &out, const Tree<Tp> &tree) { // http://mshang.ca/syntree/
            tree.tree_dp_vertex(~tree.root() ? tree.root() : 0, [&](size_type a, size_type) { out << '[' << a; }, {}, [&](size_type) { out << ']'; });
            return out;
        }
    }
}

#endif
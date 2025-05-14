#ifndef __OY_TARJAN_BRIDGE__
#define __OY_TARJAN_BRIDGE__

#include <algorithm>
#include <cstdint>
#include <vector>

/**
 * @brief 桥，边双连通分量
 * @example OY::EBCC::Graph G(vertex_cnt, edge_cnt);
 *          auto solver = G.calc<true, true>();
 * @note 无向图中，如果删除某条边后，图变成不连通，则称该边为割边（桥）。
 *       如果无论删除哪条边，图都连通，则称该图是边双连通（EBCC）的。
 * @note 边双连通具有传递性，即边 a 和边 b 边双连通，边 b 和边 c 边双连通，则边 a 和边 c 边双连通。
 * @note 原图删掉桥就得到了边双连通分量。边双+桥=树
 *       每个点属于一个边双，桥横跨了两个边双，其他边每条边属于一个边双
 */
namespace OY {
    namespace EBCC {
        using size_type = uint32_t;
        template <bool GetBridge, bool GetEBCC>
        struct Solver {
            struct info {
                size_type m_dfn, m_low;
            };
            size_type m_vertex_cnt, m_dfn_cnt, m_bridge_cnt, m_ebcc_cnt, m_stack_len;
            std::vector<size_type> m_starts, m_stack, m_ebccs;
            std::vector<bool> m_is_bridge;
            std::vector<info> m_info;
            /**
             * @brief 找桥和边双连通分量
             * @note 无向图中，DFS 生成树上的边要么是树边（tree edge），要么是非树边（返祖边，back edge）。
             * @note `m_info[u].m_low` 表示 `u` 子树中能够通过一条 B 边（返祖边）走到的最早节点的 dfn 值
             * @note 如果树边的孩子节点 `v` 不能到达 `u` 或者比 `u` 更早的点，即 `low[v] > dfn[u]`（等价于 `dfn[v] == dfn[v]`），则边 `(u, v)` 是桥
             */
            template <typename Traverser>
            void _dfs(size_type i, size_type from, Traverser &&traverser) {
                size_type pos = m_stack_len;
                if constexpr (GetEBCC) m_stack[m_stack_len++] = i;
                m_info[i].m_dfn = m_info[i].m_low = m_dfn_cnt++;
                traverser(i, [&](size_type index, size_type to) {
                    if (!~m_info[to].m_dfn) {
                        _dfs(to, index, traverser);
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_low);
                    } else if (index != from)
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_dfn);
                });
                if (m_info[i].m_low != m_info[i].m_dfn) return;
                if constexpr (GetEBCC) {
                    std::copy(m_stack.data() + pos, m_stack.data() + m_stack_len, m_ebccs.data() + (m_starts[m_ebcc_cnt++] = m_dfn_cnt - m_stack_len));
                    m_stack_len = pos;
                }
                if constexpr (GetBridge)
                    if (~from) m_is_bridge[from] = true, m_bridge_cnt++;
            }
            Solver(size_type vertex_cnt, size_type edge_cnt) : 
                m_vertex_cnt(vertex_cnt), m_dfn_cnt(0), m_bridge_cnt(0), m_ebcc_cnt(0), m_stack_len(0), 
                m_starts(vertex_cnt + 1), m_stack(vertex_cnt), m_ebccs(vertex_cnt), 
                m_is_bridge(edge_cnt), m_info(vertex_cnt) {}
            template <typename Traverser>
            void run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_dfn = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (!~m_info[i].m_dfn) _dfs(i, -1, traverser);
                if constexpr (GetEBCC) m_starts[m_ebcc_cnt] = m_vertex_cnt;
            }
            /**
             * @brief 操作所有的边双连通分量
             * @param call 的参数是边双连通分量的首尾指针，`call(uint32_t *first, uint32_t *last)`
             */
            template <typename Callback>
            void do_for_each_ebcc(Callback &&call) {
                for (size_type i = 0, cur = m_starts[0], end; i != m_ebcc_cnt; cur = end, i++) 
                    call(m_ebccs.data() + cur, m_ebccs.data() + (end = m_starts[i + 1]));
            }
            /**
             * @brief 操作所有的桥
             * @param call 的参数是桥的编号，`call(uint32_t index)`
             * @note 桥的编号是按照输入顺序存储的，如果需要获得边的两个端点，可以 `[u, v] = m_raw_edges[index]`
             */
            template <typename Callback>
            void do_for_each_bridge(Callback &&call) {
                for (size_type index = 0; index != m_is_bridge.size(); index++)
                    if (m_is_bridge[index]) call(index);
            }
        };
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
            };
            struct edge {
                size_type m_index, m_to;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts;
            mutable std::vector<edge> m_edges;
            std::vector<raw_edge> m_raw_edges; // 按照输入顺序存储边
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                auto *first = m_edges.data() + m_starts[from], *last = m_edges.data() + m_starts[from + 1];
                for (auto it = first; it != last; ++it) call(it->m_index, it->m_to);
            }
            void _prepare() const {
                for (auto &e : m_raw_edges) {
                    size_type from = e.m_from, to = e.m_to;
                    m_starts[from + 1]++;
                    if (from != to) m_starts[to + 1]++;
                }
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (size_type i = 0; i != m_raw_edges.size(); i++) {
                    size_type from = m_raw_edges[i].m_from, to = m_raw_edges[i].m_to;
                    m_edges[cursor[from]++] = {i, to};
                    if (from != to) m_edges[cursor[to]++] = {i, from};
                }
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { resize(vertex_cnt, edge_cnt); }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
            }
            /**
             * @brief 添加一条从 `a` 到 `b` 的无向边
             */
            void add_edge(size_type a, size_type b) { m_raw_edges.push_back({a, b}); }
            /**
             * @brief 获得查询器
             * @tparam GetBridge 是否查询桥
             * @tparam GetEBCC 是否查询边双连通分量
             */
            template <bool GetBridge, bool GetEBCC>
            Solver<GetBridge, GetEBCC> calc() const {
                if (!m_prepared) _prepare();
                Solver<GetBridge, GetEBCC> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                return sol;
            }
            /**
             * @brief 返回所有的边双连通分量
             */
            std::vector<std::vector<size_type>> get_ebccs() const {
                if (!m_prepared) _prepare();
                Solver<false, true> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<std::vector<size_type>> res;
                res.reserve(sol.m_ebcc_cnt);
                sol.do_for_each_ebcc([&](size_type *first, size_type *last) { res.emplace_back(first, last); });
                return res;
            }
            /**
             * @brief 返回所有的桥的编号
             */
            std::vector<size_type> get_bridges() const {
                if (!m_prepared) _prepare();
                Solver<true, false> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<size_type> res;
                res.reserve(sol.m_bridge_cnt);
                sol.do_for_each_bridge([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
    }
}

#endif
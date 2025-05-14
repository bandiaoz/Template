#ifndef __OY_TARJAN_CUT__
#define __OY_TARJAN_CUT__

#include <algorithm>
#include <cstdint>
#include <vector>

/**
 * @brief 割点，点双连通分量
 * @example OY::VBCC::Graph graph(vertex_cnt, edge_cnt);
 *          auto vbcc = graph.calc<true, true>();
 * @note 无向图中，如果删除某个点后，图变成不连通，则称该点为割点。 
 * @note 如果无论删除哪个点，图都连通，则称该图是点双连通（VBCC）的。
 *       点双连通分量中，任意两个点都存在至少两条点不重复的路径(不包括端点)。近似等价于不存在割点。
 * @note 点双连通不具有传递性，即点 A 和点 B 点双连通，点 B 和点 C 点双连通，点 A 和点 C 不一定点双连通。
 * @note 每条边属于一个点双，割点属于多个点双，其他点属于一个点双。
 * @note 圆方树中，每个点双对应一个方点，原图的点对应一个圆点，每个圆点向所有包含它的方点连边。
 *       圆方树的点数是原图的点数（圆点） + 原图的点双连通分量数（方点）。
 * @note 圆方树的性质：
 *       1. 圆点方点交替出现，两个方点（点双）之间割点数量 = 树上距离 / 2
 *          原图 u -> v 的必经点数量 = 圆方树上 u -> v 路径上圆点数量 = 树上距离 / 2
 *       2. 对于每一个度数为 1 的圆点不是割点，都对应唯一方点（仅属于一个点双）
 *       3. 原图的割点是圆方树中度数大于 1 的圆点，这种割点被若干点双（与其相邻的方点）共享
 *       4. 原图中每条边对应唯一方点（点双）
 *       5. 无论取那个点为根开始 dfs 建圆方树，圆方树的形态不变（找到的点双是固定的）
 *       6. 对于同一个点双中的两点，它们之间简单路径的并集，恰好完全等于这个点双；
 *          即同一个点双中的两不同点 u, v 之间一定存在一条简单路径经过这个点双的另一点 w
 *       7. 圆方树中，任意两个点 u, v 之间，简单路径的并集，恰好等于它们之间路径上所有点双的并集
 */
namespace OY {
    namespace VBCC {
        using size_type = uint32_t;
        template <bool GetCut, bool GetVBCC>
        struct Solver {
            struct info {
                size_type m_dfn, m_low;
            };
            size_type m_vertex_cnt, m_dfn_cnt, m_cut_cnt, m_vbcc_cnt, m_stack_len, m_cursor;
            std::vector<size_type> m_starts, m_stack, m_vbccs;
            std::vector<info> m_info;
            std::vector<bool> m_is_cut;
            /**
             * @brief 找点双和割点
             * @note 无向图中，DFS 生成树上的边要么是树边（tree edge），要么是非树边（返祖边，back edge）。
             * @note `m_info[u].m_low` 表示 `u` 子树中能够通过一条 B 边（返祖边）走到的最早节点的 dfn 值
             * @note `adj` 表示 `u` 的子树中，有多少个孩子节点 `v` 到达 `u` 或者比 `u` 更早的点，即 `low[v] >= dfn[u]`
             * @note 如果 `u` 是根节点，则需要 `adj >= 2` 才是割点，否则需要 `adj >= 1` 才是割点
             */
            template <typename Traverser>
            void _dfs(size_type i, size_type from, size_type parent, Traverser &&traverser) {
                size_type pos = m_stack_len, adj = from != -1;
                m_info[i].m_dfn = m_info[i].m_low = m_dfn_cnt++;
                if constexpr (GetVBCC) m_stack[m_stack_len++] = i;
                traverser(i, [&](size_type index, size_type to) {
                    if (!~m_info[to].m_dfn) {
                        _dfs(to, index, i, traverser);
                        if (m_info[to].m_low < m_info[i].m_dfn)
                            m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_low);
                        else
                            adj++;
                    } else if (index != from)
                        m_info[i].m_low = std::min(m_info[i].m_low, m_info[to].m_dfn);
                });
                if constexpr (GetVBCC) {
                    if (!~from) {
                        if (!adj) m_vbccs[m_starts[m_vbcc_cnt++] = m_cursor++] = i;
                        m_stack_len = pos;
                    } else if (m_info[i].m_low >= m_info[parent].m_dfn) {
                        std::swap(m_stack[pos - 1], parent);
                        size_type len = m_stack_len - pos + 1;
                        std::copy_n(m_stack.data() + pos - 1, len, m_vbccs.data() + m_cursor);
                        m_starts[m_vbcc_cnt++] = m_cursor, m_cursor += len, m_stack[(m_stack_len = pos) - 1] = parent;
                    }
                }
                if constexpr (GetCut) {
                    if (adj > 1) m_is_cut[i] = true, m_cut_cnt++;
                }
            }
            template <typename EdgeCallback, typename SingleCallback, typename Traverser>
            void _dfs(size_type i, size_type from, size_type parent, std::vector<size_type> &stack, std::vector<bool> &visit, EdgeCallback &&edge_call, SingleCallback &&single_call, Traverser &&traverser) {
                visit[i] = true;
                size_type pos = stack.size(), adj = from != -1;
                if (~from) stack.push_back(from);
                traverser(i, [&](size_type index, size_type to) {
                    if (!visit[to]) {
                        _dfs(to, index, i, stack, visit, edge_call, single_call, traverser);
                        if (m_info[to].m_low >= m_info[i].m_dfn) adj++;
                    } else if (index != from && m_info[to].m_dfn <= m_info[i].m_dfn) {
                        stack.push_back(index);
                    }
                });
                if (!~from) {
                    if (!adj) {
                        if (stack.size() == pos) {
                            single_call(i);
                        } else {
                            edge_call(stack.data() + pos, stack.data() + stack.size());
                        }
                    }
                    stack.resize(pos);
                } else if (m_info[i].m_low >= m_info[parent].m_dfn) {
                    edge_call(stack.data() + pos, stack.data() + stack.size());
                    stack.resize(pos);
                }
            }
            Solver(size_type vertex_cnt, size_type edge_cnt) 
                : m_vertex_cnt(vertex_cnt), m_dfn_cnt(0), m_cut_cnt(0), m_vbcc_cnt(0), 
                m_stack_len(0), m_cursor(0), m_info(vertex_cnt), m_starts(vertex_cnt + 1), 
                m_stack(vertex_cnt), m_vbccs(vertex_cnt + edge_cnt), m_is_cut(vertex_cnt) {}
            template <typename Traverser>
            void run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_info[i].m_dfn = -1;
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    if (!~m_info[i].m_dfn) _dfs(i, -1, -1, traverser);
                }
                if constexpr (GetVBCC) m_starts[m_vbcc_cnt] = m_cursor;
            }
            /**
             * @brief 遍历 vbcc 里边的编号，遍历 vbcc 的顺序和 `do_for_each_vbcc` 一致
             * @param edge_call 处理有边的 vbcc 里的所有边，参数是遍历 vbcc 里边的编号的迭代器，`edge_call(uint32_t *first, uint32_t *last)`
             * @param single_call 处理没边的 vbcc 里的孤立点，参数是点的编号，`single_call(uint32_t i)`
             * @param traverser 传入图结构
             */
            template <typename EdgeCallback, typename SingleCallback, typename Traverser>
            void find_edges(EdgeCallback &&edge_call, SingleCallback &&single_call, Traverser &&traverser) {
                std::vector<bool> visit(m_vertex_cnt);
                std::vector<size_type> stack;
                stack.reserve(m_vbccs.size() - m_vertex_cnt);
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    if (!visit[i]) {
                        _dfs(i, -1, -1, stack, visit, edge_call, single_call, traverser);
                    }
                }
            }
            /**
             * @brief 操作所有的点双连通分量
             * @param call 的参数是点双连通分量的首尾指针，`call(uint32_t *first, uint32_t *last)`
             */
            template <typename Callback>
            void do_for_each_vbcc(Callback &&call) {
                for (size_type i = 0, cur = m_starts[0], end; i != m_vbcc_cnt; cur = end, i++) {
                    call(m_vbccs.data() + cur, m_vbccs.data() + (end = m_starts[i + 1]));
                }
            }
            /**
             * @brief 操作所有的割点
             * @param call 的参数是割点的编号，`call(uint32_t index)`
             */
            template <typename Callback>
            void do_for_each_cut(Callback &&call) {
                for (size_type index = 0; index != m_vertex_cnt; index++) {
                    if (m_is_cut[index]) call(index);
                }
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
            std::vector<raw_edge> m_raw_edges;
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
            void add_edge(size_type a, size_type b) { m_raw_edges.push_back({a, b}); }
            /**
             * @brief 获得查询器
             * @tparam GetCut 是否查询割点
             * @tparam GetVBCC 是否查询点双连通分量
             */
            template <bool GetCut, bool GetVBCC>
            Solver<GetCut, GetVBCC> calc() const {
                if (!m_prepared) _prepare();
                Solver<GetCut, GetVBCC> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                return sol;
            }
            /**
             * @brief 查询点双连通分量
             */
            std::vector<std::vector<size_type>> get_vbccs() const {
                if (!m_prepared) _prepare();
                Solver<false, true> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<std::vector<size_type>> res;
                res.reserve(sol.m_vbcc_cnt);
                sol.do_for_each_vbcc([&](size_type *first, size_type *last) { res.emplace_back(first, last); });
                return res;
            }
            /**
             * @brief 查询割点
             */
            std::vector<size_type> get_cuts() const {
                if (!m_prepared) _prepare();
                Solver<true, false> sol(m_vertex_cnt, m_raw_edges.size());
                sol.run(*this);
                std::vector<size_type> res;
                res.reserve(sol.m_cut_cnt);
                sol.do_for_each_cut([&](size_type index) { res.push_back(index); });
                return res;
            }
        };
    }
}

#endif
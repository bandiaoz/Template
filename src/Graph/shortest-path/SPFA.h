#ifndef __OY_SPFA__
#define __OY_SPFA__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

/**
 * @brief SPFA
 * @example OY::SPFA::Graph<int> G(n, m);
 *          OY::SPFA::Solver<SemiGroup, CountType, Compare, GetPath> sol(vertex_cnt);
 */
namespace OY {
    namespace SPFA {
        using size_type = uint32_t;
        template <typename Tp, typename CountType, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            CountType m_cnt, m_offer;
            size_type m_from;
            bool m_inside;
        };
        template <typename Tp, typename CountType>
        struct DistanceNode<Tp, CountType, false> {
            Tp m_val;
            CountType m_cnt, m_offer;
            bool m_inside;
        };
        template <typename Tp>
        struct DistanceNode<Tp, void, true> {
            Tp m_val;
            size_type m_from;
            bool m_inside;
        };
        template <typename Tp>
        struct DistanceNode<Tp, void, false> {
            Tp m_val;
            bool m_inside;
        };
        template <typename Tp, bool IsNumeric = std::is_integral<Tp>::value || std::is_floating_point<Tp>::value>
        struct SafeInfinite {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max() / 2; }
        };
        template <typename Tp>
        struct SafeInfinite<Tp, false> {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max(); }
        };
        /**
         * @brief 加法半群，记录路径答案
         * @tparam ValueType 边权类型
         * @tparam SumType 路径聚合信息类型
         * @tparam Inf 默认不可到达的值
         */
        template <typename ValueType, typename SumType = ValueType, typename Compare = std::less<SumType>, SumType Inf = SafeInfinite<SumType>::max()>
        struct AddGroup {
            using value_type = ValueType;
            using sum_type = SumType;
            using compare_type = Compare;
            static sum_type op(const sum_type &x, const value_type &y) { return x + y; }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return Inf; }
        };
        template <typename ValueType, typename Compare = std::less<ValueType>, ValueType Inf = SafeInfinite<ValueType>::max()>
        struct MaxGroup {
            using value_type = ValueType;
            using sum_type = ValueType;
            using compare_type = Compare;
            static sum_type op(const sum_type &x, const value_type &y) { return std::max(x, y); }
            static sum_type identity() { return {}; }
            static sum_type infinite() { return Inf; }
        };
        template <typename Group, typename CountType = void, bool GetPath = false>
        struct Solver {
            using group = Group;
            using value_type = typename group::value_type;
            using sum_type = typename group::sum_type;
            using compare_type = typename group::compare_type;
            using node = DistanceNode<sum_type, CountType, GetPath>;
            static constexpr bool has_count = !std::is_void<CountType>::value;
            using count_type = typename std::conditional<has_count, CountType, bool>::type;
            size_type m_vertex_cnt, m_head, m_tail;
            std::vector<node> m_distance;
            std::vector<size_type> m_queue;
            size_type _pop() {
                size_type i = m_queue[m_head++];
                if (m_head == m_vertex_cnt) m_head = 0;
                m_distance[i].m_inside = false;
                return i;
            }
            static sum_type infinite() { return group::infinite(); }
            Solver(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt), m_head(0), m_tail(0), m_distance(vertex_cnt), m_queue(vertex_cnt) {
                for (size_type i = 0; i != m_vertex_cnt; i++) {
                    m_distance[i].m_val = infinite();
                    if constexpr (GetPath) m_distance[i].m_from = -1;
                }
            }
            /**
             * @brief 设置从 `source` 到 `i` 的距离为 `dis`，一般用于设置源点
             */
            void set_distance(size_type i, const sum_type &dis, count_type cnt = 1) {
                m_distance[i].m_val = dis;
                if constexpr (has_count) m_distance[i].m_cnt = m_distance[i].m_offer = cnt;
                if (!m_distance[i].m_inside) {
                    m_distance[i].m_inside = true, m_queue[m_tail++] = i;
                    if (m_tail == m_vertex_cnt) m_tail = 0;
                }
            }
            template <typename Traverser>
            bool run(Traverser &&traverser) {
                for (size_type i = 0; i != m_vertex_cnt && m_distance[m_queue[m_head]].m_inside; i++) {
                    size_type len = m_tail <= m_head ? m_tail + m_vertex_cnt - m_head : m_tail - m_head;
                    while (len--) {
                        size_type from = _pop();
                        traverser(from, [&](size_type to, const value_type &dis) {
                            sum_type to_dis = group::op(m_distance[from].m_val, dis);
                            if constexpr (has_count) {
                                if (compare_type()(to_dis, m_distance[to].m_val)) {
                                    m_distance[to].m_val = to_dis, m_distance[to].m_cnt = m_distance[to].m_offer = m_distance[from].m_offer;
                                    if constexpr (GetPath) m_distance[to].m_from = from;
                                    if (!m_distance[to].m_inside) {
                                        m_distance[to].m_inside = true, m_queue[m_tail++] = to;
                                        if (m_tail == m_vertex_cnt) m_tail = 0;
                                    }
                                } else if (!compare_type()(m_distance[to].m_val, to_dis)) {
                                    m_distance[to].m_cnt += m_distance[from].m_offer, m_distance[to].m_offer += m_distance[from].m_offer;
                                    if (!m_distance[to].m_inside) {
                                        m_distance[to].m_inside = true, m_queue[m_tail++] = to;
                                        if (m_tail == m_vertex_cnt) m_tail = 0;
                                    }
                                }
                            } else if (compare_type()(to_dis, m_distance[to].m_val)) {
                                m_distance[to].m_val = to_dis;
                                if constexpr (GetPath) m_distance[to].m_from = from;
                                if (!m_distance[to].m_inside) {
                                    m_distance[to].m_inside = true, m_queue[m_tail++] = to;
                                    if (m_tail == m_vertex_cnt) m_tail = 0;
                                }
                            }
                        });
                        if constexpr (has_count) m_distance[from].m_offer = 0;
                    }
                }
                return !m_distance[m_queue[m_head]].m_inside;
            }
            template <typename Callback>
            void trace(size_type target, Callback &&call) const {
                size_type prev = m_distance[target].m_from;
                if (~prev) trace(prev, call), call(prev, target);
            }
            /**
             * @brief 查询从 source 到 target 的最短路径信息（`sum_type`），不可到达返回 `infinite()`
             */
            const sum_type &query(size_type target) const { return m_distance[target].m_val; }
            /**
             * @brief 如果 `has_count`，则查询从 source 到 target 的最短路径数量，否则返回是否可到达
             */
            count_type query_count(size_type target) const {
                if constexpr (has_count)
                    return m_distance[target].m_cnt;
                else
                    return compare_type()(m_distance[target].m_val, infinite());
            }
        };
        template <typename Tp>
        struct Graph {
            struct raw_edge {
                size_type m_from, m_to;
                Tp m_dis;
            };
            struct edge {
                size_type m_to;
                Tp m_dis;
            };
            size_type m_vertex_cnt;
            mutable bool m_prepared;
            mutable std::vector<size_type> m_starts;
            mutable std::vector<edge> m_edges;
            std::vector<raw_edge> m_raw_edges;
            template <typename Callback>
            void operator()(size_type from, Callback &&call) const {
                auto *first = m_edges.data() + m_starts[from], *last = m_edges.data() + m_starts[from + 1];
                for (auto it = first; it != last; ++it) call(it->m_to, it->m_dis);
            }
            void _prepare() const {
                for (size_type i = 1; i != m_vertex_cnt + 1; i++) m_starts[i] += m_starts[i - 1];
                m_edges.resize(m_starts.back());
                auto cursor = m_starts;
                for (auto &e : m_raw_edges) m_edges[cursor[e.m_from]++] = {e.m_to, e.m_dis};
                m_prepared = true;
            }
            Graph(size_type vertex_cnt = 0, size_type edge_cnt = 0) { 
                resize(vertex_cnt, edge_cnt); 
            }
            void resize(size_type vertex_cnt, size_type edge_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_prepared = false, m_raw_edges.clear(), m_raw_edges.reserve(edge_cnt);
                m_starts.assign(m_vertex_cnt + 1, {});
            }
            /**
             * @brief 添加一条从 a 到 b 的长度为 dis 的有向边
             * @note 如果需要无向边，需要按正反两个方向进行加边
             */
            void add_edge(size_type a, size_type b, Tp dis) { m_starts[a + 1]++, m_raw_edges.push_back({a, b, dis}); }
            /**
             * @brief 获得以 `source` 为源点的解答器
             * @tparam SemiGroup 路径信息半群
             * @tparam CountType 路径数量的类型
             * @tparam Compare 路径长度比较器
             * @tparam GetPath 是否需要记录路径
             * @param target 如果为 -1，则计算 source 到所有点的最短路径
             * @return 返回解答器和是否最短路计算是否成功，如果有负环则返回 false
             */
            template <typename Group = AddGroup<Tp>, typename CountType = void, bool GetPath = false>
            std::pair<Solver<Group, CountType, GetPath>, bool> calc(size_type source) const {
                if (!m_prepared) _prepare();
                auto res = std::make_pair(Solver<Group, CountType, GetPath>(m_vertex_cnt), false);
                res.first.set_distance(source, Group::identity());
                res.second = res.first.run(*this);
                return res;
            }
            /**
             * @brief 是否存在负环
             * @tparam SemiGroup 路径信息半群
             * @tparam Compare 路径长度比较器
             * @param source 源点
             */
            template <typename Group = AddGroup<Tp>>
            bool has_negative_cycle(size_type source) const {
                if (!m_prepared) _prepare();
                Solver<Group> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
                return !sol.run(*this);
            }
            /**
             * @brief 获得从 source 到 target 的最短路径
             * @tparam SemiGroup 路径信息半群
             * @tparam Compare 路径长度比较器
             */
            template <typename Group = AddGroup<Tp>>
            std::vector<size_type> get_path(size_type source, size_type target) const {
                if (!m_prepared) _prepare();
                std::vector<size_type> res;
                Solver<Group, void, true> sol(m_vertex_cnt);
                sol.set_distance(source, Group::identity());
                if (!sol.run(*this)) return res;
                res.push_back(source);
                sol.trace(target, [&](size_type from, size_type to) { res.push_back(to); });
                return res;
            }
        };
    }
}

#endif
#ifndef __OY_RAYSEG__
#define __OY_RAYSEG__

#include <algorithm>
#include <cstdint>
#include <numeric>

/**
 * @brief 树上最长线段射线
 * @note 射线：从当前节点出发的射线长度，路径的一端已经被固定；而线段不限定从当前节点出发的路径长度
 * @tparam Tree 树的类型，通过 `decltype(tree)` 获取
 * @tparam Tp 路径长度的类型
 * @tparam MAX_VERTEX 最大节点数
 * @example OY::RAYSEG::Table<decltype(tree), Tp, MAX_VERTEX> sol(&tree);
 */
namespace OY {
    namespace RAYSEG {
        using size_type = uint32_t;
        struct Ignore {
            template <typename... Args>
            bool operator()(Args...) const { return true; }
        };
        template <typename Tree, typename Tp, size_type MAX_VERTEX>
        struct Diameter {
            struct node {
                Tp m_val;
                size_type m_parent;
            };
            static node s_buffer[MAX_VERTEX];
            template <typename Judger>
            static void _dfs1(Tree *tree, size_type a, size_type p, const Tp &cur_dis, Judger &&filter) {
                s_buffer[a].m_val = cur_dis;
                tree->do_for_each_adj_edge(a, [&](size_type to, const Tp &dis) { if (filter(to) && to != p) _dfs1(tree, to, a, cur_dis + dis, filter); });
            }
            template <typename Judger>
            static void _dfs2(Tree *tree, size_type a, size_type p, const Tp &cur_dis, Judger &&filter) {
                s_buffer[a].m_val = cur_dis, s_buffer[a].m_parent = p;
                tree->do_for_each_adj_edge(a, [&](size_type to, const Tp &dis) { if (filter(to) && to != p) _dfs2(tree, to, a, cur_dis + dis, filter); });
            }
            template <typename Callback>
            static void _trace(size_type cur, Callback &&call) {
                if (~s_buffer[cur].m_parent) _trace(s_buffer[cur].m_parent, call);
                call(cur);
            }
            /**
             * @brief 获取树的直径
             * @param cur 从哪个节点开始
             * @param filter 可以过滤掉某些节点，函数签名为 `bool filter(size_type u);`
             * @param call 对直径上的每个节点进行操作，函数签名为 `void call(size_type u);`
             */
            template <typename Judger = Ignore, typename Callback = Ignore>
            static Tp solve(Tree *tree, size_type cur, Judger &&filter = Judger(), Callback &&call = Callback()) {
                for (size_type i = 0; i != tree->vertex_cnt(); i++) s_buffer[i].m_val = 0, s_buffer[i].m_parent = -1;
                _dfs1(tree, cur, -1, 0, filter);
                size_type source = cur, target = cur; // 存储直径的两端
                for (size_type i = 0; i != tree->vertex_cnt(); i++)
                    if (s_buffer[i].m_val > s_buffer[source].m_val) source = i;
                s_buffer[source].m_val = 0;
                _dfs2(tree, source, -1, 0, filter);
                for (size_type i = 0; i != tree->vertex_cnt(); i++)
                    if (s_buffer[i].m_val > s_buffer[target].m_val) target = i;
                if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) {
                    _trace(target, call);
                }
                return s_buffer[target].m_val;
            }
        };
        template <typename Tree, typename Tp, size_type MAX_VERTEX>
        typename Diameter<Tree, Tp, MAX_VERTEX>::node Diameter<Tree, Tp, MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <typename Tree, typename Tp, size_type MAX_VERTEX>
        struct Table {
            /**
             * @param m_val[a].m_ray 存储以 `a` 为一端的最长射线长度前三名，每个子树（相邻连通块）只能贡献一次答案
             * @param m_val[a].m_seg 存储不经过 `a` 的最长线段长度前两名，每个子树（相邻联通块）只能贡献一次答案
             * @param m_val[a].m_down_ray 存储以 `a` 一端的向下的射线最长长度
             * @param m_val[a].m_down_seg 存储 `a` 下方的不越过 `a` 的线段的线段最长长度。注意，不越过，但是可以经过。即不含 `a` 上方的节点。
             * @param m_val[a].m_up_ray 存储以 `a` 作为一端的向上的射线的最长长度
             * @param m_val[a].m_up_seg 存储 `a` 上方的不越过 `a` 的线段的线段最长长度。注意，不越过，但是可以经过。即不含 `a` 下方的节点。
             */
            struct node {
                Tp m_ray[3], m_seg[2], m_down_ray, m_down_seg, m_up_ray, m_up_seg;
            };
            struct RaySeg {
                Tp m_ray, m_seg;
            };
            static node s_buffer[MAX_VERTEX];
            static size_type s_use_count;
            Tree *m_tree;
            node *m_val;
            void _add_ray(size_type i, const Tp &len) {
                if (len > m_val[i].m_ray[0])
                    m_val[i].m_ray[2] = m_val[i].m_ray[1], m_val[i].m_ray[1] = m_val[i].m_ray[0], m_val[i].m_ray[0] = len;
                else if (len > m_val[i].m_ray[1])
                    m_val[i].m_ray[2] = m_val[i].m_ray[1], m_val[i].m_ray[1] = len;
                else if (len > m_val[i].m_ray[2])
                    m_val[i].m_ray[2] = len;
            }
            void _add_seg(size_type i, const Tp &len) {
                if (len > m_val[i].m_seg[0])
                    m_val[i].m_seg[1] = m_val[i].m_seg[0], m_val[i].m_seg[0] = len;
                else if (len > m_val[i].m_seg[1])
                    m_val[i].m_seg[1] = len;
            }
            void _add_down_ray(size_type i, const Tp &len) {
                m_val[i].m_down_ray = std::max(m_val[i].m_down_ray, len);
                _add_ray(i, len);
            }
            void _add_down_seg(size_type i, const Tp &len) {
                m_val[i].m_down_seg = std::max(m_val[i].m_down_seg, len);
                _add_seg(i, len);
            }
            void _set_up_ray(size_type i, const Tp &len) { _add_ray(i, m_val[i].m_up_ray = len); }
            void _set_up_seg(size_type i, const Tp &len) { m_val[i].m_up_seg = len; }
            void _dfs1(size_type a, size_type p) {
                m_tree->do_for_each_adj_edge(a, [&](size_type to, const Tp &dis) {
                    if (to != p) {
                        _dfs1(to, a);
                        _add_down_ray(a, m_val[to].m_down_ray + dis);
                        _add_down_seg(a, m_val[to].m_down_seg);
                    }
                });
                m_val[a].m_down_seg = std::max(m_val[a].m_down_seg, m_val[a].m_ray[0] + m_val[a].m_ray[1]);
            }
            void _dfs2(size_type a, size_type p, const Tp &up_ray, const Tp &up_seg) {
                _set_up_ray(a, up_ray), _set_up_seg(a, up_seg);
                m_tree->do_for_each_adj_edge(a, [&](size_type to, const Tp &dis) {
                    if (to != p) {
                        auto ray_seg = max_ray_seg(a, m_val[to].m_down_ray + dis, m_val[to].m_down_seg);
                        _add_seg(to, ray_seg.m_seg);
                        ray_seg.m_seg = std::max(ray_seg.m_seg, ray_seg.m_ray += dis);
                        _dfs2(to, a, ray_seg.m_ray, ray_seg.m_seg);
                    }
                });
            }
            Table(Tree *rooted_tree = nullptr) { reset(rooted_tree); }
            void reset(Tree *rooted_tree) {
                if (!(m_tree = rooted_tree)) return;
                m_val = s_buffer + s_use_count, s_use_count += m_tree->vertex_cnt();
                _dfs1(m_tree->m_root, -1);
                _dfs2(m_tree->m_root, -1, 0, 0);
            }
            /**
             * @brief 查询树的最长射线线段
             * @param a 射线的起点
             * @param except_ray 屏蔽掉的部分所提供的最长射线长度
             * @param except_seg 屏蔽掉的部分所提供的最长线段长度
             * @note 只能屏蔽相邻的一个联通块
             * @return 分别返回射线和线段的长度
             */
            RaySeg max_ray_seg(size_type a, const Tp &except_ray, const Tp &except_seg) const {
                Tp ray = except_ray == m_val[a].m_ray[0] ? m_val[a].m_ray[1] : m_val[a].m_ray[0];
                Tp seg = except_seg == m_val[a].m_seg[0] ? m_val[a].m_seg[1] : m_val[a].m_seg[0];
                Tp rays = except_ray == m_val[a].m_ray[0] ? m_val[a].m_ray[1] + m_val[a].m_ray[2] : (except_ray == m_val[a].m_ray[1] ? m_val[a].m_ray[0] + m_val[a].m_ray[2] : m_val[a].m_ray[0] + m_val[a].m_ray[1]);
                return RaySeg{ray, std::max(seg, rays)};
            }
            /**
             * @brief 查询去掉某子树后的最长射线线段，射线从节点 `a` 出发
             * @param a 射线的起点
             * @param except_son `a` 的子节点
             * @param except_son_edge `a` 到 `except_son` 的边的长度
             * @return 分别返回射线和线段的长度
             */
            RaySeg max_ray_seg_except_son(size_type a, size_type except_son, const Tp &except_son_edge) const { 
                return max_ray_seg(a, m_val[except_son].m_down_ray + except_son_edge, m_val[except_son].m_down_seg); 
            }
            /**
             * @brief 查询从 `a` 出发，向下的射线的最长长度
             */
            Tp query_down_ray(size_type a) const { return m_val[a].m_down_ray; }
            /**
             * @brief 查询 `a` 下方的不越过 `a` 的线段的线段最长长度（即子树内的最长线段长度）
             */
            Tp query_down_seg(size_type a) const { return m_val[a].m_down_seg; }
            /**
             * @brief 查询从 `a` 出发，向上的射线的最长长度
             */
            Tp query_up_ray(size_type a) const { return m_val[a].m_up_ray; }
            /**
             * @brief 查询 `a` 上方的不越过 `a` 的线段的线段最长长度
             */
            Tp query_up_seg(size_type a) const { return m_val[a].m_up_seg; }
        };
        template <typename Tree, typename Tp, size_type MAX_VERTEX>
        typename Table<Tree, Tp, MAX_VERTEX>::node Table<Tree, Tp, MAX_VERTEX>::s_buffer[MAX_VERTEX];
        template <typename Tree, typename Tp, size_type MAX_VERTEX>
        size_type Table<Tree, Tp, MAX_VERTEX>::s_use_count;
    }
}

#endif
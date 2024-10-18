#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

/**
 * @brief 虚树 Virtual Tree
 * @tparam MAX_BUFFER 虚树的最大节点数
 * @example OY::VTREE::VirtualTree<N>::solve_rmqlca(keys.begin(), keys.end(), LCA, call);
 * @note 可以手动将根节点 `root` 加入关键序列，这样虚树中的根节点就是 `root`
 */
namespace OY {
    namespace VTREE {
        using size_type = uint32_t;
        template <size_type MAX_BUFFER>
        struct VirtualTree {
            struct node {
                size_type m_id, m_dfn;
            };
            static node s_buffer[MAX_BUFFER];
            static size_type s_key_buffer[MAX_BUFFER];
            /**
             * @brief 建立虚树，通过 `call` 回调函数「从下到上顺序」返回虚树中的边
             * @param first 关键序列的起始迭代器
             * @param last 关键序列的终止迭代器
             * @param dfn_getter 获得关键序列中节点的 dfn 编号的函数对象
             * @param lca_getter 获得两个节点的最近公共祖先的函数对象
             * @param call 虚树中边的回调函数，接受两个参数，分别是边的两个端点 `(u, f)`，其中 `f` 是 `u` 的父节点
             */
            template <typename Iterator, typename DFNGetter, typename LCAGetter, typename Callback>
            static void solve(Iterator first, Iterator last, DFNGetter &&dfn_getter, LCAGetter &&lca_getter, Callback &&call) {
                size_type n = last - first, len = 0;
                auto dfn_comp = [&](size_type x, size_type y) { return dfn_getter(x) < dfn_getter(y); };
                std::copy(first, last, s_key_buffer);
                std::sort(s_key_buffer, s_key_buffer + n, dfn_comp);
                for (size_type i = 0; i < n; i++) {
                    size_type a = s_key_buffer[i];
                    if (len) {
                        size_type lca = lca_getter(a, s_buffer[len - 1].m_id), dfn = dfn_getter(lca);
                        while (s_buffer[len - 1].m_dfn > dfn) {
                            if (len >= 2 && s_buffer[len - 2].m_dfn >= dfn) {
                                call(s_buffer[len - 1].m_id, s_buffer[len - 2].m_id);
                                len--;
                            } else {
                                call(s_buffer[len - 1].m_id, lca);
                                s_buffer[len - 1] = {lca, dfn};
                                break;
                            }
                        }
                    }
                    s_buffer[len++] = {a, dfn_getter(a)};
                }
                while (--len) call(s_buffer[len].m_id, s_buffer[len - 1].m_id);
            }
            template <typename Iterator, typename RMQLCA, typename Callback>
            static void solve_rmqlca(Iterator first, Iterator last, RMQLCA &&rmqlca, Callback &&call) {
                solve(first, last, [&](size_type a) { return rmqlca.m_dfn[a]; }, 
                        [&](size_type a, size_type b) { return rmqlca.calc(a, b); }, call);
            }
            template <typename Iterator, typename HLD, typename Callback>
            static void solve_hld(Iterator first, Iterator last, HLD &&hld, Callback &&call) {
                solve(first, last, [&](size_type a) { return hld.m_info[a].m_dfn; }, 
                        [&](size_type a, size_type b) { return hld.lca(a, b); }, call);
            }
        };
        template <size_type MAX_BUFFER>
        typename VirtualTree<MAX_BUFFER>::node VirtualTree<MAX_BUFFER>::s_buffer[MAX_BUFFER];
        template <size_type MAX_BUFFER>
        size_type VirtualTree<MAX_BUFFER>::s_key_buffer[MAX_BUFFER];
    }
}
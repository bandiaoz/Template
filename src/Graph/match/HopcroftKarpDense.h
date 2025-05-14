#ifndef __OY_HOPCROFTKARP_DENSE__
#define __OY_HOPCROFTKARP_DENSE__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <vector>

#if __has_include(<bit>)
#include <bit>
#else
#include "src/Misc/std_bit.h"
#endif

/**
 * @brief Hopcroft Karp 算法（压位版）。
 * @example OY::HKDense::Graph graph(left_cnt, right_cnt, edge_cnt);
 */
namespace OY {
    namespace HKDense {
        using size_type = uint32_t;
        using mask_type = uint64_t;
        static constexpr size_type MASK_SIZE = sizeof(mask_type) << 3, MASK_WIDTH = MASK_SIZE / 32 + 4;
        std::mt19937 hk_rand;
        struct Graph {
            size_type m_left_cnt, m_right_cnt, m_u;
            mutable bool m_prepared;
            std::vector<size_type> m_perm, m_id, m_left_match, m_right_match;
            std::vector<mask_type> m_adj_buf;
            Graph() = default;
            Graph(size_type left_cnt, size_type right_cnt, size_type edge_cnt = 0) { resize(left_cnt, right_cnt, edge_cnt); }
            void resize(size_type left_cnt, size_type right_cnt, size_type edge_cnt) {
                if (!(m_left_cnt = left_cnt) || !(m_right_cnt = right_cnt)) return;
                m_prepared = false;
                m_perm.resize(m_left_cnt), m_id.resize(m_left_cnt + 1);
                std::iota(m_perm.begin(), m_perm.end(), 0);
                std::shuffle(m_perm.begin(), m_perm.end(), hk_rand);
                m_id[0] = -1;
                for (size_type i = 0; i != m_left_cnt; i++) m_id[m_perm[i] + 1] = i;
                m_left_match.assign(m_left_cnt, -1), m_right_match.assign(m_right_cnt, -1);
                m_u = (m_right_cnt + MASK_SIZE - 1) >> MASK_WIDTH;
                m_adj_buf.resize(m_u * m_left_cnt);
            }
            /**
             * @brief 添加一条从左侧 `left` 节点到右侧 `right` 节点的边
             * @param left 左侧节点编号 `0 <= left < m_left_cnt`
             * @param right 右侧节点编号 `0 <= right < m_right_cnt`
             */
            void add_edge(size_type left, size_type right) { m_adj_buf[m_perm[left] * m_u + (right >> MASK_WIDTH)] |= mask_type(1) << (right & (MASK_SIZE - 1)); }
            /**
             * @brief 计算最大匹配
             * @note 时间复杂度为 $O(\frac {n^{2.5}} \omega)$
             */
            size_type calc() {
                std::vector<size_type> buf(m_left_cnt * 2 + m_right_cnt);
                size_type *dist = buf.data(), *q = buf.data() + m_left_cnt, *from = buf.data() + m_left_cnt * 2, res = 0;
                std::vector<mask_type> can_buf(m_u);
                mask_type *can = can_buf.data();
                auto bfs = [&]() {
                    std::fill_n(dist, m_left_cnt, -1);
                    size_type l = 0, r = 0;
                    for (size_type a = 0; a != m_left_cnt; a++)
                        if (!~m_left_match[a]) dist[a] = 0, q[r++] = a;
                    while (l != r) {
                        size_type a = q[l++];
                        mask_type *adj = m_adj_buf.data() + a * m_u;
                        for (size_type bi = 0; bi != m_u; bi++)
                            if (mask_type mask = adj[bi])
                                do {
                                    size_type ctz = std::countr_zero(mask), b = (bi << MASK_WIDTH) + ctz;
                                    size_type a2 = m_right_match[b];
                                    if (~a2 && dist[a2] > dist[a] + 1) dist[a2] = dist[a] + 1, q[r++] = a2;
                                    mask -= mask_type(1) << ctz;
                                } while (mask);
                    }
                };
                auto bfs2 = [&](size_type a) {
                    size_type l = 0, r = 0;
                    q[r++] = a;
                    while (l != r) {
                        size_type a = q[l++];
                        mask_type *adj = m_adj_buf.data() + a * m_u;
                        for (size_type bi = 0; bi != m_u; bi++)
                            if (mask_type mask = can[bi] & adj[bi])
                                do {
                                    size_type ctz = std::countr_zero(mask), b = (bi << MASK_WIDTH) + ctz, br = m_right_match[b];
                                    mask -= mask_type(1) << ctz;
                                    if (!~br) {
                                        while (true) {
                                            m_right_match[b] = a;
                                            std::swap(m_left_match[a], b);
                                            if (!~b) break;
                                            a = from[b];
                                        }
                                        return true;
                                    }
                                    if (dist[br] == dist[a] + 1) can[bi] -= mask_type(1) << ctz, from[b] = a, q[r++] = br;
                                } while (mask);
                    }
                    return false;
                };
                while (true) {
                    bfs();
                    size_type augment = 0;
                    std::fill_n(can, m_u, -1);
                    for (size_type left = 0; left != m_left_cnt; left++) augment += (!~m_left_match[left] && bfs2(left));
                    if (!augment) break;
                    res += augment;
                }
                return res;
            }
            /**
             * @brief 查询左侧结点匹配的右侧结点
             * @param left 左侧结点编号 `0 <= left < m_left_cnt`
             * @return 右侧结点编号
             */
            size_type find_right(size_type left) const { return m_left_match[m_perm[left]]; }
            /**
             * @brief 查询右侧结点匹配的左侧结点
             * @param right 右侧结点编号 `0 <= right < m_right_cnt`
             * @return 左侧结点编号
             */
            size_type find_left(size_type right) const { return m_id[m_right_match[right] + 1]; }
        };
    }
}

#endif
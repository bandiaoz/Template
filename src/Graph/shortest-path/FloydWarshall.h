#pragma once

#include <bitset>
#include <cstdint>
#include <vector>

/**
 * @brief Floyd 传递闭包
 * @example OY::FloydWarshall::Graph<100> G(vertex_cnt);
 */
namespace OY {
    namespace FloydWarshall {
        using size_type = uint32_t;
        template <size_type MAX_VERTEX>
        struct Graph {
            std::vector<std::bitset<MAX_VERTEX>> m_can;
            size_type m_vertex_cnt;
            Graph(size_type vertex_cnt) : m_can(vertex_cnt), m_vertex_cnt(vertex_cnt) {}
            void add_edge(size_type a, size_type b) { m_can[a].set(b); }
            void calc() {
                for (size_type k = 0; k != m_vertex_cnt; k++) {
                    for (size_type i = 0; i != m_vertex_cnt; i++) {
                        if (m_can[i][k]) m_can[i] |= m_can[k];
                    }
                }
            }
            /**
             * @brief 查询是否存在 a -> b 的路径
             */
            bool can(size_type a, size_type b) { return m_can[a][b]; }
        };
    }
}
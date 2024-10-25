#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief Prufer 序列
 * @example 1. 使用 `OY::PRUFER::Tree::encode()` 将无根树编码为 Prufer 序列
 *          2. 使用 `OY::PRUFER::Tree::decode(...)` 静态函数解码 Prufer 序列
 */
namespace OY {
    namespace PRUFER {
        using size_type = uint32_t;
        class Tree {
            struct node {
                size_type m_xor, m_deg;
            };
            size_type m_vertex_cnt;
            std::vector<node> m_info;
        public:
            /**
             * @brief 将 Prufer 序列解码为树
             * @return 当根为 `n - 1` 时树的父亲数组，其中 fa[n - 1] = -1
             */
            template <typename Iterator>
            static std::vector<size_type> decode(Iterator first, Iterator last) {
                size_type n = last - first + 2;
                std::vector<size_type> res(n), deg(n);
                for (auto it = first; it != last; ++it) deg[*it]++;
                auto it = first;
                for (size_type i = 0; i != n; i++)
                    if (!deg[i]) {
                        size_type cur = i;
                        do {
                            if (it == last)
                                res[cur] = n - 1;
                            else
                                res[cur] = *it++;
                            cur = res[cur];
                        } while (!--deg[cur] && cur <= i);
                    }
                res[n - 1] = -1;
                return res;
            }
            Tree(size_type vertex_cnt = 0) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                if (!(m_vertex_cnt = vertex_cnt)) return;
                m_info.resize(m_vertex_cnt);
            }
            void add_edge(size_type a, size_type b) { m_info[a].m_deg++, m_info[b].m_deg++, m_info[a].m_xor ^= b, m_info[b].m_xor ^= a; }
            /**
             * @brief 将树编码为长度 `n - 2` 的 Prufer 序列
             */
            std::vector<size_type> encode() {
                std::vector<size_type> res;
                res.reserve(m_vertex_cnt - 1);
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    if (m_info[i].m_deg == 1) {
                        size_type cur = i;
                        do m_info[m_info[cur].m_xor].m_xor ^= cur, res.push_back(cur = m_info[cur].m_xor);
                        while (--m_info[cur].m_deg == 1 && cur <= i);
                    }
                res.pop_back();
                return res;
            }
        };
    }
}
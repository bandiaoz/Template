#ifndef __OY_TWOSAT__
#define __OY_TWOSAT__

#include "src/Graph/connectivity/Tarjan_scc.h"

/**
 * @brief 2-SAT
 */
namespace OY {
    namespace TWOSAT {
        using size_type = uint32_t;
        struct Graph {
            SCC::Graph m_scc;
            std::vector<bool> m_state;
            Graph() = default;
            Graph(size_type vertex_cnt, size_type clause_cnt) { 
                resize(vertex_cnt, clause_cnt); 
            }
            void resize(size_type vertex_cnt, size_type clause_cnt) {
                m_scc.resize(vertex_cnt << 1, clause_cnt << 1);
                m_state.assign(vertex_cnt, false);
            }
            /**
             * @brief `i` is `i_val` or `j` is `j_val`
             */
            void add_clause(size_type i, bool i_val, size_type j, bool j_val) { 
                m_scc.add_edge(i * 2 + !i_val, j * 2 + j_val);
                m_scc.add_edge(j * 2 + !j_val, i * 2 + i_val); 
            }
            bool calc() {
                auto sol = m_scc.calc();
                for (size_type i = 0; i != m_scc.m_vertex_cnt; i += 2) {
                    if (sol.query(i) == sol.query(i ^ 1)) return false;
                    m_state[i >> 1] = sol.query(i) < sol.query(i ^ 1);
                }
                return true;
            }
            bool query(size_type i) const { return m_state[i]; }
        };
    }
};

#endif
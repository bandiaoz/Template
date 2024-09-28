#pragma once

#include "src/Math/NumberTheory/ExtendedEuclidean.h"

namespace OY {
    struct ChineseRemainderTheorem_ex {
        struct node {
            uint64_t m_rem = 0, m_div = 1;
        };
        node m_res;
        bool add(uint64_t remain, uint64_t divisor) {
            auto res = ExtenedEuclideanSolver::solve(m_res.m_div, divisor, remain - m_res.m_rem);
            if (!res.m_flag) return false;
            m_res.m_rem += m_res.m_div * res.m_coef1, m_res.m_div = m_res.m_div / res.m_gcd * divisor, m_res.m_rem %= m_res.m_div;
            return true;
        }
        node query() const { return m_res; }
    };
}
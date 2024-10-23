#pragma once

#include "src/Math/NumberTheory/ExtendedEuclidean.h"

/**
 * @brief 中国剩余定理 Chinese Remainder Theorem
 */
namespace OY {
    struct ChineseRemainderTheorem_ex {
        struct node {
            uint64_t m_rem = 0, m_div = 1;
        };
        node m_res;
        /**
         * @brief 添加一个余数方程 `x ≡ remain (mod divisor)`
         * @return 如果和之前的添加有冲突，返回 `false`
         */
        bool add(uint64_t remain, uint64_t divisor) {
            auto res = ExtenedEuclideanSolver::solve(m_res.m_div, divisor, remain - m_res.m_rem);
            if (!res.m_flag) return false;
            m_res.m_rem += m_res.m_div * res.m_coef1;
            m_res.m_div = m_res.m_div / res.m_gcd * divisor;
            m_res.m_rem %= m_res.m_div;
            return true;
        }
        /**
         * @brief 返回符合要求的最小非负整数解和答案周期
         */
        node query() const { return m_res; }
    };
}
#ifndef __OY_LINEARMODEQUATIONS__
#define __OY_LINEARMODEQUATIONS__

#include "src/Math/NumberTheory/ChineseRemainderTheorem_ex.h"

/**
 * @brief 线性同余方程组
 */
namespace OY {
    struct LinearModEquations {
        ChineseRemainderTheorem_ex m_crt;
        /**
         * @brief 添加一个同余方程 `k * x ≡ rem (mod divisor)`
         * @return 如果和之前的添加有冲突，返回 `false`
         */
        bool add_equation(uint64_t k, uint64_t rem, uint64_t divisor) {
            auto res = ExtenedEuclideanSolver::solve(k, divisor, rem);
            return res.m_flag && m_crt.add(res.m_coef1, divisor / res.m_gcd);
        }
        /**
         * @brief 查询符合要求的最小非负整数解和答案周期
         */
        ChineseRemainderTheorem_ex::node query() const { 
            return m_crt.query(); 
        }
    };
}

#endif
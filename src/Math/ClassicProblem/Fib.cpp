#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/LinearAlgebra/StaticMatrix.h"
#include "src/Math/Misc/power.hpp"

using Z = OY::mint1000000007;

/**
 * @brief 斐波那契数列
 * @note f[1] = 1, f[2] = 1, f[n] = f[n - 1] + f[n - 2]
 * @link https://www.luogu.com.cn/problem/P1962
 */
auto f(int64_t n) -> Z {
    if (n <= 2) return Z(1);   
    OY::StaticMatrix<Z, 2, 2> base{{{1, 1}, {1, 0}}};
    auto res = power(base, n - 2);
    return res[0][0] * f(2) + res[0][1] * f(1);
}
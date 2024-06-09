#include <bits/stdc++.h>
#include "../Others/Z.hpp"
#include "../Math/Matrix.cpp"

struct MOD { static int value; };
int MOD::value = 1'000'000'007;
using Z = ModInt<MOD>;

/**
 * @brief 斐波那契数列
 * @note f[1] = 1, f[2] = 1, f[n] = f[n - 1] + f[n - 2]
 * @link https://www.luogu.com.cn/problem/P1962
 */
auto f(int64_t n) -> Z {
    if (n <= 2) return Z(1);   
    Matrix<Z> base(2, 2);
    base[0][0] = base[0][1] = base[1][0] = 1;
    Matrix res = power(base, n - 2);
    return res[0][0] * f(2) + res[0][1] * f(1);
}
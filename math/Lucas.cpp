#include <bits/stdc++.h>
#include "../Others/Z.hpp"
#include "./Comb.cpp"

struct MOD { static int value; };
int MOD::value = 1'000'007;
using Z = ModInt<MOD>;

/**
 * @brief Lucas 定理
 * @note binom(n, m) % P = binom(n % P, m % P) * binom(n / P, m / P) % P
 * @note 注意 comb.init(P - 1)
 * @link https://www.luogu.com.cn/problem/P3807
 */
auto lucas(Comb<Z> &comb, int n, int m, int P) -> Z {
    if (m == 0) return 1;
    return comb.binom(n % P, m % P) * lucas(comb, n / P, m / P, P);
}
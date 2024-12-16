#include <bits/stdc++.h>
#include "src/Misc/DigitDP.h"

using ll = long long;
/*
[P2657 [SCOI2009] windy 数](https://www.luogu.com.cn/problem/P2657)
[status](https://www.luogu.com.cn/record/193758241)
*/
/**
 * 数位 dp 模板
 * 不含前导零，相邻两个数字之差至少为 2 的正整数为 windy 数，求 [l, r] 里 windy 数的数量
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto solve = [&](int n) {
        using OY::DIGITDP::IntStr10;
        auto transfer = [&](uint32_t old, uint32_t len, uint32_t c) -> uint32_t {
            if (old != -1 && c + 1 >= old && c <= old + 1) return -1;
            return c;
        };
        auto map = [&](auto state, auto len) {
            return 1;
        };
        auto sol = OY::DIGITDP::AppendHighSolver<int, IntStr10>();
        // auto sol = OY::DIGITDP::AppendLowSolver<int, IntStr10>();
        return sol.solve(n, 10, transfer, map);
    };

    ll l, r;
    std::cin >> l >> r;
    std::cout << solve(r) - solve(l - 1) << '\n';

    return 0;
}
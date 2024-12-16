#include <bits/stdc++.h>
#include "src/Misc/DigitDP.h"
#include "src/Math/Modular/StaticModInt32.h"

using ll = long long;

using Z = OY::mint1000000007;
using OY::DIGITDP::IntStr10;

/*
[P3413 SAC#1 - 萌数](https://www.luogu.com.cn/problem/P3413)
[status](https://www.luogu.com.cn/record/193761398)
*/
/**
 * 数位 dp 模板
 * 求 [l, r] 范围内萌数的数量，萌数定义为：存在长度至少为 2 的回文子串
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // OY::DIGITDP::AppendLowSolver<Z, IntStr10> sol;
    OY::DIGITDP::AppendHighSolver<Z, IntStr10> sol;
    auto solve = [&](std::string &&n) -> Z {
        auto transfer = [&](int old, int len, int c) -> uint32_t {
            if (old == 100) return 100;
            if (old == -1) return c * 10;
            if (len == 1) return old == c * 10 ? 100 : c * 10 + old / 10;
            int first = old / 10, second = old % 10;
            if (c == first || c == second) return 100;
            return c * 10 + first;
        };
        auto map = [&](int state, int len) -> Z {
            return Z::raw(state == 100);
        };
        return sol.solve(std::move(n), 101, transfer, map);
    };

    std::string l, r;
    std::cin >> l >> r;
    std::cout << solve(std::move(r)) - solve(OY::DIGITDP::prev_number_base10(l)) << "\n";

    return 0;
}
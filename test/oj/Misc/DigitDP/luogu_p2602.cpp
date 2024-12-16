#include <bits/stdc++.h>
#include "src/Misc/DigitDP.h"

using ll = long long;

/*
[P2602 [ZJOI2010] 数字计数](https://www.luogu.com.cn/problem/P2602)
[status](https://www.luogu.com.cn/record/193759816)
*/
/**
 * 数位 dp 模板
 * 在十进制下，求 [l, r] 范围内数字 0~9 出现次数
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using OY::DIGITDP::IntStr10;
    OY::DIGITDP::AppendHighSolver<ll, IntStr10> sol;
    // OY::DIGITDP::AppendLowSolver<ll, IntStr10> sol;
    auto solve = [&](ll n, int t) {
        auto transfer = [&](uint32_t old, uint32_t len, uint32_t c) -> uint32_t {
            if (old == -1) old = 0;
            return old + (c == t);
        };
        auto map = [&](uint32_t state, uint32_t len) {
            return state;
        };
        return sol.solve(n, 14, transfer, map);
    };

    ll l, r;
    std::cin >> l >> r;
    for (int i = 0; i < 10; i++) {
        std::cout << solve(r, i) - solve(l - 1, i) << ' ';
    }

    return 0;
}
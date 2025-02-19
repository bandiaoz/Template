#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Misc/DigitDP.h"

using Z = OY::mint1000000007;

/*
[P3311 [SDOI2014] 数数](https://www.luogu.com.cn/problem/P3311)
[status](https://www.luogu.com.cn/record/202879561)
*/
/**
 * 给定 m 个模式串，求 [1, n] 中，有多少个数字不包含这些模式串
 * 
 * 数位 dp 模板
 * 由于要多模式匹配，所以上 ac 自动机
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string n;
    int m;
    std::cin >> n >> m;

    OY::ACAM<10> ac;
    const int N = 1500;
    ac.reserve(N);
    std::vector<int> bad(N + 1);
    for (int i = 0; i < m; i++) {
        std::string s;
        std::cin >> s;
        int p = ac.insert(s.begin(), s.end(), [&](char c) { return c - '0'; });
        bad[p] = true;
    }

    ac.prepare();
    ac.do_for_failing_nodes([&](auto u) {
        bad[u] |= bad[ac.query_fail(u)];
    });

    using OY::DIGITDP::IntStr10;
    auto transfer = [&](auto old, auto len, auto c) -> int {
        if (old == -1) old = 0;
        auto now = ac.next(old, c);
        return bad[now] ? -1 : now;
    };
    auto map = [&](auto state, auto len) -> Z {
        return Z::raw(1);
    };

    std::cout << OY::DIGITDP::AppendLowSolver<Z, IntStr10>().solve(n, ac.m_data.size(), transfer, map) << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"

/*
[P3388 【模板】割点（割顶）](https://www.luogu.com.cn/problem/P3388)
[status](https://www.luogu.com.cn/record/179172235)
*/
/**
 * 本题为割点模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    OY::VBCC::Graph graph(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto solver = graph.calc<true, false>();
    std::cout << solver.m_cut_cnt << '\n';
    solver.do_for_each_cut([](int index) { std::cout << index + 1 << ' '; });

    return 0;
}
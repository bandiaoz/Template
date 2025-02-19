#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"

/*
[P8435 【模板】点双连通分量](https://www.luogu.com.cn/problem/P8435)
[status](https://www.luogu.com.cn/record/179172972)
*/
/**
 * 本题为点双连通分量模板题
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

    auto solver = graph.calc<false, true>();
    std::cout << solver.m_vbcc_cnt << '\n';
    solver.do_for_each_vbcc([](uint32_t *first, uint32_t *last) {
        std::cout << std::distance(first, last) << ' ';
        for (auto it = first; it != last; ++it) {
            std::cout << *it + 1 << " ";
        }
        std::cout << '\n';
    });

    return 0;
}
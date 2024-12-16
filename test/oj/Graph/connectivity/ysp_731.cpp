#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"

/*
[Biconnected Components](https://judge.yosupo.jp/problem/biconnected_components)(https://github.com/yosupo06/library-checker-problems/issues/731)
*/
/**
 * 本题为 tarjan 算法求点双模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::VBCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        graph.add_edge(u, v);
    }

    auto sol = graph.calc<false, true>();
    std::cout << sol.m_vbcc_cnt << '\n';
    sol.do_for_each_vbcc([&](auto first, auto last) {
        std::cout << last - first;
        for (auto it = first; it != last; ++it) {
            std::cout << ' ' << *it;
        }
        std::cout << '\n';
    });

    return 0;
}
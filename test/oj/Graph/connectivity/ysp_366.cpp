#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_bridge.h"

/*
[Two-Edge-Connected Components](https://judge.yosupo.jp/problem/two_edge_connected_components)(https://github.com/yosupo06/library-checker-problems/issues/366)
[status](https://judge.yosupo.jp/submission/239175)
*/
/**
 * 本题为 tarjan 算法求 ebcc 模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::EBCC::Graph G(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        G.add_edge(u, v);
    }

    auto sol = G.calc<false, true>();

    std::cout << sol.m_ebcc_cnt << '\n';
    sol.do_for_each_ebcc([&](auto first, auto last) {
        std::cout << last - first << " ";
        for (auto it = first; it != last; it++) {
            std::cout << *it << " \n"[it == last - 1];
        }
    });

    return 0;
}
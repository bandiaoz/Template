#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_bridge.h"

/*
[P8436 【模板】边双连通分量](https://www.luogu.com.cn/problem/P8436)
[status](https://www.luogu.com.cn/record/179787865)
*/
/**
 * 本题为边双连通分量模板题
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
        u--, v--;
        G.add_edge(u, v);
    }

    auto sol = G.calc<false, true>();
    std::cout << sol.m_ebcc_cnt << '\n';
    sol.do_for_each_ebcc([&](auto first, auto last) {
        std::cout << last - first << ' ';
        for (auto it = first; it != last; it++) {
            std::cout << *it + 1 << " \n"[it == last - 1];
        }
    });

    return 0;
}
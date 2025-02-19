#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[Strongly Connected Components](https://judge.yosupo.jp/problem/scc)(https://github.com/yosupo06/library-checker-problems/issues/36)
[status](https://judge.yosupo.jp/submission/239157)
*/
/**
 * 给定一张图，求出所有的强连通分量，并按照拓扑序输出
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::SCC::Graph G(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        G.add_edge(u, v);
    }

    auto sol = G.calc();
    auto groups = sol.get_groups();
    std::cout << groups.size() << '\n';
    for (auto group : groups) {
        std::cout << group.size() << ' ';
        for (int i = 0; i < group.size(); i++) {
            std::cout << group[i] << " \n"[i == group.size() - 1];
        }
    }

    return 0;
}
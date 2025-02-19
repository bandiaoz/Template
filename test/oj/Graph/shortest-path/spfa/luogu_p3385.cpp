#include <bits/stdc++.h>
#include "src/Graph/shortest-path/SPFA.h"

/*
[P3385 【模板】负环](https://www.luogu.com.cn/problem/P3385)
[status](https://www.luogu.com.cn/record/185889892)
*/
/**
 * 本题为 Bellman-Ford 算法判负环模板题
 * SPFA 为 Bellman-Ford 算法的上位替代
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    while (t--) {
        int n, m;
        std::cin >> n >> m;
        OY::SPFA::Graph<int> G(n, m * 2);
        for (int i = 0; i < m; i++) {
            int u, v, w;
            std::cin >> u >> v >> w;
            u--, v--;
            G.add_edge(u, v, w);
            if (w >= 0) G.add_edge(v, u, w);
        }

        std::cout << (G.has_negative_cycle(0) ? "YES" : "NO") << '\n';
    }

    return 0;
}
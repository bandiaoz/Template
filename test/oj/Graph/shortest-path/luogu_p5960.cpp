#include <bits/stdc++.h>
#include "src/Graph/shortest-path/SPFA.h"

using ll = long long;

/*
[P5960 【模板】差分约束](https://www.luogu.com.cn/problem/P5960)
[status](https://www.luogu.com.cn/record/179811804)
*/
/**
 * 本题为差分约束模板题
 * 如果 x_v - x_u <= w, 则添加边 u -> v, 边权为 w
 * 这个形式和单源最短路 dist[v] <= dist[u] + w 是一样的
 * 在 SPFA 求解过程中，需要把所有的点的初始距离都归零，以及入队
 * 如果有负环，输出 NO，否则 x_i = dist[i] 即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::SPFA::Graph<int> G(n + 1, m);
    for (int i = 0; i < m; i++) {
        int v, u, w;
        std::cin >> v >> u >> w;
        u--, v--;
        G.add_edge(u, v, w);
    }
    for (int i = 0; i < n; i++) {
        G.add_edge(n, i, 0);
    }

    auto [sol, ok] = G.calc<false>(n);
    if (!ok) {
        std::cout << "NO\n";
    } else {
        for (int i = 0; i < n; i++) {
            std::cout << sol.query(i) << ' ';
        }
    }

    return 0;
}
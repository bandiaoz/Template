#include <bits/stdc++.h>
#include "src/Graph/shortest-path/Dijkstra_heap.h"

using ll = long long;

/*
[P4779 【模板】单源最短路径（标准版）](https://www.luogu.com.cn/problem/P4779)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, s;
    std::cin >> n >> m >> s;
    s--;
    OY::DijkstraHeap::Graph<int> G(n, m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        u--, v--;
        G.add_edge(u, v, w);
    }

    auto sol = G.calc(s);
    for (int i = 0; i < n; i++) {
        std::cout << sol.query(i) << " \n"[i == n - 1];
    }

    return 0;
}
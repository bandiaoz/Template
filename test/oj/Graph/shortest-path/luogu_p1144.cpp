#include <bits/stdc++.h>
#include "src/Graph/shortest-path/Dijkstra_heap.h"
#include "src/Math/Modular/StaticModInt32.h"

using ll = long long;

/*
[P1144 最短路计数](https://www.luogu.com.cn/problem/P1144)
[status](https://www.luogu.com.cn/record/185906559)
*/
/**
 * 本题为 Dijkstra 算法模板题
 * 需要计数
 */

using Z = OY::StaticModInt32<100003, false>;
using group = OY::DijkstraHeap::AddSemiGroup<int>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::DijkstraHeap::Graph<int> G(n, m * 2);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        G.add_edge(u, v, 1);
        G.add_edge(v, u, 1);
    }

    auto sol = G.calc<group, Z>(0);
    for (int i = 0; i < n; i++) {
        std::cout << sol.query_count(i) << '\n';
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/shortest-path/Dijkstra.h"

using ll = long long;

/*
[CF1076D Edge Deletion](https://codeforces.com/contest/1076/problem/D)
[status](https://codeforces.com/contest/1076/submission/311648139)
*/
/**
 * 给定无向简单带权连通图，要求删边至多保留 k 条边，最大化「好点」的个数。
 * 「好点」定义为：删边前后，1 号点到它的最短路长度不变。
 * 
 * 求出最小路径树，最多保留 n - 1 条边就够了，如果 k 小于 n-1，则优先保证最短路长度小的点是好点。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, k;
    std::cin >> n >> m >> k;
    OY::Dijkstra::Graph<int> graph(n, m * 2);
    std::map<std::pair<int, int>, int> edge_id;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        u--, v--;
        graph.add_edge(u, v, w);
        graph.add_edge(v, u, w);
        edge_id[{u, v}] = i;
        edge_id[{v, u}] = i;
    }

    using Group = OY::Dijkstra::AddGroup<int, ll, std::less_equal<ll>>;
    auto sol = graph.calc<Group, void, true>(0);

    std::vector<std::pair<int, ll>> ans;
    for (int i = 0; i < n; i++) {
        if (i == 0) continue;
        int prev = sol.m_dis_heap.m_distance[i].m_from;
        ans.emplace_back(edge_id[{prev, i}], sol.query(i));
    }

    std::sort(ans.begin(), ans.end(), [](const auto &a, const auto &b) {
        return a.second < b.second;
    });

    k = std::min(k, n - 1);
    std::cout << k << "\n";
    for (int i = 0; i < k; i++) {
        std::cout << ans[i].first + 1 << " \n"[i == k - 1];
    }

    return 0;
}
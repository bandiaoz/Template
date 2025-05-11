#include <bits/stdc++.h>
#include "src/Graph/shortest-path/Dijkstra.h"

using ll = long long;

/*
[CF545E Paths and Trees](https://codeforces.com/contest/545/problem/E)
[status](https://codeforces.com/contest/545/submission/311646204)
*/
/**
 * 给定一个无向带权图，无重边无自环，求以 s 为根的最小路径树，同时保证树的边权总和尽可能小。
 * 最小路径树：从 s 到任意点的树上距离等于原图中 s 到该点的最短路。
 * 
 * 从 start 点开始跑最短路
 * 注意比较器需要使用 std::less_equal<ll>，从而保证每个点的前驱边权尽可能小，保证最小路径树的边权总和尽可能小
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
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
    int start;
    std::cin >> start;
    start--;

    using Group = OY::Dijkstra::AddGroup<int, ll, std::less_equal<ll>>;
    auto sol = graph.calc<Group, void, true>(start);

    ll sum = 0;
    std::vector<int> ans;
    for (int i = 0; i < n; i++) {
        if (i == start) continue;
        int prev = sol.m_dis_heap.m_distance[i].m_from;
        sum += sol.query(i) - sol.query(prev);
        ans.push_back(edge_id[{prev, i}]);
    }

    std::cout << sum << "\n";
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] + 1 << " \n"[i == ans.size() - 1];
    }

    return 0;
}
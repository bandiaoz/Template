#include <bits/stdc++.h>
#include "src/Graph/simple/BFS.h"

using ll = long long;

/*
[CF1005F Berland and the Shortest Paths](https://codeforces.com/contest/1005/problem/F)
[status](https://codeforces.com/contest/1005/submission/311654641)
*/
/**
 * 给定无向连通图，求出至少 k 种不同的最短路径树，如果无法构造出至少 k 种，则输出所有可能的最短路径树。
 * 
 * 这里边权全部为 1，所以可以不用 dijkstra，用 bfs 即可
 * 对于 1~n-1 的每个点，满足 dis[u] = dis[prev] + 1 的前驱个数为 cnt[u] 个，则最短路径树有 cnt[1] * cnt[2] * ... * cnt[n-1] 种
 * 从 1 到 n-1，枚举所有前驱，进行 dfs 即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, k;
    std::cin >> n >> m >> k;
    OY::BFS::Graph graph(n, m * 2);
    std::map<std::pair<int, int>, int> edge_id;
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
        graph.add_edge(v, u);
        edge_id[{u, v}] = i;
        edge_id[{v, u}] = i;
    }

    auto sol = graph.calc<void, true>(0);
    ll cnt = 1;
    for (int u = 1; u < n; u++) {
        int num = 0;
        graph(u, [&](int prev) {
            if (sol.query(u) == sol.query(prev) + 1) {
                num++;
            }
        });
        cnt = std::min<ll>(k, cnt * num);
    }

    std::cout << cnt << "\n";

    std::string res(m, '0');
    auto dfs = [&](auto &&self, int u) -> void {
        if (cnt == 0) return;
        if (u == n) {
            std::cout << res << "\n";
            cnt--;
            return;
        }
        graph(u, [&](int prev) {
            if (sol.query(u) == sol.query(prev) + 1) {
                res[edge_id[{u, prev}]] = '1';
                self(self, u + 1);
                res[edge_id[{u, prev}]] = '0';
            }
        });
    };
    dfs(dfs, 1);

    return 0;
}
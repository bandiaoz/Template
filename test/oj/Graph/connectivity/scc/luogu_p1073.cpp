#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P1073 [NOIP 2009 提高组] 最优贸易](https://www.luogu.com.cn/problem/P1073)
[status](https://www.luogu.com.cn/record/203012200)
*/
/**
 * 给定一个有向图，每个点有一个权值，构造一条从 1 到 n 的路径，使得路径终点权值减去路径起点权值最大
 * 
 * 缩点后，按照逆拓扑序枚举起点，维护这个点出发可以到达的最大点权即可。
 * 注意一定是从 1 出发，终点是 n
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    OY::SCC::Graph graph(n);
    for (int i = 0; i < m; i++) {
        int u, v, z;
        std::cin >> u >> v >> z;
        u--, v--;
        graph.add_edge(u, v);
        if (z == 2) {
            graph.add_edge(v, u);
        }
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    std::vector<int> max_of_group(groups.size());
    std::vector<int> reach(groups.size()); // 表示是否能到达终点
    std::vector<int> ans(groups.size());
    for (int i = groups.size() - 1; i >= 0; i--) {
        int min = 1e9;
        for (int u : groups[i]) {
            min = std::min(min, a[u]);
            max_of_group[i] = std::max(max_of_group[i], a[u]);
            reach[i] |= (u == n - 1);
        }

        for (int u : groups[i]) {
            graph(u, [&](int v) {
                if (reach[scc.query(v)]) {
                    reach[i] = true;
                    max_of_group[i] = std::max(max_of_group[i], max_of_group[scc.query(v)]);
                    ans[i] = std::max(ans[i], ans[scc.query(v)]);
                }
            });
        }
        if (reach[i]) {
            ans[i] = std::max(ans[i], max_of_group[i] - min);
        }
    }

    std::cout << ans[scc.query(0)] << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P3387 【模板】缩点](https://www.luogu.com.cn/problem/P3387)
[status](https://www.luogu.com.cn/record/202976331)
*/
/**
 * 对于一个有向图，带点权，求一条路径，使得路径经过的点权值之和最大。
 * 允许经过重复的点和边，但是点权只计算一遍。
 * 
 * 先缩点，然后逆拓扑排序 dp 即可
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

    OY::SCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto sol = graph.calc();
    auto groups = sol.get_groups();

    std::vector<int> weight_of_group(groups.size());
    for (int i = groups.size() - 1; i >= 0; i--) {
        weight_of_group[i] = std::accumulate(groups[i].begin(), groups[i].end(), 0, [&](int sum, int x) {
            return sum + a[x];
        });

        // 遍历 scc 中每个点，max 表示从本 scc 出发所能获得的最大值
        int max = weight_of_group[i];
        for (int u : groups[i]) {
            graph(u, [&](int v) {
                if (sol.query(v) != i) {
                    max = std::max(max, weight_of_group[i] + weight_of_group[sol.query(v)]);
                }
            });
        }
        weight_of_group[i] = max;
    }

    std::cout << std::ranges::max(weight_of_group) << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[367. 学校网络](https://www.acwing.com/problem/content/369)
[status](https://www.acwing.com/problem/content/submission/code_detail/39299675/)
*/
/**
 * 给定一个有向图，问最少选定多少个点，使得从这些点出发可以到达所有点
 * 以及最少需要添加多少条边，使得新图的任意点都可以到达图上所有点，即新图是一个强连通图
 * 
 * 先缩点，DAG 上有 x 个点入度为 0，y 个点出度为 0
 * 第一问答案是 x，选定所有入度为 0 的点
 * 第二问答案是 max(x, y)
 * 
 * 注意特判原图就是强连通图的情况
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::SCC::Graph graph(n);
    for (int i = 0; i < n; i++) {
        int j;
        while (std::cin >> j && j) {
            j--;
            graph.add_edge(i, j);
        }
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    if (groups.size() == 1) {
        std::cout << 1 << '\n';
        std::cout << 0 << '\n';
        return 0;
    }

    std::vector<int> in_deg(groups.size()), out_deg(groups.size());
    for (int i = 0; i < groups.size(); i++) {
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                if (scc.query(u) != scc.query(v)) {
                    in_deg[scc.query(v)]++;
                    out_deg[scc.query(u)]++;
                }
            });
        }
    }

    int x = std::count(in_deg.begin(), in_deg.end(), 0);
    int y = std::count(out_deg.begin(), out_deg.end(), 0);

    std::cout << x << '\n';
    std::cout << std::max(x, y) << '\n';

    return 0;
}
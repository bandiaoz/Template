#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P7251 [JSOI2014] 强连通图](https://www.luogu.com.cn/problem/P7251)
[status](https://www.luogu.com.cn/record/203234242)
*/
/**
 * 给定一个有向图，问
 * 1. 最多可以选出多少个点，这些点在原图两两可达
 * 2. 最少加多少条边，使得原图强连通
 * 
 * 先缩点，第一问的答案就是最大强连通分量的大小
 * 缩点后的 DAG 有 x 个点入度为 0，y 个点出度为 0
 * 第二问的答案是 max(x, y)，注意特判原图就是强连通图的情况
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::SCC::Graph graph(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    std::vector<int> in_deg(groups.size()), out_deg(groups.size());
    size_t max_size = 0;
    for (int i = 0; i < groups.size(); i++) {
        max_size = std::max(max_size, groups[i].size());
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

    std::cout << max_size << '\n';
    std::cout << (groups.size() == 1 ? 0 : std::max(x, y)) << '\n';

    return 0;
}
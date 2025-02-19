#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P2341 [USACO03FALL / HAOI2006] 受欢迎的牛 G](https://www.luogu.com.cn/problem/P2341)
[status](https://www.luogu.com.cn/record/202990551)
*/
/**
 * 给定一个有向图，问有多少点被所有点可达
 * 
 * DAG 中，任何往前走，必定终止于一个出度为 0 的点
 * 缩点后，如果只有一个 group 的出度为 0，则该 group 中的所有点被所有点可达
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::SCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    std::vector<int> deg(groups.size());
    for (int i = 0; i < groups.size(); i++) {
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                if (scc.query(u) != scc.query(v)) {
                    deg[scc.query(u)]++;
                }
            });
        }
    }

    if (std::count(deg.begin(), deg.end(), 0) != 1) {
        std::cout << "0\n";
    } else {
        int id = std::find(deg.begin(), deg.end(), 0) - deg.begin();
        std::cout << groups[id].size() << "\n";
    }
    
    return 0;
}
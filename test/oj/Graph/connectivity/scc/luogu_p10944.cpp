#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P10944 Going from u to v or from v to u?](https://www.luogu.com.cn/problem/P10944)
[status](https://www.luogu.com.cn/record/203209711)
*/
/**
 * 给定一张有向图，判断是否可以任选两点 u 和 v 都单向可达（即 u 可达 v 或 v 可达 u）。
 * 
 * 先缩点得到一个 DAG，每个强连通分量内部一定相互可达
 * 充要条件是缩点后的 DAG 存在一条链经过所有点，也就是拓扑序唯一。
 * 实现的时候，可以在是拓扑的时候队列大小始终小于等于 1，或者缩点后得到的拓扑序相邻不可交换。
 */

void solve() {
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
    
    for (int i = 0; i < groups.size(); i++) {
        bool exist = false; // 一定存在一条边指向下一个强连通分量
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                exist |= scc.query(v) == i + 1;
            });
        }
        if (i + 1 < groups.size() && !exist) {
            std::cout << "No\n";
            return;
        }
    }
    std::cout << "Yes\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"
#include "src/Math/Modular/StaticModInt32.h"

using ll = long long;
using Z = OY::mint1000000007;

/*
[P2194 HXY烧情侣](https://www.luogu.com.cn/problem/P2194)
[status](https://www.luogu.com.cn/record/203237867)
*/
/**
 * 给定一个有向图，每个点都有一个权值，可以花费 a[i] 的代价，从 i 点出发，最终回到 i 点
 * 问最少花费多少代价，可以走遍所有的点，以及费用最少时的方案数量
 * 
 * 先缩点，从强连通分量中任意一点出发，可以遍历这个强连通分量，最终回到这个点
 * 所以以每个强连通分量中代价最小的点为起点即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    int m;
    std::cin >> m;
    OY::SCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }
    
    auto scc = graph.calc();
    auto groups = scc.get_groups();

    ll cost = 0;
    Z plan = 1;
    for (auto &group : groups) {
        int min = 1e9, cnt = 1;
        for (auto u : group) {
            if (a[u] < min) {
                min = a[u];
                cnt = 1;
            } else if (a[u] == min) {
                cnt++;
            }
        }
        cost += min;
        plan *= cnt;
    }

    std::cout << cost << ' ' << plan << '\n';

    return 0;
}
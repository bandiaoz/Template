#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P3436 [POI 2006] PRO-Professor Szu](https://www.luogu.com.cn/problem/P3436)
[status](https://www.luogu.com.cn/record/203253352)
*/
/**
 * 有 n 个住宅楼和 1 个主楼组成有向图，从哪些住宅楼前往主楼的上班路线数最多。
 * 
 * 建反图，转换为主楼前往住宅楼的方案数量，按照拓扑序进行 dp 转移
 * 如果存在一个强连通分量大小大于 1 或者存在自环，并且可达，那么方案数为无穷大
 * 注意在 DAG 上对每个连通分量进行 dp，而不是对原图上每个点进行 dp
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::SCC::Graph graph(n + 1, m);
    std::vector<int> self_loop(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        if (u == v) {
            self_loop[u] = 1;
        } else {
            graph.add_edge(v, u);
        }
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    const int MAX = 36500;
    std::vector<int> dp(groups.size());
    dp[scc.query(n)] = 1;
    for (int i = 0; i < groups.size(); i++) {
        if (dp[i] && (groups[i].size() > 1 || self_loop[groups[i][0]])) {
            dp[i] = MAX;
        }
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                if (int j = scc.query(v); j != i) {
                    dp[j] = std::min(MAX, dp[j] + dp[i]);
                }
            });
        }
    }

    std::vector<int> ans;
    int cur = 0;
    for (int i = 0; i < n; i++) {
        int val = dp[scc.query(i)];
        if (val > cur) {
            cur = val;
            ans = {i};
        } else if (val == cur && cur != 0) {
            ans.push_back(i);
        }
    }

    if (cur == MAX) {
        std::cout << "zawsze\n";
    } else {
        std::cout << cur << "\n";
    }
    std::cout << ans.size() << "\n";
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] + 1 << " \n"[i == ans.size() - 1];
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P3627 [APIO2009] 抢掠计划](https://www.luogu.com.cn/problem/P3627)
[status](https://www.luogu.com.cn/record/203352146)
*/
/**
 * 给定一张无向图，每个点有点权，从 s 点开始，到 t_i 点结束，问经过的点的点权和最大是多少
 * 
 * scc 缩点后在 DAG 上 dp 即可，注意维护每个 scc 是否可达，如果可达则加上 scc 内所有点权
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
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    int s, p;
    std::cin >> s >> p;
    s--;
    std::vector<int> t(p);
    for (auto &i : t) {
        std::cin >> i;
        i--;
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    std::vector<int> dp(groups.size(), -1);
    dp[scc.query(s)] = 0;
    for (int i = 0; i < groups.size(); i++) {
        if (dp[i] == -1) continue;
        dp[i] += std::accumulate(groups[i].begin(), groups[i].end(), 0, [&](int sum, int u) {
            return sum + a[u];
        });
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                if (scc.query(v) != i) {
                    dp[scc.query(v)] = std::max(dp[scc.query(v)], dp[i]);
                }
            });
        }
    }

    int ans = 0;
    for (auto i : t) {
        ans = std::max(ans, dp[scc.query(i)]);
    }

    std::cout << ans << '\n';

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P4306 [JSOI2010] 连通数](https://www.luogu.com.cn/problem/P4306)
[status](https://www.luogu.com.cn/record/203354954)
*/
/**
 * 给定一张有向图，定义连通数 f(i) 为从 i 出发能到达的点数
 * 求所有 f(i) 的和
 * 
 * scc 缩点后在 DAG 上 dp，每个 scc 内所有点的答案相同
 * 用 bitset 维护每个点对 scc 的可达性
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::SCC::Graph graph(n);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        for (int j = 0; j < n; j++) {
            if (s[j] == '1') {
                graph.add_edge(i, j);
            }
        }
    }

    auto scc = graph.calc();
    auto groups = scc.get_groups();

    std::vector<std::bitset<2000>> dp(groups.size());
    for (int i = 0; i < groups.size(); i++) {
        for (auto u : groups[i]) {
            dp[i].set(u);
        }
        for (auto u : groups[i]) {
            graph(u, [&](int v) {
                if (scc.query(v) != i) {
                    dp[scc.query(v)] |= dp[i];
                }
            });
        }
    }

    int ans = 0;
    for (int i = 0; i < groups.size(); i++) {
        ans += dp[i].count() * groups[i].size();
    }

    std::cout << ans << '\n';
    
    return 0;
}
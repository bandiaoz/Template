#include <bits/stdc++.h>
#include "src/Graph/misc/Psuedotree_ug.h"

using ll = long long;

/*
[P1453 城市环路](https://www.luogu.com.cn/problem/P1453)
[status](https://www.luogu.com.cn/record/210363376)
*/
/**
 * 给定一棵基环树，每个点有点权，求最大独立集，即选出的点两两之间没有边相连
 * 
 * 环外做树形 dp，dp[u][0] 表示不选 u 的环外收益，dp[u][1] 表示选 u 的环外收益
 * 环上滚动 dp，roll_dp[0][0] 表示不选环的第一个点，不选 u 的总收益，
 *            roll[0][1] 表示不选环的第一个点，选 u 的总收益，
 *            roll[1][0] 表示选环的第一个点，不选 u 的总收益，
 *            roll[1][1] 表示选环的第一个点，选 u 的总收益
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

    OY::PsuedoUG::Graph<bool> graph(n);
    for (int i = 0; i < n; i++) {
        int u, v;
        std::cin >> u >> v;
        graph.add_edge(u, v);
    }
    graph.prepare();

    std::vector dp(n, std::vector<int>(2));
    for (int u = 0; u < n; u++) {
        if (graph.get_vertex_info(u).step_to_cycle() == 0) {
            auto dfs = [&](auto &&self, int u) -> void {
                dp[u][1] = a[u];
                graph.do_for_out_edges(u, [&](int v) {
                    self(self, v);
                    dp[u][0] += std::max(dp[v][0], dp[v][1]);
                    dp[u][1] += dp[v][0];
                });
            };
            dfs(dfs, u);
        }
    }

    ll ans = 0;
    for (int id = 0; id < graph.cycle_count(); id++) {
        auto cycle = graph.get_cycle_info(id);
        int len = cycle.size();
        std::array<std::array<ll, 2>, 2> roll{};
        roll[0][0] = dp[cycle[0]][0];
        roll[1][1] = dp[cycle[0]][1];
        for (int idx = 1; idx < len; idx++) {
            int u = cycle[idx];
            std::array<std::array<ll, 2>, 2> new_roll;
            for (int head = 0; head < 2; head++) {
                new_roll[head][0] = std::max(roll[head][0], roll[head][1]) + dp[u][0];
                new_roll[head][1] = roll[head][0] + dp[u][1];
            }
            roll = new_roll;
        }
        ans += std::max({roll[0][0], roll[0][1], roll[1][0]});
    }

    double k;
    std::cin >> k;
    std::cout << std::fixed << std::setprecision(1) << ans * k << "\n";

    return 0;
}
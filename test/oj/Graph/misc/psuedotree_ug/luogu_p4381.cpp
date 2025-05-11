#include <bits/stdc++.h>
#include "src/Graph/misc/Psuedotree_ug.h"

using ll = long long;

/*
[P4381 [IOI2008] Island](https://www.luogu.com.cn/problem/P4381)
[status](https://www.luogu.com.cn/record/210354042)
*/
/**
 * 给定基环树森林，求每个基环树的直径（最长链）之和
 * 
 * 基环树直径可能经过环，也可能不经过环。
 * 对于不经过环的基环树，其直径就是树上最长链，树上 dp 即可。
 * 对于经过环的基环树，
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::PsuedoUG::Graph<int, ll, false> graph(n);
    for (int i = 0; i < n; i++) {
        int to, len;
        std::cin >> to >> len;
        to--;
        graph.add_edge(i, to, len);
    }

    graph.prepare();

    std::vector<ll> ans(graph.cycle_count());
    std::vector<ll> out_ray(n);
    // 先在环上的每个点，往环外搜
    for (int u = 0; u < n; u++) {
        if (graph.get_vertex_info(u).step_to_cycle() == 0) {
            int cycle_id = graph.get_vertex_info(u).cycle_id();
            auto dfs = [&](auto &&self, int u) -> ll {
                ll res = 0;
                graph.do_for_out_edges(u, [&](int v, int dis) {
                    ll ray = self(self, v);
                    ans[cycle_id] = std::max(ans[cycle_id], res + ray + dis);
                    res = std::max(res, ray + dis);
                });
                return res;
            };
            out_ray[u] = dfs(dfs, u);
        }
    }

    // 在环上 dp
    for (int id = 0; id < graph.cycle_count(); id++) {
        auto cycle = graph.get_cycle_info(id);
        // 统计从 l 正向连到 r 的最长链
        ll pre = 0;
        for (int r = 0; r < cycle.size(); r++) {
            int x = cycle[r];
            if (r) pre += cycle.position(r) - cycle.position(r - 1);
            ans[id] = std::max(ans[id], pre + out_ray[x]);
            pre = std::max(pre, out_ray[x]);
        }
        // 统计从 l 反向连到 r 的最长链
        pre = 0;
        for (int r = 0; r < cycle.size(); r++) {
            int x = cycle[r];
            if (r) ans[id] = std::max(ans[id], pre + out_ray[x] + (cycle.length() - cycle.position(r)));
            pre = std::max(pre, out_ray[x] + cycle.position(r));
        }
    }

    std::cout << std::accumulate(ans.begin(), ans.end(), 0LL) << "\n";

    return 0;
}
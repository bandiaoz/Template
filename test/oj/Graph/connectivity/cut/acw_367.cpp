#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"

/*
[365. 圆桌骑士](https://www.acwing.com/problem/content/367/)
[status](https://www.acwing.com/problem/content/submission/code_detail/39381609/)
*/
/**
 * 现在给定骑士总数 n，以及 m 对相互僧恨的关系。
 * 求至少要踢掉多少个骑士，使得剩下的骑士都能存在某个相邻点没有憎恨关系的奇环中。
 * 
 * 对于补图，删除不在奇环上的点即可。
 * 1. 如果两个点在不同的点双中，那么不可能在同一个奇环中。
 * 2. 若某个点双中存在奇环，那么这个点双中所有点至少被一个奇环包含。
 * 
 * 所以对补图建图，求点双，判断每个点双中是否存在奇环即可，如果不存在奇环，则这个点双所有点都删除，否则都保留。
 * 可二分图染色，则不存在奇环，需要删除。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    while (std::cin >> n >> m && n && m) {
        std::vector g(n, std::vector<int>(n, 1));
        for (int i = 0; i < m; i++) {
            int u, v;
            std::cin >> u >> v;
            u--, v--;
            g[u][v] = g[v][u] = 0;
        }

        OY::VBCC::Graph graph(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (g[i][j]) {
                    graph.add_edge(i, j);
                }
            }
        }

        auto vbcc = graph.calc<true, true>();
        
        std::vector<int> color(n);
        std::vector<int> vbcc_id(n, -1);
        std::vector<int> in_odd_cycle(n);
        int vbcc_cnt = 0;
        vbcc.do_for_each_vbcc([&](uint32_t *first, uint32_t *last) {
            for (auto it = first; it != last; it++) {
                color[*it] = -1;
                vbcc_id[*it] = vbcc_cnt;
            }
            
            auto dfs = [&](auto &&self, int u) -> bool {
                for (int v = 0; v < n; v++) {
                    if (u == v || !g[u][v] || vbcc_id[v] != vbcc_cnt) continue;
                    if (color[v] == -1) {
                        color[v] = color[u] ^ 1;
                        if (!self(self, v)) return false;
                    } else if (color[v] == color[u]) {
                        return false;
                    }
                }
                return true;
            };
            color[*first] = 0;
            if (!dfs(dfs, *first)) {
                for (auto it = first; it != last; it++) {
                    in_odd_cycle[*it] = 1;
                }
            }
            vbcc_cnt++;
        });

        std::cout << n - std::count(in_odd_cycle.begin(), in_odd_cycle.end(), 1) << '\n';
    }

    return 0;
}
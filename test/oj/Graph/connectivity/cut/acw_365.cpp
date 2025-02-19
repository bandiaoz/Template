#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"
#include "src/Tree/VectorTree.h"

using ll = long long;

/*
[363. B城](https://www.acwing.com/problem/content/365/)
[status](https://www.acwing.com/problem/content/submission/code_detail/39363446/)
*/
/**
 * 给定一张无向连通图，如果 f(u) 表示删除点 u 的所有邻边后，不联通的有序点对数量
 * 求 f(1) ... f(n) 的值
 * 
 * 建立圆方树，维护每个点双的大小，枚举割点和其相邻的联通块，计算贡献即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::VBCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto vbcc = graph.calc<true, true>();
    OY::VectorTree::Tree<bool> tree(n + vbcc.m_vbcc_cnt);
    int vbcc_cnt = 0;
    vbcc.do_for_each_vbcc([&](uint32_t *first, uint32_t *last) {
        for (auto it = first; it != last; it++) {
            tree.add_edge(n + vbcc_cnt, *it);
        }
        vbcc_cnt++;
    });

    std::vector<int> fa(n + vbcc.m_vbcc_cnt, -1);
    std::vector<int> sz(n + vbcc.m_vbcc_cnt);
    auto dfs = [&](auto &&self, int u) -> void {
        if (u < n) sz[u] = 1;
        tree.do_for_each_adj_vertex(u, [&](int v) {
            if (v == fa[u]) return;
            fa[v] = u;
            self(self, v);
            sz[u] += sz[v];
        });
    };
    dfs(dfs, 0);

    std::vector<ll> ans(n, 2 * (n - 1));
    vbcc.do_for_each_cut([&](int u) {
        tree.do_for_each_adj_vertex(u, [&](int v) {
            int size = (v == fa[u] ? n - sz[u] : sz[v]);
            ans[u] += 1LL * size * (n - 1 - size);
        });
    });

    for (int i = 0; i < n; i++) {
        std::cout << ans[i] << '\n';
    }

    return 0;
}
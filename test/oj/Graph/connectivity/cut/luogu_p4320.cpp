#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[P4320 道路相遇](https://www.luogu.com.cn/problem/P4320)
[status](https://www.luogu.com.cn/record/203536925)
*/
/**
 * 给定一个无向图，求从 u 到 v 的路径中，有多少必经点，包括 u 和 v 本身。
 * 
 * 建立圆方树，查询两点之间有多少个圆点即可。
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
            tree.add_edge(*it, vbcc_cnt + n);
        }
        vbcc_cnt++;
    });

    tree.set_root(0);
    tree.prepare();

    OY::HLD::Table<decltype(tree)> hld(&tree);
    auto dist = [&](int u, int v) {
        int lca = hld.lca(u, v);
        return hld.get_depth(u) + hld.get_depth(v) - 2 * hld.get_depth(lca);
    };

    int q;
    std::cin >> q;
    while (q--) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        std::cout << dist(u, v) / 2 + 1 << '\n';
    }
        
    return 0;
}
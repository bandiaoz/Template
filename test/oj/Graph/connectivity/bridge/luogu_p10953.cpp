#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_bridge.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[P10953 逃不掉的路](https://www.luogu.com.cn/problem/P10953)
[status](https://www.luogu.com.cn/record/203405871)
*/
/**
 * 给定一个无向连通图，q 次询问，每次询问任意两点桥的数量
 * 
 * 通过边双 ebcc 缩成一棵树，u 和 v 之间桥的数量等价于树上两点的边数（距离）
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::EBCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto ebcc = graph.calc<true, true>();
    int ebcc_cnt = 0;
    std::vector<int> ebcc_id(n);
    ebcc.do_for_each_ebcc([&](auto first, auto last) {
        for (auto it = first; it != last; it++) {
            ebcc_id[*it] = ebcc_cnt;
        }
        ebcc_cnt++;
    });

    OY::VectorTree::Tree<bool> tree(ebcc_cnt);
    ebcc.do_for_each_bridge([&](int index) {
        auto [u, v] = graph.m_raw_edges[index];
        tree.add_edge(ebcc_id[u], ebcc_id[v]);
    });
    tree.set_root(0);
    tree.prepare();

    OY::HLD::Table<decltype(tree)> hld(&tree);

    int q;
    std::cin >> q;
    while (q--) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;

        auto dist = [&](int u, int v) {
            int lca = hld.lca(u, v);
            return hld.get_depth(u) + hld.get_depth(v) - 2 * hld.get_depth(lca);
        };

        std::cout << dist(ebcc_id[u], ebcc_id[v]) << '\n';
    }

    return 0;
}
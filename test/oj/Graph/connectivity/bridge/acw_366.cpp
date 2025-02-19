#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_bridge.h"
#include "src/DataStruct/DSU/DSU.hpp"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[364. 网络](https://www.acwing.com/problem/content/description/366)
[status](https://www.acwing.com/problem/content/submission/code_detail/39353144/)
*/
/**
 * 给定一个无向连通图，每次添加一条边，并询问此时图中桥的数量。
 * 
 * 先用边双缩成一棵树，每次询问暴力将两点之间的边标记为不是桥，维护当前桥的数量。复杂度为 O(nq)
 * 可以使用并查集优化，例如标记从 u 到 anc 这条路径上的边，暴力跳转完后，并查集合并这些路径上的点，下次跳转时直接跳到 anc，每条边只标记一次
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    int test_case = 0;
    while (std::cin >> n >> m && n) {
        std::cout << "Case " << ++test_case << ":\n";
        OY::EBCC::Graph graph(n, m);
        for (int i = 0; i < m; i++) {
            int u, v;
            std::cin >> u >> v;
            u--, v--;
            graph.add_edge(u, v);
        }

        auto ebcc = graph.calc<true, true>();
        std::vector<int> ebcc_id(n);
        int ebcc_cnt = 0;
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

        OY::DSUTable<false> dsu(ebcc_cnt);
        int ans = ebcc_cnt - 1;
        auto work = [&](int u, int lca) {
            for (u = dsu.find(u); hld.m_info[u].m_dep > hld.m_info[lca].m_dep; ) {
                ans--;
                int fa = hld.find_parent(u);
                dsu.unite_to(u, dsu.find(fa));
                u = dsu.find(fa);
            }
        };

        int q;
        std::cin >> q;
        while (q--) {
            int u, v;
            std::cin >> u >> v;
            u--, v--;
            
            u = ebcc_id[u], v = ebcc_id[v];
            int lca = hld.lca(u, v);
            work(u, lca);
            work(v, lca);
            std::cout << ans << '\n';
        }
        std::cout << "\n";
    } 

    return 0;
}
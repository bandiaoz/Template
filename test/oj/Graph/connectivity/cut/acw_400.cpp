#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[398. 交通实时查询系统](https://www.acwing.com/problem/content/400/)
[status](https://www.acwing.com/problem/content/submission/code_detail/39362359/)
*/
/**
 * 给定一张无向图，多次询问两条边之间有多少个必经点
 * 
 * 建立圆方树，先找到两条边对应的方点，然后求树上路径长度，必经点个数就是方点之间圆点的个数，即树上距离 / 2
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    while (std::cin >> n >> m && n && m) {
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

        tree.set_root(0);
        tree.prepare();
        OY::HLD::Table<decltype(tree)> hld(&tree);

        vbcc_cnt = 0;
        std::vector<int> vbcc_e_id(m);
        auto edge_call = [&](uint32_t *first, uint32_t *last) {
            for (auto it = first; it != last; it++) {
                vbcc_e_id[*it] = vbcc_cnt;
            }
            vbcc_cnt++;
        };
        auto single_call = [&](uint32_t i) {};
        vbcc.find_edges(edge_call, single_call, graph);
        
        auto dist = [&](int u, int v) {
            int lca = hld.lca(u, v);
            return hld.m_info[u].m_dep + hld.m_info[v].m_dep - 2 * hld.m_info[lca].m_dep;
        };

        int q;
        std::cin >> q;
        while (q--) {
            int s, t;
            std::cin >> s >> t;
            s--, t--;
            s = vbcc_e_id[s] + n;
            t = vbcc_e_id[t] + n;
            std::cout << dist(s, t) / 2 << '\n';
        }
    }

    return 0;
}
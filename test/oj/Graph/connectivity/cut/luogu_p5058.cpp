#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[P5058 [ZJOI2004] 嗅探器](https://www.luogu.com.cn/problem/P5058)
[status](https://www.luogu.com.cn/record/203436505)
*/
/**
 * 给定一个无向图，求两点之间的一个必经点，如果有多解，输出最小的必经点
 * 
 * 建立圆方树，求圆方树上两点路径上的圆点，不包括端点
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::VBCC::Graph graph(n);
    for (int u, v; std::cin >> u >> v && u && v; ) {
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

    int s, t;
    std::cin >> s >> t;
    s--, t--;

    int ans = n;
    hld.do_for_path<true>(s, t, [&](int l, int r) {
        for (int i = l; i < r; i++) {
            int u = hld.m_seq[i];
            if (u != s && u != t && u < n) {
                ans = std::min(ans, u);
            }
        }
    });

    if (ans == n) {
        std::cout << "No solution\n";
    } else {
        std::cout << ans + 1 << '\n';
    }

    return 0;
}
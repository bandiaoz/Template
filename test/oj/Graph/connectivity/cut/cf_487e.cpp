#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HLDZkw.h"

/*
[CF487E Tourists](https://codeforces.com/contest/487/problem/E)
[status](https://codeforces.com/contest/487/submission/307517040)
*/
/**
 * 给定一个无向连通图，每个点有一个点权，有若干次操作：
 * 1. 修改某个点的点权
 * 2. 询问两点之间的所有路径上点权的最小值
 * 
 * 建立圆方树，令方点权值为相邻圆点权值的最小值，那么问题转换为求圆方树上两点路径上最小值；
 * 但圆点权值改变，需要改变相邻的所有方点的权值，复杂度会在菊花图下退化。
 * 
 * 利用圆方树是树的性质，令方点权值为自己的儿子圆点权值的最小值，这样圆点权值改变时，只需要改变父亲方点的权值。
 * 注意如果 LCA 是方点，需要额外查 LCA 的父亲圆点的权值
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, q;
    std::cin >> n >> m >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
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
            tree.add_edge(*it, n + vbcc_cnt);
        }
        vbcc_cnt++;
    });

    tree.set_root(0);
    tree.prepare();

    std::vector<int> val(n + vbcc.m_vbcc_cnt);
    std::vector<std::multiset<int>> st(vbcc.m_vbcc_cnt);
    tree.tree_dp_vertex(0, [&](int u, int f) {
        val[u] = (u < n) ? a[u] : INT_MAX;
    }, [&](int u, int v) {
        if (u >= n) {
            val[u] = std::min(val[u], val[v]);
            st[u - n].insert(val[v]);
        }
    }, {});

    auto zkw = OY::make_HLDZkw<int>(&tree, std::min, [&](int u) { return val[u]; });
    auto hld = &zkw.m_hld;

    while (q--) {
        char op;
        std::cin >> op;
        if (op == 'C') {
            int u, x;
            std::cin >> u >> x;
            u--;

            if (int fa = hld->find_parent(u); fa != -1) {
                st[fa - n].extract(val[u]);
                st[fa - n].insert(x);
                zkw.modify(fa, *st[fa - n].begin());
            }
            val[u] = x;
            zkw.modify(u, val[u]);
        } else {
            int u, v;
            std::cin >> u >> v;
            u--, v--;

            int ans = zkw.query_path<true>(u, v);
            if (int lca = hld->lca(u, v); lca >= n) {
                ans = std::min(ans, val[hld->find_parent(lca)]);
            }
            std::cout << ans << "\n";
        }
    }

    return 0;
}
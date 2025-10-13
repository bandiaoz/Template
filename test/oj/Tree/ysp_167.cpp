#include <bits/stdc++.h>
#include "src/DataStruct/Fenwick/MonoBIT.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/DfnController.h"

using ll = long long;

/*
[Vertex Add Subtree Sum](https://judge.yosupo.jp/problem/vertex_add_subtree_sum)(https://github.com/yosupo06/library-checker-problems/issues/167)
[status](https://judge.yosupo.jp/submission/319111)
*/
/*
给定一棵树，每个点有一个权值，支持两种操作：
1. 将一个点的权值加上一个值
2. 查询一个点的子树的权值和

使用 DfnController + MonoBIT 解决
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 1; i < n; i++) {
        int p;
        std::cin >> p;
        tree.add_edge(i, p);
    }

    tree.set_root(0);
    tree.prepare();
    OY::DFN::Table<decltype(tree)> dfn(&tree);
    OY::MonoSumBIT<ll> fen(n, [&](uint32_t i) {
        return a[dfn.m_seq[i]];
    });

    while (q--) {
        int op;
        std::cin >> op;
        if (op == 0) {
            int p, x;
            std::cin >> p >> x;
            dfn.do_for_vertex(p, [&](uint32_t pos) {
                fen.add(pos, x);
            });
        } else {
            int u;
            std::cin >> u;
            std::cout << dfn.do_for_subtree(u, [&](uint32_t l, uint32_t r) {
                return fen.query(l, r);
            }) << '\n';
        }
    }

    return 0;
}
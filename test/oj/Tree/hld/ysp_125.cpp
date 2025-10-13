#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#include "src/DataStruct/Fenwick/MonoBIT.h"

using ll = long long;

/*
[Vertex Add Path Sum](https://judge.yosupo.jp/problem/vertex_add_path_sum)(https://github.com/yosupo06/library-checker-problems/issues/125)
[status](https://judge.yosupo.jp/submission/319084)
*/
/*
给定一棵树，每个点有一个权值，支持两种操作：
1. 将一个点的权值加上一个值
2. 查询一条路径上的权值和

HLD + MonoBIT
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
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        tree.add_edge(u, v);
    }
    
    tree.set_root(0);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);
    OY::MonoSumBIT<ll> fen(n, [&](uint32_t i) {
        return a[hld.m_seq[i]];
    });

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 0) {
            int p, x;
            std::cin >> p >> x;
            hld.do_for_vertex(p, [&](int pos) {
                fen.add(pos, x);
            });
        } else {
            int u, v;
            std::cin >> u >> v;

            ll ans = 0;
            hld.do_for_path<true>(u, v, [&](int l, int r) {
                ans += fen.query(l, r);
            });
            std::cout << ans << "\n";
        }
    }

    return 0;
}
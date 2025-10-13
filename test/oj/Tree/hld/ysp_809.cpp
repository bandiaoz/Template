#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[Jump on Tree](https://judge.yosupo.jp/problem/jump_on_tree)(https://github.com/yosupo06/library-checker-problems/issues/809)
[status](https://judge.yosupo.jp/submission/319068)
*/
/*
给定一棵树，询问 $$s \to t$$ 路径上第 $$k$$ 个点。

使用 HLD 解决。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        tree.add_edge(u, v);
    }

    tree.set_root(0);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);

    auto dist = [&](int u, int v) {
        return hld.get_depth(u) + hld.get_depth(v) - 2 * hld.get_depth(hld.lca(u, v));
    };

    while (q--) {
        int s, t, k;
        std::cin >> s >> t >> k;

        int lca = hld.lca(s, t);
        if (k > dist(s, t)) {
            std::cout << "-1\n";
        } else if (k <= dist(s, lca)) {
            std::cout << hld.get_ancestor(s, k) << "\n";
        } else {
            std::cout << hld.get_ancestor(t, dist(s, t) - k) << "\n";
        }
    }

    return 0;
}
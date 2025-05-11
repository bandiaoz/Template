#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBaseTree.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

using ll = long long;

/*
[P3292 [SCOI2016] 幸运数字](https://www.luogu.com.cn/problem/P3292)
[status](https://www.luogu.com.cn/record/207193628)
*/
/**
 * 对于一棵树，有点权，每次询问树上路径的点权组成的集合中，子集的最大异或和。
 * 
 * 对于树上路径线性基查询，有特化实现。可以初始化复杂度为 O(w^2)，然后可以在 O(w) / O(w^2) 时间内获得路径点权组成的线性基。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<ll> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        tree.add_edge(u, v);
    }

    tree.set_root(0);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);
    OY::StaticHamelXorBaseTree64<60> hxb(&tree, [&](int u) { return a[u]; });

    while (m--) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        int lca = hld.lca(u, v);

        std::cout << hxb.to_base_type(u, v, lca).query_max_bitxor() << '\n';
    }

    return 0;
}
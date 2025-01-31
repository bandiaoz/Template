#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/DataStruct/RMQ/MaskRMQ.h"
#include "src/Tree/RMQLCA.h"
#include "src/Tree/DoubleLCA.h"

using ll = long long;

/*
[P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)
[status](https://www.luogu.com.cn/record/182931061)
*/
/**
 * 本题为 LCA 模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q, root;
    std::cin >> n >> q >> root;
    root--;
    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        tree.add_edge(u, v);
    }

    tree.set_root(root);
    tree.prepare();
    OY::RMQLCA::Table<decltype(tree), OY::MaskRMQMinValueTable<uint32_t>> LCA(&tree);
    // OY::DoubleLCA::Table<decltype(tree)> LCA(&tree);

    while (q--) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        std::cout << LCA.calc(u, v) + 1 << '\n';
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#include "src/DataStruct/Bit/PersistentBiTrie.h"

/*
[P4592 [TJOI2018]异或](https://www.luogu.com.cn/problem/P4592)
[status](https://www.luogu.com.cn/record/237643887)
*/
/*
给定一棵树，每个节点有一个权值，现在有 $$q$$ 次操作：
1. 查询节点 $$x$$ 的子树中的某个节点权值与 $$z$$ 异或结果的最大值；
2. 查询节点 $$x$$ 到节点 $$y$$ 的简单路径上的某个节点的权值与 $$z$$ 异或结果最大值。
$$2\leq n, q \leq10^5$$，$$1 \leq v_i, z \lt 2^{30}$$

维护两棵 PersistentBiTrie，一棵按照 dfn，一棵按照从根到叶子
*/

using Trie = OY::PerBiTrie::CountTree<uint32_t, uint32_t, 30>;
using Node = Trie::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<uint32_t> a(n);
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

    std::vector<Trie> trie_dfn(n + 1), trie_path(n);
    trie_dfn[0].insert_one(0);
    for (int i = 0; i < n; i++) {
        int u = hld.m_seq[i];
        trie_dfn[i + 1] = trie_dfn[i].copy();
        trie_dfn[i + 1].insert_one(a[u]);
    }

    trie_path[0].insert_one(a[0]);
    for (int u = 1; u < n; u++) {
        int f = hld.find_parent(u);
        trie_path[u] = trie_path[f].copy();
        trie_path[u].insert_one(a[u]);
    }

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            int u, z;
            std::cin >> u >> z;
            u--;
            int l = hld.m_info[u].m_dfn, r = hld.m_info[u].m_dfn + hld.m_info[u].m_size;
            std::cout << (trie_dfn[r] - trie_dfn[l]).max_bitxor(z) << '\n';
        } else {
            int u, v, z;
            std::cin >> u >> v >> z;
            u--, v--;

            int lca = hld.lca(u, v);
            std::cout << std::max<uint32_t>({
                z ^ a[lca],
                (trie_path[u] - trie_path[lca]).max_bitxor(z),
                (trie_path[v] - trie_path[lca]).max_bitxor(z)
            }) << "\n";
        }
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HLDZkw.h"

using ll = long long;

/*
[CF916E Jamie and Tree](https://codeforces.com/contest/916/problem/E)
[status](https://codeforces.com/contest/916/submission/339060086)
*/
/*
给定一棵树，有 $$q$$ 次操作：
1. 将树的根节点修改为 $$u$$
2. 将 $$\mathrm{lca}(u, v)$$ 的子树内所有点权加 $$w$$
3. 询问以 $$u$$ 为根的子树的权值和
$$1 \leq n, q \leq 10^5$$

换根树剖板子题
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
        u--, v--;
        tree.add_edge(u, v);
    }

    int root = 0;
    tree.set_root(root);
    tree.prepare();

    OY::HLDZkwLazySumTree<decltype(tree), ll> zkw(&tree, [&](uint32_t u) { return a[u]; });

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            std::cin >> root;
            root--;
        } else if (op == 2) {
            int u, v, w;
            std::cin >> u >> v >> w;
            u--, v--;

            int lca = zkw.m_hld.rootedLca(u, v, root);
            if (lca == root) {
                zkw.add_subtree(0, w);
            } else if (zkw.m_hld.isAncestor(lca, root)) {
                int child = zkw.m_hld.rootedChild(lca, root);
                zkw.add_subtree(0, w);
                zkw.add_subtree(child, -w);
            } else {
                zkw.add_subtree(lca, w);
            }
        } else {
            int u;
            std::cin >> u;
            u--;

            if (u == root) {
                std::cout << zkw.query_all() << "\n";
            } else if (zkw.m_hld.isAncestor(u, root)) {
                int child = zkw.m_hld.rootedChild(u, root);
                std::cout << zkw.query_all() - zkw.query_subtree(child) << "\n";
            } else {
                std::cout << zkw.query_subtree(u) << "\n";
            }
        }
    }

    return 0;
}
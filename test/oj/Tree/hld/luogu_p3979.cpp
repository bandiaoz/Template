#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#include "src/DataStruct/Segtree/AssignZkwTree.h"

/*
[P3979 遥远的国度](https://www.luogu.com.cn/problem/P3979)
[status](https://www.luogu.com.cn/record/180584605)
*/
/**
 * 给定一棵树，涉及三种操作：
 * 1. 换根
 * 2. 修改路径上的点权
 * 3. 查询路径上的点权最小值
 * 本题涉及换根、路径修改、子树查询
 * 树的形态发生变化，需要使用 LCT
 * 由于没有发生加边、断边，所以也可以使用重链剖分
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
        u--, v--;
        tree.add_edge(u, v);
    }
    std::vector<int> vals(n);
    for (auto &i : vals) {
        std::cin >> i;
    }
    int root;
    std::cin >> root;
    root--;

    tree.set_root(0);
    OY::HLD::Table<decltype(tree)> hld(&tree);
    OY::AssignMinZkw<int> zkw(n, [&](int u) {
        return vals[hld.m_seq[u]];
    }, INT32_MAX);

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            std::cin >> root;
            root--;
        } else if (op == 2) {
            int u, v, val;
            std::cin >> u >> v >> val;
            u--, v--;
            hld.do_for_path<true>(u, v, [&](int l, int r) {
                zkw.modify(l, r, val);
            });
        } else {
            int x;
            std::cin >> x;
            x--;

            if (x == root) {
                std::cout << zkw.query_all() << '\n';
            } else if (!hld.isAncestor(x, root)) {
                hld.do_for_subtree(x, [&](int l, int r) {
                    std::cout << zkw.query(l, r) << "\n";    
                });
            } else {
                hld.do_for_subtree(hld.find_son(x, root), [&](int l, int r) {
                    int res = INT32_MAX;
                    if (l > 0) res = std::min(res, zkw.query(0, l));
                    if (r < n) res = std::min(res, zkw.query(r, n));
                    std::cout << res << '\n';
                });
            }
        }   
    }

    return 0;
}
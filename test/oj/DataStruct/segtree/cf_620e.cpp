#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/AssignZkwTree.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[CF620E New Year Tree](https://codeforces.com/contest/620/problem/E)
[status](https://codeforces.com/contest/620/submission/308751296)
*/
/**
 * 给定一棵树，树上有点权，有 q 次操作：
 * 1. 将以 u 为根的子树节点权值修改为 c
 * 2. 询问以 u 为根的子树有多少种不同的权值
 * 权值大小 1 <= c <= 60
 * 
 * 将权值用位运算合并，使用 AssignZkwTree 做区间覆盖和区间求或即可。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
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

    tree.set_root(0);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);

    using Info = unsigned long long;
    OY::AssignBitOrZkw<Info> zkw(n, [&](int i) { return 1ULL << a[hld.m_seq[i]]; }, 0);

    while (m--) {
        int op, u, c;
        std::cin >> op >> u;
        u--;

        if (op == 1) {
            std::cin >> c;
            zkw.modify(hld.m_info[u].m_dfn, hld.m_info[u].m_dfn + hld.m_info[u].m_size, 1ULL << c);
        } else {
            std::cout << std::popcount(zkw.query(hld.m_info[u].m_dfn, hld.m_info[u].m_dfn + hld.m_info[u].m_size)) << '\n';
        }
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#include "src/DataStruct/RMQ/SparseTable.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[CF1062E Company](https://codeforces.com/contest/1062/problem/E)
[status](https://codeforces.com/contest/1062/submission/339034052)
*/
/*
给定一棵树，每次询问区间 $$[l, r]$$ 内任意删去一个点，使得剩余点的 LCA 的深度最大，输出删去的点编号和 LCA 最大深度

区间 LCA 可以用 RMQ 维护
LCA 的子树范围内 dfn 是连续的，所以只需要删去最大或最小的 dfn 即可
*/

using HLDTable = OY::HLD::Table<OY::VectorTree::Tree<bool>>;
struct LCAOp {
    static const HLDTable *H;
    uint32_t operator()(uint32_t x, uint32_t y) const {
        if (!~x) return y;
        if (!~y) return x;
        return H->lca(x, y);
    }
};
const HLDTable *LCAOp::H = nullptr;
using STLCATable = OY::ST::Table<OY::ST::BaseSemiGroup<uint32_t, LCAOp>>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    OY::VectorTree::Tree<bool> tree(n);
    for (int i = 1; i < n; i++) {
        int f;
        std::cin >> f;
        f--;
        tree.add_edge(f, i);
    }

    tree.set_root(0);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);

    OY::STMinTable<int> min_dfn(n, [&](int u) { return hld.m_info[u].m_dfn; });
    OY::STMaxTable<int> max_dfn(n, [&](int u) { return hld.m_info[u].m_dfn; });

    LCAOp::H = &hld;
    STLCATable lca(n, [&](uint32_t i) { return i; });

    while (q--) {
        int l, r;
        std::cin >> l >> r;
        l--;

        int x = hld.m_seq[min_dfn.query(l, r)];
        int y = hld.m_seq[max_dfn.query(l, r)];

        auto get = [&](int x) {
            int left = -1, right = -1;
            if (x != l) {
                left = lca.query(l, x);
            }            
            if (x + 1 != r) {
                right = lca.query(x + 1, r);
            }
            int LCA = LCAOp()(left, right);
            return hld.m_info[LCA].m_dep;
        };

        if (get(x) > get(y)) {
            std::cout << x + 1 << " " << get(x) << "\n";
        } else {
            std::cout << y + 1 << " " << get(y) << "\n";
        }
    }

    return 0;
}
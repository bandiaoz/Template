#include <bits/stdc++.h>
#include "src/DataStruct/DSU/DSU.hpp"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#include "src/DataStruct/RMQ/SparseTable.h"

/*
[CF1706E Qpwoeirut and Vertices](https://codeforces.com/contest/1706/problem/E)
[status](https://codeforces.com/contest/1706/submission/338512676)
*/
/*
给定 $$n$$ 个点 $$m$$ 条边的无向连通图，$$q$$ 次询问，每次给出 $$l, r$$，求最小的 $$k$$，连接前 $$k$$ 条边后，$$l \leq a \leq b \leq r$$ 的点 $$a$$ 和点 $$b$$ 连通

Kruskal 重构树，原图中两个点之间的所有简单路径上最大边权的最小值=最小生成树上两个点之间简单路径上的最大值=Kruskal重构树上两点之间的 LCA 权值

将边的编号定义为边权，u 和 v 连通，需要连边数量等于原图 u 和 v 之间的最大边权，等于连接 u 和 v 的 Kruskal 重构树上的 LCA 的编号
计算 $$ans_i$$ 表示连接 $$i$$ 和 $$i + 1$$ 的连边数量，查询区间最大值即可
*/

void solve() {
    int n, m, q;
    std::cin >> n >> m >> q;
    std::vector<std::pair<int, int>> edges(m);
    for (auto &[u, v] : edges) {
        std::cin >> u >> v;
        u--, v--;
    }

    int node = n;
    const int N = 2 * n - 1;
    OY::DSUTable<true> dsu(N);
    OY::VectorTree::Tree<bool> tree(N);
    std::vector<int> val(N);
    for (int i = 0; i < m; i++) {
        auto [u, v] = edges[i];
        if (!dsu.in_same_group(u, v)) {
            u = dsu.find(u);
            v = dsu.find(v);
            tree.add_edge(node, u);
            tree.add_edge(node, v);
            dsu.unite_to(u, node);
            dsu.unite_to(v, node);
            dsu.unite_by_ID(node, u);
            dsu.unite_by_ID(node, v);
            val[node] = i;
            node++;
        }
    }

    tree.set_root(N - 1);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);
    OY::STMaxTable<int> max(N - 1, [&](uint32_t i) { return val[hld.lca(i, i + 1)]; });

    for (int i = 0; i < q; i++) {
        int l, r;
        std::cin >> l >> r;
        l--;

        int ans = (r - l == 1) ? 0 : max.query(l, r - 1) + 1;
        std::cout << ans << " \n"[i == q - 1];
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/RaySeg.h"

using ll = long long;

/*
[P3304 [SDOI2013] 直径](https://www.luogu.com.cn/problem/P3304)
[status](https://www.luogu.com.cn/record/184865259)
*/
/**
 * 对于给定的树，有多少条边满足所有直径都经过这条边
 * 与直径相关的问题，可以使用最长射线线段助手解决
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::VectorTree::Tree<int> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        u--, v--;
        tree.add_edge(u, v, w);
    }

    tree.prepare();
    tree.set_root(0);

    OY::RAYSEG::Table<decltype(tree), ll, 200000> table(&tree);

    auto diameter = table.query_down_seg(0);
    int ans = 0;
    auto dfs = [&](auto &&self, int u, int f) -> void {
        tree.do_for_each_adj_edge(u, [&](int v, int w) {
            if (v != f) {
                // 屏蔽 v 子树的最长线段小于原直径
                // v 子树的最长线段长度小于原直径
                if (table.max_ray_seg_except_son(u, v, w).m_seg < diameter && 
                    table.query_down_seg(v) < diameter) {
                    ans++;
                }
                self(self, v, u);
            }
        });
    };
    dfs(dfs, 0, -1);

    std::cout << diameter << '\n';
    std::cout << ans << '\n';

    return 0;
}
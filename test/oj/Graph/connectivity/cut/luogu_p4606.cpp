#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"

/*
[P4606 [SDOI2018] 战略游戏](https://www.luogu.com.cn/problem/P4606)
[status](https://www.luogu.com.cn/record/203543234)
*/
/**
 * 给定一个无向连通图，每次询问给出一些关键点，问摧毁多少个非关键点，可以使得至少有两个关键点不联通
 * 
 * 建立圆方树，问题转换为有多少个非关键的圆点，在关键点的路径上。
 * 答案等价于包含所有关键点，并且「圆点数量最小」的联通块，减掉关键点数量。
 * 
 * 将圆点点权设置为 1，方点点权设置为 0，将点权放到与父节点连边的边权上。
 * 对关键点按照 dfn 排序，相邻关键点组成的所有路径，将最小联通块的每条边覆盖两次，可以得到联通块内边权总和，再加上联通块的根的权值即可。
 */

void solve() {
    int n, m;
    std::cin >> n >> m;
    OY::VBCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto vbcc = graph.calc<true, true>();

    OY::VectorTree::Tree<bool> tree(n + vbcc.m_vbcc_cnt);
    int vbcc_cnt = 0;
    vbcc.do_for_each_vbcc([&](uint32_t *first, uint32_t *last) {
        for (auto it = first; it != last; it++) {
            tree.add_edge(*it, vbcc_cnt + n);
        }
        vbcc_cnt++;
    });

    tree.set_root(0);
    tree.prepare();

    std::vector<int> val(n + vbcc.m_vbcc_cnt);
    auto dfs = [&](auto &&self, int u, int f) -> void {
        if (u < n) val[u]++; // 圆点权值为 1
        tree.do_for_each_adj_vertex(u, [&](int v) {
            if (v == f) return;
            val[v] += val[u];
            self(self, v, u);
        });
    };
    dfs(dfs, 0, -1);
    
    OY::HLD::Table<decltype(tree)> hld(&tree);
    auto dist = [&](int u, int v) {
        int lca = hld.lca(u, v);
        return val[u] + val[v] - 2 * val[lca];
    };

    int q;
    std::cin >> q;
    while (q--) {
        int k;
        std::cin >> k;
        std::vector<int> key(k);
        for (auto &x : key) {
            std::cin >> x;
            x--;
        }

        std::sort(key.begin(), key.end(), [&](int u, int v) {
            return hld.m_info[u].m_dfn < hld.m_info[v].m_dfn;
        });

        int ans = 0;
        for (int i = 0; i < k; i++) {
            int u = key[i], v = key[(i + 1) % k];
            ans += dist(u, v);
        }
        ans = ans / 2;
        ans += hld.lca(key.front(), key.back()) < n;

        std::cout << ans - k << "\n";
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
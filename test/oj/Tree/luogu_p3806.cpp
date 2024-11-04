#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/Centroid.h"

/*
[P3806 【模板】点分治1](https://www.luogu.com.cn/problem/P3806)
[status](https://www.luogu.com.cn/record/list?pid=P3806&user=130521)
*/
/**
 * 本题为点分树模板题
 * 由于本题在建好点分树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::VectorTree::Tree<int> tree(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        u--, v--;
        tree.add_edge(u, v, w);
    }
    std::vector<int> queries(m);
    for (auto &x : queries) {
        std::cin >> x;
    }

    tree.prepare();
    tree.set_root(0);

    std::vector<int> ans(m);
    std::vector<int> blocked(n), dis(n);
    auto pre_work = [&](int root) { blocked[root] = true; };
    auto after_work = [&](int root) {
        std::set<int> total{0};
        tree.do_for_each_adj_edge(root, [&](int v, int w) {
            std::set<int> curset;
            dis[v] = w;
            auto dfs = [&](auto &&self, int u, int f) -> void {
                if (blocked[u]) return;
                curset.insert(dis[u]);
                for (auto [v, w] : tree.m_adj[u]) {
                    if (v == f) continue;
                    dis[v] = dis[u] + w;
                    self(self, v, u);
                }
            };
            dfs(dfs, v, -1);
            auto dfs_2 = [&](auto &&self, int u, int f) -> void {
                if (blocked[u]) return;
                for (int i = 0; i < m; i++) {
                    if (total.count(queries[i] - dis[u])) {
                        ans[i] = true;
                    }
                }
                for (auto [v, w] : tree.m_adj[u]) {
                    if (v == f) continue;
                    self(self, v, u);
                }
            };
            dfs_2(dfs_2, v, -1);
            total.insert(curset.begin(), curset.end());
        });
        blocked[root] = false;
    };
    OY::Centroid::CentroidDecomposition<10000>::solve(tree, pre_work, {}, after_work);

    for (int i = 0; i < m; i++) {
        std::cout << (ans[i] ? "AYE" : "NAY") << '\n';
    }

    return 0;
}
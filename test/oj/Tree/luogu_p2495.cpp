#include <bits/stdc++.h>
#include "src/DataStruct/RMQ/MaskRMQ.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/RMQLCA.h"
#include "src/Tree/VirtualTree.h"

using ll = long long;

/*
[P2495 [SDOI2011] 消耗战](https://www.luogu.com.cn/problem/P2495)
[status](https://www.luogu.com.cn/record/182971528)
*/
/**
 * 本题为虚树模板题
 * 由于本题在建好虚树之后就是一个 dp，所以其实可以不用真正地建树，在找到边的时候直接转移即可
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

    tree.set_root(0);
    tree.prepare();
    OY::RMQLCA::Table<decltype(tree), OY::MaskRMQMinValueTable<uint32_t>> LCA(&tree);

    constexpr int N = 250000;
    constexpr int inf = 1e9;
    std::vector<ll> dp(n);
    std::vector<int> cost(n);
    std::vector<int> is_key(n);
    cost[0] = inf;
    auto dfs = [&](auto &&self, int u, int f) -> void {
        for (auto [v, w] : tree.m_adj[u]) {
            if (v == f) continue;
            cost[v] = std::min(cost[u], w);
            self(self, v, u);
        }
    };
    dfs(dfs, 0, -1);

    int q;
    std::cin >> q;
    while (q--) {
        int m;
        std::cin >> m;
        std::vector<int> keys(m);
        for (auto &key : keys) {
            std::cin >> key;
            key--;
            is_key[key] = true;
        }
        keys.push_back(0);

        std::vector<int> points{0};
        OY::VTREE::VirtualTree<N>::solve_rmqlca(keys.begin(), keys.end(), LCA, [&](int v, int u) {
            points.push_back(v);
            if (is_key[v]) {
                dp[u] += cost[v];
            } else {
                dp[u] += std::min<ll>(cost[v], dp[v]);
            }
        });

        std::cout << dp[0] << '\n';
        for (auto point : points) {
            dp[point] = 0;
            is_key[point] = false;
        }
    }

    return 0;
}
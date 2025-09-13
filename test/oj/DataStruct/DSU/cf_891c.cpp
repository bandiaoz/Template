#include <bits/stdc++.h>
#include "src/DataStruct/DSU/RollbackDSU.h"

/*
[CF891C Envy](https://codeforces.com/contest/891/problem/C) 
[status](https://codeforces.com/contest/891/submission/338166025)
*/
/*
给定一张 $$n$$ 个点 $$m$$ 条边的无向图，每条边有边权，共 $$Q$$ 次询问，每次给出 $$k_i$$ 条边，问这些边能否同时在一棵最小生成树上
$$1 \leq n, m, q, \sum k_i \leq 5 \times 10^5$$
​
考虑 kruskal 的过程，在加入小于 $$w$$ 的所有边之后，图的连通性是确定的
一个询问中所有边权等于 $$w$$ 的边，在加入所有小于 $$w$$ 的边之后，边权等于 $$w$$ 的边必须可以全部顺利加入，否则答案为 NO
可以离线下来使用并查集回滚来实现
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<std::array<int, 3>> edges(m);
    for (auto &[u, v, w] : edges) {
        std::cin >> u >> v >> w;
        u--, v--;
    }

    const int N = 500000;
    std::vector<std::vector<int>> mp(N + 1);
    for (int i = 0; i < m; i++) {
        auto [u, v, w] = edges[i];
        mp[w].push_back(i);
    }

    int q;
    std::cin >> q;
    std::vector<std::vector<int>> querys(q);
    for (auto &v : querys) {
        int k;
        std::cin >> k;
        v.resize(k);
        for (auto &i : v) {
            std::cin >> i;
            i--;
        }
    }
    std::vector qmp(N + 1, std::map<int, std::vector<int>>());
    for (int i = 0; i < q; i++) {
        for (auto j : querys[i]) {
            auto [u, v, w] = edges[j];
            qmp[w][i].push_back(j);
        }
    }

    OY::RollbackDSU::Table dsu(n);
    std::vector<int> ans(q, true);
    for (int i = 1; i <= N; i++) {
        for (auto [j, v] : qmp[i]) {
            int success = 0;
            for (auto id : v) {
                auto [u, v, w] = edges[id];
                if (!dsu.unite_by_size(u, v)) {
                    ans[j] = false;
                } else {
                    success++;
                }
            }
            while (success--) {
                dsu.cancel();
            }
        }
        for (auto j : mp[i]) {
            auto [u, v, w] = edges[j];
            dsu.unite_by_size(u, v);
        }
    }

    for (auto res : ans) {
        std::cout << (res ? "YES" : "NO") << '\n';
    }

    return 0;
}
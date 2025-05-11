#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"

using ll = long long;

/*
[P4151 [WC2011] 最大XOR和路径](https://www.luogu.com.cn/problem/P4151)
[status](https://www.luogu.com.cn/record/207191615)
*/
/**
 * 给定无向连通图，求一条从 1 到 n 的路径（可以不是简单路径），使得经过的边权异或和最大。
 * 
 * 任意求一条路径，异或和为 x，将所有环的异或和插入线性基中，求 x 与线性基的异或最大值即可。
 * 所有可能得路径异或和，一定等于一条路径的异或和，异或上任意的环。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int, ll>>> g(n);
    while (m--) {
        int u, v;
        ll w;
        std::cin >> u >> v >> w;
        u--, v--;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }

    OY::StaticHamelXorBase64<60> hxb;
    std::vector<ll> dis(n, -1);
    auto dfs = [&](auto &&self, int u, ll d) -> void {
        dis[u] = d;
        for (auto [v, w] : g[u]) {
            if (dis[v] != -1) {
                ll loop = dis[v] ^ d ^ w;
                hxb.insert(loop);
            } else {
                self(self, v, d ^ w);
            }
        }
    };
    dfs(dfs, 0, 0);

    std::cout << hxb.query_max_bitxor(dis[n - 1]) << "\n";

    return 0;
}
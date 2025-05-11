#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"
#include "src/Math/Modular/StaticModInt32.h"

using ll = long long;
using Z = OY::mint1000000007;

/*
[CF724G Xor-matic Number of the Graph](https://codeforces.com/contest/724/problem/G)
[status](https://codeforces.com/contest/724/submission/310996603)
*/
/**
 * 给定 n 个点 m 条边的无向图（不一定连通），每条边权值非负。
 * 如果三元组 (u, v, s) 满足，存在一条从 u 到 v 的路径（可以经过重复的点和边），其路径上的边权值异或和为 s，则称 (u, v, s) 为好三元组。
 * 求所有好三元组的 s 的和。
 * 
 * 对于一个连通子图，从任意一点出发，到达其他点的异或和等于 d[u] ^ B，B 表示环的异或和组成的线性基。
 * 所以 u 到 v 的所有路径的异或和等于 d[u] ^ d[v] ^ B。
 * 
 * 接下来对于每一对 u, v，尝试统计 d[u] ^ d[v] ^ B 中所有数的和
 * - 对于线性基 B，B 中元素个数为 s，则 B 可以表示出 2^s 个不同的数；
 * - 如果 B 中存在基向量第 w 位为 1，则 B 可以表示出 2^(s - 1) 个第 w 位为 1 的数；
 * - 如果 B 中不存在基向量第 w 位为 1，则 B 无法表示出第 w 位为 1 的数；
 * 
 * 考虑拆位，对于第 w 位
 * - 如果 B 可以表达出，意味着无论 d[u] 和 d[v] 的第 w 位是什么，都恰好有 2^(s - 1) 条路径的第 w 位为 1，对答案贡献为 2^w * 2^{s - 1}\binom(size, 2)
 * - 如果 B 无法表达出，那么 d[u] 和 d[v] 的第 w 位一定不同，且有 2^s 条路径的第 w 位为 1，对答案贡献为 2^w * 2^s * one * zero
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

    Z ans = 0;
    std::vector<ll> dis(n, -1);
    for (int i = 0; i < n; i++) {
        if (dis[i] != -1) continue;

        OY::StaticHamelXorBase64<60> hxb;
        std::vector<int> group;
        auto dfs = [&](auto &&self, int u, ll d) -> void {
            dis[u] = d;
            group.push_back(u);
            for (auto [v, w] : g[u]) {
                if (dis[v] != -1) {
                    ll loop = dis[v] ^ d ^ w;
                    hxb.insert(loop);
                } else {
                    self(self, v, d ^ w);
                }
            }
        };
        dfs(dfs, i, 0);

        for (int i = 0; i < 60; i++) {
            Z base = Z((1LL << i) % 1000000007);
            int size = group.size();
            int one = std::count_if(group.begin(), group.end(), [&](int j) { return dis[j] >> i & 1; });
            int zero = size - one;
            bool contains = false;
            hxb.enumerate_base([&](ll mask) { contains |= mask >> i & 1; });
            if (contains) {
                ans += base * size * (size - 1) / 2 * (1LL << (hxb.base_count() - 1));
            } else {
                ans += base * one * zero * (1LL << hxb.base_count());
            }
        }
    }

    std::cout << ans << "\n";

    return 0;
}
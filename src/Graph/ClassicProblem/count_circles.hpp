#include <bits/stdc++.h>

/**
 * @brief 简单无向图三元环计数
 * @note 复杂度 $O(m \sqrt{m})$，答案上界 $m \sqrt{m}$
 * @note 给边重定向，让度数小的边连向度数大的边，得到一个 DAG，所有节点的出度都小于 $\sqrt{m}$
 * @link https://www.luogu.com.cn/problem/P1989
 * @link https://judge.yosupo.jp/problem/enumerate_triangles
 */
static int circle3count(const int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<int> d(n);
    for (auto [u, v] : edges) {
        d[u]++, d[v]++;
    }
    std::vector<std::vector<int>> g(n);
    for (auto [u, v] : edges) {
        if (std::pair(d[u], u) > std::pair(d[v], v)) {
            std::swap(u, v);
        }
        g[u].emplace_back(v);
    }

    int ans = 0;
    std::vector<int> vis(n, -1);
    for (int u = 0; u < n; u++) {
        for (auto v : g[u]) {
            vis[v] = u;
        }
        for (auto v : g[u]) {
            for (auto w : g[v]) {
                if (vis[w] == u) {
                    ans++;
                }
            }
        }
    }
    return ans;
}

/**
 * @brief 简单无向图四元环计数
 * @note 枚举的复杂度实际和三元环计数类似, 为 $O(m \sqrt m)$
 * @link https://loj.ac/p/191
 */
static int64_t circle4count(const int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<int> deg(n);
    for (auto [u, v] : edges) {
        deg[u]++, deg[v]++;
    }

    std::vector<std::vector<int>> dir_g(n), g(n);
    for (auto [u, v] : edges) {
        if (std::pair(deg[u], u) < std::pair(deg[v], v)) {
            std::swap(u, v);
        }
        dir_g[u].push_back(v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    int64_t ans = 0;
    std::vector<int> cnt(n);
    // x -> y1 -> z and x -> y2 -> z
    for (int x = 0; x < n; x++) {
        for (int y : dir_g[x]) {
            for (int z : g[y]) {
                if (std::pair(deg[x], x) <= std::pair(deg[z], z)) continue;
                ans += cnt[z]++;
            }
        }
        for (int y : dir_g[x]) {
            for (int z : g[y]) {
                cnt[z] = 0;
            }
        }
    }

    return ans;
}

/**
 * @brief 无向图简单环计数
 * @note 允许有重边，不允许有自环
 * @link https://codeforces.com/contest/11/problem/D
 */
int64_t simple_circle_count(const int n, const std::vector<std::pair<int, int>> &edges) {
    int m = edges.size();
    std::vector<std::vector<int>> g(n);
    for (auto [u, v] : edges) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // dp[mask][u] 表示以 lowbit(mask) 为起点，以 u 为终点的 mask 状态下的路径数
    std::vector dp(1 << n, std::vector<int64_t>(n));
    int64_t ans = 0;
    for (int i = 0; i < n; i++) {
        dp[1 << i][i] = 1;
    }
    auto lowbit = [](int x) { return x & -x; };
    for (int mask = 1; mask < (1 << n); mask++) {
        for (int u = 0; u < n; u++) {
            if (!dp[mask][u]) continue;
            for (int v : g[u]) {
                if ((lowbit(mask) > (1 << v))) continue;
                if (mask >> v & 1) {
                    if (lowbit(mask) == (1 << v)) {
                        ans += dp[mask][u];
                    }
                } else {
                    dp[mask | (1 << v)][v] += dp[mask][u];
                }
            }
        }
    }
    
    // 减去每条边使用两次造成的二元环
    // 并且每个环会被计算两次（因为固定起点可以向两个方向走）
    return (ans - m) / 2;
}

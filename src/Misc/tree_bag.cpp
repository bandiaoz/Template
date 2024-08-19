#include <bits/stdc++.h>

/**
 * @brief 树上背包
 * @link https://www.luogu.com.cn/problem/P2014
 * @note 注意枚举的边界，i <= sz[u], j <= sz[v]，O(nm)
 */
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    m++;
    std::vector<int> a(n + 1);
    std::vector<std::vector<int>> g(n + 1);
    for (int i = 0; i < n; i++) {
        int f;
        std::cin >> f >> a[i];
        f--;
        if (f == -1) {
            f = n;
        }
        g[f].push_back(i);
    }

    const int inf = 1e9;
    std::vector dp(n + 1, std::vector<int>(m + 1, -inf));
    std::vector<int> sz(n + 1, 1);
    auto dfs = [&](auto &&self, int u) -> void {
        dp[u][0] = 0;
        dp[u][1] = a[u];
        for (auto v : g[u]) {
            self(self, v);
            sz[u] += sz[v];
            // 注意 i <= sz[u], j <= sz[v]
            for (int i = std::min(sz[u], m); i >= 0; i--) {
                for (int j = 0; j < i && j <= sz[v]; j++) {
                    dp[u][i] = std::max(dp[u][i], dp[u][i - j] + dp[v][j]);
                }
            }
        }
    };

    dfs(dfs, n);

    std::cout << dp[n][m] << "\n";

    return 0;
}
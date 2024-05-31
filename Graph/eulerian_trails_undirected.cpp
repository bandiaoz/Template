#include <bits/stdc++.h>

using ll = long long;

/**
 * @brief Eulerian trails for undirected graphs 无向图欧拉路径
 * @return 如果存在欧拉路径，返回 true 和路径 ans
 *         ans = {(i, dir)} 表示第 i 条边 [u, v] = edges[i]
 *         dir = 1 表示从 u 到 v，dir = 0 表示从 v 到 u
 * @link https://judge.yosupo.jp/problem/eulerian_trail_undirected
 * @note 无向图是欧拉图当且仅当：非零度顶点是连通的；顶点的度数都是偶数。此时从任意一个非零度顶点开始。
 * @note 无向图是半欧拉图当且仅当：非零度顶点是连通的；恰有 2 个奇度顶点。此时从其中一个奇度顶点开始。
 */
auto eulerian_trails_undirected(int n, const std::vector<std::pair<int, int>> &edges)
        -> std::pair<bool, std::vector<std::pair<int, bool>>> {
    std::vector<std::pair<int, bool>> res;
    if (edges.empty()) return {true, {}};

    std::vector<std::vector<std::pair<int, bool>>> g(n);
    for (int i = 0; i < edges.size(); i++) {
        auto [u, v] = edges[i];
        g[u].emplace_back(i, true);
        g[v].emplace_back(i, false);
    }

    int start = -1, odd = 0;
    for (int i = 0; i < n; i++) {
        if (g[i].size() % 2 == 0) continue;
        if (odd++ >= 2) return {false, {}};
        start = i;
    }
    for (int i = 0; i < n && start == -1; i++) {
        if (!g[i].empty()) start = i;
    }

    std::vector<bool> visited(edges.size());
    auto dfs = [&](auto &&self, int u) -> void {
        while (!g[u].empty()) {
            auto [i, dir] = g[u].back();
            g[u].pop_back();
            if (visited[i]) {
                continue;
            }
            visited[i] = true;
            self(self, dir ? edges[i].second : edges[i].first);
            res.emplace_back(i, dir);
        }
    };
    dfs(dfs, start);

    if (res.size() != edges.size()) {
        return {false, {}};
    }

    std::reverse(res.begin(), res.end());
    return {true, res};
}

/**
 * @brief 根据 res 获取欧拉路径的节点和边，如果 res 为空，nodes = {0}
 * @return {nodes, trails}, nodes 表示欧拉路径的节点，trails 表示欧拉路径的边
 */
auto get_trails(int n, const std::vector<std::pair<int, int>> &edges,
                const std::vector<std::pair<int, bool>> &res) {
    std::vector<int> nodes, trails;
    if (res.empty()) {
        nodes.push_back(0);
    } else {
        auto [i, dir] = res.front();
        nodes.push_back(dir ? edges[i].first : edges[i].second);
        for (auto [i, dir] : res) {
            nodes.push_back(dir ? edges[i].second : edges[i].first);
            trails.push_back(i);
        }
    }

    return std::pair(nodes, trails);
}
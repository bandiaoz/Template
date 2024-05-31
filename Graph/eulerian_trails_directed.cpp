#include <bits/stdc++.h>

using ll = long long;

/**
 * @brief Eulerian trails for directed graphs 有向图欧拉路径
 * @return 如果存在欧拉路径，返回 true 和路径 ans，ans 为边的编号
 * @link https://judge.yosupo.jp/problem/eulerian_trail_directed
 * @note 有向图是欧拉图当且仅当：非零度顶点是强连通的，每个顶点的入度和出度相等。
 * @note 有向图是半欧拉图当且仅当：非零度顶点是弱连通的；至多一个顶点的出度与入度之差为 1；
 *                            至多一个顶点的入度与出度之差为 1；其他顶点的入度和出度相等。
 */
auto eulerian_trails_directed(int n, const std::vector<std::pair<int, int>>& edges) 
        -> std::pair<bool, std::vector<int>> {
    std::vector<int> res;
    if (edges.empty()) return {true, {}};

    std::vector<std::vector<int>> g(n);
    std::vector<int> in(n);
    for (int i = 0; i < edges.size(); i++) {
        auto [u, v] = edges[i];
        g[u].push_back(i);
        in[v]++;
    }
    int start = -1;
    for (int i = 0; i < n; i++) {
        if (g[i].size() <= in[i]) continue;
        if (g[i].size() > in[i] + 1 || ~start) return {false, {}};
        start = i;
    }
    for (int i = 0; i < n && start == -1; i++) {
        if (!g[i].empty()) start = i;
    }

    auto dfs = [&](auto&& self, int u) -> void {
        while (!g[u].empty()) {
            int j = g[u].back();
            g[u].pop_back();
            self(self, edges[j].second);
            res.push_back(j);
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
auto get_trails(int n, const std::vector<std::pair<int, int>>& edges, 
                    const std::vector<int>& res) {
    std::vector<int> nodes;
    if (res.empty()) {
        nodes.push_back(0);
    } else {
        int i = res.front();
        nodes.push_back(edges[i].first);
        for (int i : res) {
            nodes.push_back(edges[i].second);
        }
    }
    return std::pair{nodes, res};
}
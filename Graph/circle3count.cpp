#include <bits/stdc++.h>

/**
 * @brief Counting ternary rings in undirected graphs
 * @param edges: the edges of the graph
 * @return the number of ternary rings
 * @note $O(m \sqrt{m})$, and the upper bound on the number of ternary rings is $m \sqrt{m}$
 * @note Giving Orienting to Edge, get a DAG, the out-degree of all nodes is less than \sqrt{m}
 */
static int circle3count(const std::vector<std::pair<int, int>>& edges) {
    int n = edges.size();
    std::vector<int> d(n);
    for (auto [u, v] : edges) {
        ++d[u], ++d[v];
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
    for (int u = 0; u < n; ++u) {
        for (auto v : g[u]) {
            vis[v] = u;
        }
        for (auto v : g[u]) {
            for (auto w : g[v]) {
                if (vis[w] == u) {
                    ++ans;
                }
            }
        }
    }
    return ans;
}
// test problem: https://www.luogu.com.cn/problem/P1989
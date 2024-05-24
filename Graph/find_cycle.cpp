#include <bits/stdc++.h>

/**
 * @brief 找任意一个环
 * @param g SELF_LOOP = false, MULTIPLE_EDGE = true, CONNECTED = false, DIRECTED = false or true
 * @return edge indices of cycle
 * @link directed: https://judge.yosupo.jp/problem/cycle_detection
 * @link undirected: https://judge.yosupo.jp/problem/cycle_detection_undirected
 */
auto find_cycle(const std::vector<std::vector<std::pair<int, int>>>& g) -> std::pair<std::vector<int>, std::vector<int>> {
    std::vector<int> state(g.size(), 0);
    std::vector parent(g.size(), std::pair(-1, -1));
    std::vector<int> cycle_edge;
    std::vector<int> cycle_vertex;
    auto dfs = [&](auto &&self, int u, int f) -> bool {
        state[u] = 1;
        for (auto [v, i] : g[u]) {
            if (i == f) continue;
            if (state[v] == 0) {
                parent[v] = std::pair(u, i);
                if (self(self, v, i)) {
                    return true;
                }
            } else if (state[v] == 1) {
                cycle_edge.push_back(i);
                cycle_vertex.push_back(v);
                for (int w = u; w != v; w = parent[w].first) {
                    cycle_edge.push_back(parent[w].second);
                    cycle_vertex.push_back(w);
                }
                return true;
            }
        }
        state[u] = 2;
        return false;
    };

    for (int i = 0; i < g.size(); i++) {
        if (state[i] == 0 && dfs(dfs, i, -1)) {
            return {cycle_edge, cycle_vertex};
        }
    }
    return {};
}
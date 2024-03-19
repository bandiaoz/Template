#include <bits/stdc++.h>

using ll = long long;

/**
 * @brief dijkstra algorithm
 * @tparam T type of distance
 * @param g 
 * @param w 
 * @param start 
 * @return distance from start to each vertex
 * @link dist: https://www.luogu.com.cn/problem/P4779
 * @link path: https://judge.yosupo.jp/problem/shortest_path
 * @link count: https://www.luogu.com.cn/problem/P1144
 */
template <typename T>
auto dijkstra(const std::vector<std::vector<std::pair<int, int>>> &g,
              const std::vector<T> &w, int start) -> std::pair<std::vector<int64_t>, std::vector<int>> {
    std::vector<int64_t> dis(g.size(), -1);
    std::vector<int> pre(g.size(), -1);
    constexpr int mod = 100003;
    std::vector<int> cnt(g.size()); 
    cnt[start] = 1;

    std::priority_queue<std::tuple<int64_t, int, int>> q;
    q.emplace(0, start, -1);
    while (!q.empty()) {
        auto [d, u, f] = q.top();
        q.pop();

        if (f != -1 && (dis[u] == -1 || dis[u] == -d)) {
            cnt[u] = (cnt[u] + cnt[f]) % mod;
        }
        if (dis[u] != -1) continue;
        dis[u] = -d;
        pre[u] = f;

        for (auto [v, j] : g[u]) {
            q.emplace(d - w[j], v, u);
        }
    }
    return {dis, pre};
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, s, t;
    std::cin >> n >> m >> s >> t;
    std::vector<std::vector<std::pair<int, int>>> g(n);
    std::vector<int> w(m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v >> w[i];
        g[u].emplace_back(v, i);
    }

    auto [dis, pre] = dijkstra(g, w, s);

    if (dis[t] == -1) {
        std::cout << "-1\n";
    } else {
        std::vector<int> path{t};
        for (int i = t; i != s; i = pre[i]) {
            path.push_back(pre[i]);
        }
        std::reverse(path.begin(), path.end());
        
        std::cout << dis[t] << ' ' << path.size() - 1 << '\n';
        for (int i = 0; i < path.size() - 1; i++) {
            std::cout << path[i] << ' ' << path[i + 1] << '\n';
        }
    }

    return 0;
}
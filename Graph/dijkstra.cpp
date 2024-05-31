#include <bits/stdc++.h>

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
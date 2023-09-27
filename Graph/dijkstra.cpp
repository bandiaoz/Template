#include <bits/stdc++.h>

using ll = long long;

/**
 * @brief dijkstra algorithm
 * @tparam T type of distance
 * @param g 
 * @param w 
 * @param start 
 * @return distance from start to each vertex
 * @link https://www.luogu.com.cn/problem/P4779
 * @link https://www.luogu.com.cn/problem/P1144
 */
template <typename T>
auto dijkstra(const std::vector<std::vector<std::pair<int, int>>> &g,
              const std::vector<T> &w, int start) {
    std::vector<T> dis(g.size(), -1);
    std::vector<int> pre(g.size(), -1);
    constexpr int mod = 100003;
    std::vector<int> cnt(g.size()); 
    cnt[start] = 1;

    std::priority_queue<std::tuple<T, int, int>> q;
    q.emplace(0, start, -1);
    while (!q.empty()) {
        auto [d, u, f] = q.top();
        q.pop();

        if (dis[u] != -1) {
            if (dis[u] == -d) {
                cnt[u] = (cnt[u] + cnt[f]) % mod;
            }
            continue;
        }
        dis[u] = -d;
        pre[u] = f;
        cnt[u] = (cnt[u] + cnt[f]) % mod;

        for (auto [v, j] : g[u]) {
            q.emplace(d - w[j], v, u);
        }
    }
    return dis;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, s;
    std::cin >> n >> m >> s;
    s--;
    std::vector<std::vector<std::pair<int, int>>> g(n);
    std::vector<int> w(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v >> w[i];
        u--, v--;
        g[u].emplace_back(v, i);
    }

    auto dis = dijkstra<int>(g, w, s);
    for (int i = 0; i < n; ++i) {
        std::cout << dis[i] << " \n"[i == n - 1];
    }

    return 0;
}
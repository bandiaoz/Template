#include <bits/stdc++.h>

/**
 * @brief 单源最短路 spfa algorithm
 * @return std::pair{无负环, 最短路}
 * @link 找负环: https://www.luogu.com.cn/problem/P3385
 * @link 最长路: https://www.luogu.com.cn/problem/P1807
 */
template <typename T>
auto spfa(const std::vector<std::vector<std::pair<int, int>>> &g,
          const std::vector<T> &w, int start) {   
    int n = g.size();
    constexpr T inf = std::numeric_limits<T>::max();
    std::vector dis(n, inf);
    std::vector<bool> vis(n);
    dis[start] = 0;
    vis[start] = true;
    std::queue<int> q;
    q.push(start);

    std::vector<int> cnt(n);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        vis[u] = false;
        for (auto [v, j] : g[u]) {
            if (dis[v] > dis[u] + w[j]) {
                dis[v] = dis[u] + w[j];
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) {
                    return std::pair(false, std::vector<T>{});
                }
                if (vis[v] == false) {
                    q.push(v);
                    vis[v] = true;
                }
            }
        }
    }
    return std::pair(true, dis);
}
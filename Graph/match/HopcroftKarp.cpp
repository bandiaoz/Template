#include <bits/stdc++.h>

/**
 * @brief 二分图匹配 Hopcroft-Karp algorithm
 * @note O(sqrt(V) * E)
 * @link https://uoj.ac/problem/78
 * @param 左部有 $n$ 个点，编号为 $0$ 到 $n-1$，右部有 $m$ 个点，编号为 $0$ 到 $m-1$，$edges$ 为边集
 * @return 返回 [ans, left, right] 最多 ans 对匹配
 *         left[i] 为左部第 i 个点匹配到的右部点编号，right[j] 为右部第 j 个点匹配到的左部点编号
*/
auto HopcroftKarp(int n, int m, const std::vector<std::pair<int, int>>& edges) {
    std::vector<int> g(edges.size()), left(n, -1), right(m, -1);
    int ans = 0;
    std::vector<int> deg(n + 1);
    for (auto& [x, y] : edges) deg[x]++;
    for (int i = 1; i <= n; i++) deg[i] += deg[i - 1];
    for (auto& [x, y] : edges) g[--deg[x]] = y;

    std::vector<int> a, p, q(n);
    for (;;) {
        a.assign(n, -1), p.assign(n, -1);
        int t = 0;
        for (int i = 0; i < n; i++)
            if (left[i] == -1) q[t++] = a[i] = p[i] = i;

        bool match = false;
        for (int i = 0; i < t; i++) {
            int x = q[i];
            if (~left[a[x]]) continue;
            for (int j = deg[x]; j < deg[x + 1]; j++) {
                int y = g[j];
                if (right[y] == -1) {
                    while (~y) right[y] = x, std::swap(left[x], y), x = p[x];
                    match = true, ans++;
                    break;
                }

                if (p[right[y]] == -1) q[t++] = y = right[y], p[y] = x, a[y] = a[x];
            }
        }

        if (!match) break;
    }
    return std::make_tuple(ans, left, right);
}
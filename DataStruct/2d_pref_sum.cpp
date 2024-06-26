#include <bits/stdc++.h>

/**
 * @brief 二维前缀和
 */
template <typename T>
struct pref_sum_2d {
    int n, m;
    std::vector<std::vector<T>> sum;
    template <typename U>
    pref_sum_2d(const std::vector<std::vector<U>>& a)
        : n(a.size()), m(a[0].size()), sum(n + 1, std::vector<T>(m + 1)) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                sum[i + 1][j + 1] = a[i][j] + sum[i][j + 1] + sum[i + 1][j] - sum[i][j];
            }
        }
    }
    // get sum of [x1, x2) * [y1, y2)
    T query(int x1, int y1, int x2, int y2) {
        return sum[x2][y2] - sum[x2][y1] - sum[x1][y2] + sum[x1][y1];
    }
};
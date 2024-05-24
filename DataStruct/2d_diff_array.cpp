#include <bits/stdc++.h>

/**
 * @brief 二维差分
 * @brief add(x1, y1, x2, y2, c) 表示将矩形内的所有数加上 c，不包括右边界和上边界
 * @link https://www.luogu.com.cn/problem/P3397
*/
template <typename T> 
struct diff_2d {
    int n, m;
    std::vector<std::vector<T>> dif;
    diff_2d(int n_, int m_) : n(n_), m(m_), dif(n + 2, std::vector<T>(m + 2)) {}
    void add(int x1, int y1, int x2, int y2, T c) {
        x1++, y1++, x2++, y2++;
        dif[x1][y1] += c;
        dif[x2][y1] -= c;
        dif[x1][y2] -= c;
        dif[x2][y2] += c;
    }

    std::vector<std::vector<T>> build() {
        std::vector res(n, std::vector<T>(m));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                dif[i + 1][j + 1] += dif[i][j + 1] + dif[i + 1][j] - dif[i][j];
                res[i][j] = dif[i + 1][j + 1];
            }
        }
        return res;
    }
};
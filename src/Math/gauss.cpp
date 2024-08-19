#include <bits/stdc++.h>

/**
 * @brief 高斯消元 Gaussian Elimination
 * @param a: \forall i, \sum_{j=0}^{m-1} a[i][j] * x[j] = a[i][m]
 * @return 0 -> no solution
 *         1 -> unique solution, 
 *         2 -> infinite solutions, 
 *         ans -> solution
 * @link https://www.luogu.com.cn/problem/P3389
*/
static std::pair<int, std::vector<double>> gauss(std::vector<std::vector<double>> a) {
    constexpr double EPS = 1e-9;
    int n = a.size();
    int m = a[0].size() - 1;

    std::vector<int> where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; col++) {
        int sel = row;
        for (int i = row; i < n; i++) {
            if (std::abs(a[i][col]) > std::abs(a[sel][col])) {
                sel = i;
            }
        }
        if (std::abs(a[sel][col]) < EPS) continue;
        std::swap(a[sel], a[row]);
        where[col] = row;

        for (int i = 0; i < n; i++) {
            if (i != row) {
                double c = a[i][col] / a[row][col];
                for (int j = col; j <= m; j++) {
                    a[i][j] -= a[row][j] * c;
                }
            }
        }
        row++;
    }

    std::vector<double> ans(m);
    for (int i = 0; i < m; i++) {
        if (where[i] != -1) {
            ans[i] = a[where[i]][m] / a[where[i]][i];
        }
    }
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < m; j++) {
            sum += ans[j] * a[i][j];
        }
        if (abs(sum - a[i][m]) > EPS) return {0, {}};
    }
    for (int i = 0; i < m; i++) {
        if (where[i] == -1) return {2, ans};
    }
    return {1, ans};
}
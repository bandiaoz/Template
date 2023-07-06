#include <bits/stdc++.h>

/**
 * @brief Gaussian Elimination
 * @param a: \forall i, \sum_{j=0}^{m-1} a[i][j] * x[j] = a[i][m]
 * @return 0 -> no solution, 1 -> unique solution, 2 -> infinite solutions, ans -> solution
*/
static std::pair<int, std::vector<double>> gauss(std::vector<std::vector<double>> a) {
    constexpr double EPS = 1e-9;
    int n = a.size();
    int m = a[0].size() - 1;

    std::vector<int> where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col) {
        int sel = row;
        for (int i = row; i < n; ++i) {
            if (abs(a[i][col]) > abs(a[sel][col])) {
                sel = i;
            }
        }
        if (abs(a[sel][col]) < EPS) continue;
        for (int i = col; i <= m; ++i) {
            std::swap(a[sel][i], a[row][i]);
        }
        where[col] = row;

        for (int i = 0; i < n; ++i) {
            if (i != row) {
                double c = a[i][col] / a[row][col];
                for (int j = col; j <= m; ++j) {
                    a[i][j] -= a[row][j] * c;
                }
            }
        }
        ++row;
    }

    std::vector<double> ans(m);
    for (int i = 0; i < m; ++i) {
        if (where[i] != -1) {
            ans[i] = a[where[i]][m] / a[where[i]][i];
        }
    }
    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < m; ++j) {
            sum += ans[j] * a[i][j];
        }
        if (abs(sum - a[i][m]) > EPS) return {0, {}};
    }
    for (int i = 0; i < m; ++i) {
        if (where[i] == -1) return {2, ans};
    }
    return {1, ans};
}

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector a(n, vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= n; ++j) {
            cin >> a[i][j];
        }
    }

    auto [sol, ans] = gauss(a);
    if (sol == 1) {
        cout << fixed << setprecision(2);
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << "\n";
        }
    } else {
        cout << "No Solution\n";
    }

    return 0;
}
// test case: https://www.luogu.com.cn/problem/P3389
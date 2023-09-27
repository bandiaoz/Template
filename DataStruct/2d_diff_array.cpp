// test problem: https://www.luogu.com.cn/problem/P3397
#include <bits/stdc++.h>

using ll = long long;

template <typename T> struct diff_2d {
    int n, m;
    std::vector<std::vector<T>> dif;
    diff_2d(int n_, int m_)
        : n(n_), m(m_), dif(n + 2, std::vector<T>(m + 2)) {}
    void add(int x1, int y1, int x2, int y2, T c) {
        x1++, x2++, y1++, y2++;
        dif[x1][y1] += c;
        dif[x2 + 1][y1] -= c;
        dif[x1][y2 + 1] -= c;
        dif[x2 + 1][y2 + 1] += c;
    }

    std::vector<std::vector<T>> build() {
        std::vector res(n, std::vector<T>(m));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                dif[i][j] += dif[i - 1][j] + dif[i][j - 1] - dif[i - 1][j - 1];
                res[i - 1][j - 1] = dif[i][j];
            }
        }
        return res;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;
    diff_2d<int> d(n, n);
    while (k--) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        x1--, y1--, x2--, y2--;
        d.add(x1, y1, x2, y2, 1);
    }

    auto a = d.build();
    for (auto &i : a) {
        for (auto &j : i) {
            std::cout << j << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
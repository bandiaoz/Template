#include <bits/stdc++.h>

/*
[P3265 [JLOI2015] 装备购买](https://www.luogu.com.cn/problem/P3265)
[status](https://www.luogu.com.cn/record/207208547)
*/
/**
 * 有 n 个装备，每个装备有 m 个属性，用向量 z_i = (a_1, a_2, ..., a_m) 表示
 * 希望买最多的装备前提下花最少的钱，并且购买的装备不能被其他已经购买的装备「线性组合」得到
 * 
 * 按照价格从小到大排序，依次插入线性基，如果成功插入就购买
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector z(n, std::pair<std::vector<long double>, int>());
    for (auto &row : z) {
        row.first.resize(m);
        for (auto &i : row.first) {
            std::cin >> i;
        }
    }
    for (auto &row : z) {
        std::cin >> row.second;
    }

    std::sort(z.begin(), z.end(), [&](const auto &a, const auto &b) {
        return a.second < b.second;
    });

    std::vector b(m, std::vector<long double>(m));
    constexpr long double eps = 1e-8;
    auto insert = [&](std::vector<long double> vec) -> bool {
        for (int i = m - 1; i >= 0; i--) {
            if (std::abs(vec[i]) < eps) continue;
            if (std::abs(b[i][i]) < eps) {
                b[i] = vec;
                return true;
            }
            long double t = vec[i] / b[i][i];
            for (int j = 0; j < m; j++) {
                vec[j] -= b[i][j] * t;
            }
        }
        return false;
    };

    int cnt = 0, cost = 0;
    for (int i = 0; i < n; i++) {
        if (insert(z[i].first)) {
            cnt++;
            cost += z[i].second;
        }
    }

    std::cout << cnt << " " << cost << "\n";
    

    return 0;
}
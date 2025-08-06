#include <bits/stdc++.h>
#include "src/Geometry/closest_pair.h"

using ll = long long;

/*
[Closest Pair of Points](https://judge.yosupo.jp/problem/closest_pair)
[status](https://judge.yosupo.jp/submission/298999)
*/
/**
 * 给定 n 个点，求平面最近点对（欧几里得距离），输出点对的编号
 */

void solve() {
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int>> a(n);
    for (auto &[x, y] : a) {
        std::cin >> x >> y;
    }

    auto solve = [&](const std::vector<std::pair<int, int>> &a) -> std::pair<int, int> {
        auto [d, p0, p1] = closest_pair<int, ll>(a);
        // p0 和 p1 是点集中的点，不是下标，通过 std::find 找到下标
        if (p0 != p1) {
            auto get_idx = [&](const std::pair<int, int> &p) {
                return std::find(a.begin(), a.end(), p) - a.begin();
            };
            return {get_idx(p0), get_idx(p1)};
        } else {
            auto it0 = std::find(a.begin(), a.end(), p0);
            auto it1 = std::find(std::next(it0), a.end(), p0);
            return {it0 - a.begin(), it1 - a.begin()};
        }
    };

    auto [i, j] = solve(a);
    std::cout << i << " " << j << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    
    while (t--) {
        solve();
    }

    return 0;
}
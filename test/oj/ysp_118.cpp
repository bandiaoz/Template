#include <bits/stdc++.h>
#include "src/DataStruct/ClassicProblem/OfflinePointAddRectSumCounter2D.h"

/*
[Rectangle Sum](https://judge.yosupo.jp/problem/rectangle_sum)
[status](https://judge.yosupo.jp/submission/229785)
*/
/**
 * 本题为一次性加入很多点，再一次性进行二维矩形值的查询
 * 可以离线进行
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    OY::OFFLINEPARSC2D::Solver<int, int> count(n, q);
    for (int i = 0; i < n; i++) {
        int x, y, w;
        std::cin >> x >> y >> w;
        count.add_point(x, y, w);
    }

    for (int i = 0; i < q; i++) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        count.add_query(x1, x2, y1, y2);
    }

    for (auto x : count.solve<int64_t>()) {
        std::cout << x << '\n';
    }

    return 0;
}
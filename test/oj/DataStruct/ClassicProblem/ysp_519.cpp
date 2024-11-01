#include <bits/stdc++.h>
#include "src/DataStruct/ClassicProblem/OfflinePointAddRectSumMaintainer2D.h"

using ll = long long;

/*
[Point Add Rectangle Sum](https://judge.yosupo.jp/problem/point_add_rectangle_sum)(https://github.com/yosupo06/library-checker-problems/issues/519)
[status](https://judge.yosupo.jp/submission/246249)
*/
/**
 * 本题为加入点和二维矩形值的查询混杂
 * 可以把修改和查询统统离线
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q; 
    OY::OFFLINEPARSM2D::Solver<int, int, ll> sol(n, q);
    for (int i = 0; i < n; i++) {
        int x, y, w;
        std::cin >> x >> y >> w;
        sol.add_point(x, y, w);
    }

    for (int i = 0; i < q; i++) {
        int op;
        std::cin >> op;
        if (op == 0) {
            int x, y, w;
            std::cin >> x >> y >> w;
            sol.add_point(x, y, w);
        } else {
            int l, d, r, u;
            std::cin >> l >> d >> r >> u;
            sol.add_query(l, r, d, u);
        }
    }

    for (auto x : sol.solve()) {
        std::cout << x << '\n';
    }

    return 0;
}
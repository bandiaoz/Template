#include <bits/stdc++.h>
#include "src/Graph/shortest-path/Johnson.h"

using ll = long long;

/*
[P5905 【模板】全源最短路（Johnson）](https://www.luogu.com.cn/problem/P5905)
[status](https://www.luogu.com.cn/record/185890808)
*/
/**
 * 本题为 Johnson 算法模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::Johnson::Graph<int> G(n, m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        u--, v--;
        G.add_edge(u, v, w);
    }

    const int inf = 1e9;
    auto [sol, ok] = G.calc<false>(inf);
    if (!ok) {
        std::cout << "-1\n";
    } else {
        for (int i = 0; i < n; i++) {
            ll ans = 0;
            for (int j = 0; j < n; j++) {
                ans += 1LL * (j + 1) * sol.query(i, j);
            }
            std::cout << ans << '\n';
        }
    }

    return 0;
}
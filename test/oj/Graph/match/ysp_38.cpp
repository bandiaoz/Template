#include <bits/stdc++.h>
#include "src/Graph/match/KuhnMunkres.h"

using ll = long long;

/*
[Assignment Problem](https://judge.yosupo.jp/problem/assignment)(https://github.com/yosupo06/library-checker-problems/issues/38)
[status](https://judge.yosupo.jp/submission/238569)
*/
/**
 * 二分图带权完美匹配模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    const ll inf = 1e18;
    OY::KM::Graph<ll> G(n, -inf);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ll x;
            std::cin >> x;
            G.add_edge(i, j, -x);
        }
    }

    std::cout << -G.calc() << '\n';
    for (int i = 0; i < n; i++) {
        std::cout << G.find_right(i) << " \n"[i == n - 1];
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Graph/match/KuhnMunkres.h"

using ll = long long;

/*
[P6577 【模板】二分图最大权完美匹配](https://www.luogu.com.cn/problem/P6577)
[status](https://www.luogu.com.cn/record/178956180)
*/
/**
 * 本题为二分图完美匹配模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    
    const ll inf = 1e18;
    OY::KM::Graph<ll> G(n, -inf);
    for (int i = 0; i < m; i++) {
        int u, v;
        ll cost;
        std::cin >> u >> v >> cost;
        u--, v--;
        G.add_edge(u, v, cost);
    }

    std::cout << G.calc() << '\n';
    for (int i = 0; i < n; i++) {
        std::cout << G.find_left(i) + 1 << " \n"[i == n - 1];
    }

    return 0;
}
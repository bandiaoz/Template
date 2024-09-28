#include <bits/stdc++.h>
#include "src/Graph/flow/Dinic.h"

using ll = long long;

/*
[P3376 【模板】网络最大流](https://www.luogu.com.cn/problem/P3376)
[status](https://www.luogu.com.cn/record/178918994)
*/
/**
 * 本题为最大流模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, source, sink;
    std::cin >> n >> m >> source >> sink;
    source--, sink--;

    OY::DINIC::Graph<int> flow(n, m);
    for (int i = 0; i < m; i++) {
        int u, v, cap;
        std::cin >> u >> v >> cap;
        u--, v--;
        flow.add_edge(u, v, cap);
    }

    std::cout << flow.calc<ll>(source, sink) << '\n';

    return 0;
}
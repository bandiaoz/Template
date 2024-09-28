#include <bits/stdc++.h>
#include "src/Graph/flow/Dinic_mcmf.h"

/*
[P3381 【模板】最小费用最大流](https://www.luogu.com.cn/problem/P3381)
[status](https://www.luogu.com.cn/record/178915909)
*/
/**
 * 本题为最小费用最大流模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, s, t;
    std::cin >> n >> m >> s >> t;
    s--, t--;
    OY::DINICMCMF::Graph<int, int, false> flow(n, m);
    for (int i = 0; i < m; i++) {
        int u, v, cap, cost;
        std::cin >> u >> v >> cap >> cost;
        u--, v--;
        flow.add_edge(u, v, cap, cost);
    }

    auto [max_flow, min_cost] = flow.calc(s, t);
    std::cout << max_flow << ' ' << min_cost << '\n';

    return 0;
}
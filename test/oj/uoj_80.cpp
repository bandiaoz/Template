#include <bits/stdc++.h>
#include "src/Graph/match/KuhnMunkres.h"

/*
[#80. 二分图最大权匹配](https://uoj.ac/problem/80)
[status](https://uoj.ac/submission/711212)
*/
/**
 * 本题为二分图最大权匹配模板题，需要输出方案
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t nl, nr, m;
    std::cin >> nl >> nr >> m;
    // 边权初始值设为零，这样就不会取负边
    OY::KM::Graph<int64_t> G(std::max(nl, nr), 0);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cost;
        std::cin >> a >> b >> cost;
        G.add_edge(a - 1, b - 1, cost);
    }

    std::cout << G.calc() << "\n";

    // 可以观察最后的边权
    // 如果是正的，说明是配对成功
    // 如果是零，说明取的是默认边，没有配对
    for (uint32_t l = 0; l < nl; l++) {
        uint32_t r = G.find_right(l);
        if (G.query(l, r)) {
            std::cout << r + 1 << ' ';
        } else {
            std::cout << 0 << ' ';
        }
    }
}
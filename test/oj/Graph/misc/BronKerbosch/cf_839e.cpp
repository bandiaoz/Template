#include <bits/stdc++.h>
#include "src/Graph/misc/BronKerbosch.h"

/*
[CF839E Mother of Dragons](https://codeforces.com/contest/839/problem/E)
[status](https://codeforces.com/contest/839/submission/335585000)
*/
/*
给定一张 $$n(1 \leq n \leq 40)$$ 个点的图，需要给每个点分配正权值，权值之和为 $$s(1 \leq s \leq 10^9)$$ 

最大化 $$\sum_{(u, v) \in E} a_ua_v$$


考虑不相邻的两个点 $$u, v$$，$$s_u$$ 表示和 $$u$$ 相邻的点权之和，那么这两个点的贡献为 $$a_us_u + a_vs_v$$
不妨设 $$s_u \leq s_v$$，那么将 $$a_u$$ 的点权全部分给 $$a_v$$ 一定不劣，因此一定是给一个最大团分配点权

设最大团大小为 $$k$$，那么答案为 $$\frac{s^2(k - 1)}{2k}$$
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, s;
    std::cin >> n >> s;
    OY::BK::Graph<uint64_t, 40> graph(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x;
            std::cin >> x;
            if (x) graph.add_edge(i, j);
        }
    }

    int sz = graph.max_clique();

    std::cout << std::fixed << std::setprecision(10);
    std::cout << 1. * s * s * (sz - 1) / sz / 2 << "\n";

    return 0;
}
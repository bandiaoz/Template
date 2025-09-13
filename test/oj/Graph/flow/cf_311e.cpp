#include <bits/stdc++.h>
#include "src/Graph/flow/Dinic.h"

/*
[CF311E Biologist](https://codeforces.com/contest/311/problem/E)
[status](https://codeforces.com/contest/311/submission/338029427)
*/
/*
有 $$n(1 \leq n \leq 10^4)$$ 条狗，改变狗的性别成本为 $$v_i$$，有 $$m(1 \leq m \leq 2000)$$ 个赌局，第 $$i$$ 个赌局要求 $$k_i$$ 条狗的性别为指定性别，可以赢 $$w_i$$ 元，失败了会损失 0 或 $$g$$。
求最大收益。

闭合子图：子图中满足每个点的所有后继都在子图中，即 $$\forall u \in V^*, (u, v) \in E \Rightarrow v \in V^*$$

最大权值闭合图：权值之和最大的闭合子图。
做法：
1. 若节点 $$u$$ 权值为正，则从源点 $$s$$ 向 $$u$$ 连一条有向边，边权即为该点点权；
2. 若节点 $$u$$ 权值为负，则从 $$u$$ 向汇点 $$t$$ 连一条有向边，边权即为该点点权的相反数；
3. 原图上所有边权改为 $$\infty$$；
4. 将所有正点权之和减去最大流，即为答案。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, g;
    std::cin >> n >> m >> g;
    std::vector<int> gender(n), v(n);
    for (auto &i : gender) {
        std::cin >> i;
    }
    for (auto &i : v) {
        std::cin >> i;
    }

    OY::DINIC::Graph<int> flow(n + m + 2);
    const int source = n + m, sink = n + m + 1;
    for (int i = 0; i < n; i++) {
        if (gender[i] == 0) {
            flow.add_edge(source, i, v[i]);
        } else {
            flow.add_edge(i, sink, v[i]);
        }
    }

    const int inf = 1e9;

    int ans = 0;
    for (int i = 0; i < m; i++) {
        int gen, w, k;
        std::cin >> gen >> w >> k;
        ans += w;
        for (int j = 0; j < k; j++) {
            int x;
            std::cin >> x;
            x--;
            if (!gen) {
                flow.add_edge(n + i, x, inf);
            } else {
                flow.add_edge(x, n + i, inf);
            }
        }

        int f;
        std::cin >> f;
        if (f) {
            w += g;
        }
        if (!gen) {
            flow.add_edge(source, n + i, w);
        } else {
            flow.add_edge(n + i, sink, w);
        }
    }

    std::cout << ans - flow.calc(source, sink) << '\n';

    return 0;
}
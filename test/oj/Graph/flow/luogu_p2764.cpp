#include <bits/stdc++.h>
#include "src/Graph/flow/Dinic.h"

/*
[P2764 最小路径覆盖问题](https://www.luogu.com.cn/problem/P2764)
[status](https://www.luogu.com.cn/record/180327039)
*/
/**
 * 本题为 DAG 最小不相交路径覆盖问题，即找到若干路径，使得每个点恰好在一条路径上，覆盖所有的点，并输出方案
 * 本题可以转化为最大流问题，即将每个点拆成两个点，组成一个二分图，如果存在一条边 (u, v)，则连接 $(left_u, right_v)$
 * 原图的点数 - 最大匹配数 = 最小路径覆盖数
 * 还原路径覆盖方案：对找到的流回调，如果流量为 1，则说明这条边在覆盖路径中，路径不会相交，使用 prev 和 next 数组还原路径
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;    
    OY::DINIC::Graph<int> flow(2 * n + 2, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        flow.add_edge(u, n + v, 1);
    }

    int source = 2 * n, sink = 2 * n + 1;
    for (int i = 0; i < n; i++) {
        flow.add_edge(source, i, 1);
        flow.add_edge(n + i, sink, 1);
    }

    int match = flow.calc(source, sink);
    std::vector<int> prev(n, -1), next(n, -1);
    flow.do_for_flows([&](int i, int f) {
        auto [u, v, cap] = flow.m_raw_edges[i];
        if (f == 1 && u < 2 * n && v < 2 * n) {
            next[u] = v - n;
            prev[v - n] = u;
        }
    });

    for (int i = 0; i < n; i++) {
        if (prev[i] == -1) {
            std::vector<int> path;
            for (int j = i; j != -1; j = next[j]) {
                path.push_back(j);
            }
            for (int j = 0; j < path.size(); j++) {
                std::cout << path[j] + 1 << " \n"[j == path.size() - 1];
            }
        }
    }

    std::cout << n - match << "\n";

    return 0;
}
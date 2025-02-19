#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"

/*
[P3225 [HNOI2012] 矿场搭建](https://www.luogu.com.cn/problem/P3225)
[status](https://www.luogu.com.cn/record/203461403)
*/
/**
 * 给定一个无向连通图，最少需要建立多少个救援点，以及建立最少救援点的情况下，方案数是多少
 * 需要满足删除任何一个点后，其他点都可以到达救援点
 * 
 * 对于每一个点双：
 * 如果有 2 个及以上割点，则无需建立救援点，删除任何一个点都可以走到其他点双中
 * 如果有 1 个割点，则需要建立一个救援点，并且不能建立在割点上
 * 如果没有任何割点，则需要建立两个救援点，删除一个点后至少还有一个救援点
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int m;
    int test_case = 0;
    while (std::cin >> m && m) {
        std::vector<std::pair<int, int>> edges(m);
        int n = 0;
        for (auto &[u, v] : edges) {
            std::cin >> u >> v;
            n = std::max({n, u, v});
            u--, v--;
        }
        OY::VBCC::Graph graph(n, m);
        for (auto &[u, v] : edges) {
            graph.add_edge(u, v);
        }

        auto vbcc = graph.calc<true, true>();
        uint64_t build = 0, method = 1;
        vbcc.do_for_each_vbcc([&](uint32_t *first, uint32_t *last) {
            int cut = std::count_if(first, last, [&](uint32_t u) { return vbcc.m_is_cut[u]; });
            int size = last - first;
            if (cut == 0) {
                build += 2; 
                method *= 1LL * size * (size - 1) / 2;
            } else if (cut == 1) {
                build += 1;
                method *= size - 1;
            }
        });

        std::cout << "Case " << ++test_case << ": " << build << " " << method << "\n";
    }

    return 0;
}
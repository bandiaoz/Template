#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_bridge.h"

/*
[395. 冗余路径](https://www.acwing.com/problem/content/397/)
[status](https://www.acwing.com/problem/content/submission/code_detail/39353484/)
*/
/**
 * 给定一个无向连通图，最少需要加多少条边，使得该图变成一个边双连通图
 * 
 * 先 ebcc 缩点成一棵树，度数为 1 的点为叶子节点，答案等于 ceil(leaf / 2)
 * leaf 个叶子，按 dfn 排序，i 向 i + leaf / 2 连边（每条割边两端子树的叶子是连续 2 段，肯定有 1 段 <= leaf / 2）
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::EBCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto ebcc = graph.calc<true, true>();
    std::vector<int> ebcc_id(n);
    int ebcc_cnt = 0;
    ebcc.do_for_each_ebcc([&](auto first, auto last) {
        for (auto it = first; it != last; it++) {
            ebcc_id[*it] = ebcc_cnt;
        }
        ebcc_cnt++;
    });

    std::vector<int> deg(ebcc_cnt);
    ebcc.do_for_each_bridge([&](int index) {
        auto [u, v] = graph.m_raw_edges[index];
        deg[ebcc_id[u]]++;
        deg[ebcc_id[v]]++;
    });

    int leaf = std::count_if(deg.begin(), deg.end(), [](int x) { return x == 1; });
    std::cout << (leaf + 1) / 2 << '\n';
    
    return 0;
}
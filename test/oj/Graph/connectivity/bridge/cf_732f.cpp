#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_bridge.h"
#include "src/Tree/VectorTree.h"

/*
[CF732F Tourist Reform](https://codeforces.com/contest/732/problem/F)
[status](https://codeforces.com/contest/732/submission/340850344)
*/
/*
给定一张无向图，要求给边定向，令 $$f(s)$$ 表示从 $$s$$ 出发能到达的点数，要求使得 $$f(s)$$ 的最小值最大，输出方案。

在一个边双内的点可以互相到达，并且至少有一个边双不能通过桥到达其他边双，选取点最多的边双即可。
以这个边双为根，桥的方向指向父亲节点；对于边双内，按照 dfs 定向
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::EBCC::Graph graph(n, m);
    std::vector<std::pair<int, int>> edges(m);
    for (auto &[u, v] : edges) {
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto solver = graph.calc<true, true>();
    std::vector<int> ebcc_id(n);
    int ebcc_cnt = 0;
    int max = 0, max_id = -1;
    solver.do_for_each_ebcc([&](auto first, auto last) {
        if (max < last - first) {
            max = last - first;
            max_id = ebcc_cnt;
        }
        for (auto it = first; it != last; it++) {
            ebcc_id[*it] = ebcc_cnt;
        }
        ebcc_cnt++;
    });

    OY::VectorTree::Tree<bool> tree(ebcc_cnt);
    std::map<std::pair<int, int>, std::pair<int, int>> raw_bridge;
    solver.do_for_each_bridge([&](int index) {
        auto [u, v] = graph.m_raw_edges[index];
        tree.add_edge(ebcc_id[u], ebcc_id[v]);
        raw_bridge[{ebcc_id[u], ebcc_id[v]}] = {u, v};
        raw_bridge[{ebcc_id[v], ebcc_id[u]}] = {v, u};
    });
    
    std::map<std::pair<int, int>, std::pair<int, int>> ans;
    tree.tree_dp_vertex(max_id, [&](int u, int f) {
        if (f != -1) {
            auto [raw_u, raw_f] = raw_bridge[{u, f}];
            ans[{raw_u, raw_f}] = {raw_u, raw_f};
            ans[{raw_f, raw_u}] = {raw_u, raw_f};
        }
    }, {}, {});

    std::vector<int> vis(m);
    solver.do_for_each_ebcc([&](auto first, auto last) {
        int id = ebcc_id[*first];
        auto dfs = [&](auto &&self, int u, int f) -> void {
            graph(u, [&](int index, int v) {
                if (ebcc_id[v] != id) return;
                if (v == f) return;
                if (vis[index]) return;
                vis[index] = true;
                ans[{u, v}] = ans[{v, u}] = {u, v};
                self(self, v, u);
            });
        };
        dfs(dfs, *first, -1);
    });
    
    std::cout << max << "\n";
    for (auto edge : edges) {
        auto [u, v] = ans[edge];
        std::cout << u + 1 << " " << v + 1 << "\n";
    }

    return 0;
} 
#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_cut.h"
#include "src/Tree/VectorTree.h"

using ll = long long;

/*
[P4630 [APIO2018] 铁人两项](https://www.luogu.com.cn/problem/P4630)
[status](https://www.luogu.com.cn/record/204519993)
*/
/**
 * 给定一张无向图，问有多少个三元组 (s, c, f) 满足 s, c, f 互不相同，并且 s -> c -> f 是一条简单路径
 * 
 * 首先建出圆方树
 * 枚举 s 和 f，c 的数量就是圆方树上 s -> f 路径上，所有点双的并集的点数减 2。
 * 可以将方点的权值设为点双的大小，圆点的权值设为 -1。
 * 这样赋权后则有两圆点间圆方树上路径点权和，恰好等于原图中简单路径并集点数大小减 2。
 * 
 * 问题转换为求圆方树任意两圆点路径权值之和，通过树形 dp 计算贡献。
 * 注意图可能不连通。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::VBCC::Graph graph(n, m);
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        graph.add_edge(u, v);
    }

    auto vbcc = graph.calc<true, true>();
    OY::VectorTree::Tree<bool> tree(n + vbcc.m_vbcc_cnt);
    int vbcc_cnt = 0;
    std::vector<int> val(n + vbcc.m_vbcc_cnt, -1);
    vbcc.do_for_each_vbcc([&](uint32_t *first, uint32_t *last) {
        for (auto it = first; it != last; it++) {
            tree.add_edge(n + vbcc_cnt, *it);
        }
        val[n + vbcc_cnt] = last - first;
        vbcc_cnt++;
    });

    std::vector<int> vis(n + vbcc.m_vbcc_cnt), root(n + vbcc.m_vbcc_cnt);
    std::vector<int> fa(n + vbcc.m_vbcc_cnt, -1), size(n + vbcc.m_vbcc_cnt);
    for (int i = 0; i < n + vbcc.m_vbcc_cnt; i++) {
        if (!vis[i]) {
            tree.tree_dp_vertex(i, [&](int u, int f) { 
                root[u] = i;
                vis[u] = 1;
                size[u] = (u < n); 
                fa[u] = f;
            }, [&](int u, int v) {
                size[u] += size[v];
            }, {});
        }
    }

    ll ans = 0;
    for (int u = 0; u < n + vbcc.m_vbcc_cnt; u++) {
        // 枚举两个端点是圆点但不是 u，并且经过 u 的路径数量
        tree.do_for_each_adj_vertex(u, [&](int v) {
            int sz = (v == fa[u] ? size[root[u]] - size[u] : size[v]);
            int other_sz = size[root[u]] - sz - (u < n);
            ans += 1LL * val[u] * sz * other_sz;
        });
        // 枚举以圆点 u 为端点的路径数量
        if (u < n) {
            ans += 1LL * val[u] * (size[root[u]] - 1) * 2;
        }
    }

    std::cout << ans << '\n';

    return 0;
}
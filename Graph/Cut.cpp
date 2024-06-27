#include <bits/stdc++.h>

/**
 * @brief 割点 Cut Point
 * @note 对于一个无向图，如果把一个点删除后这个图的极大连通分量数增加了，那么这个点就是这个图的割点（又称割顶）。
 * @note cut[u] is true -> u is a cut point.
 * @if u 不是根，且满足有一个孩子节点 v，low[v] >= dfn[u]，即不能回到祖先，则 u 是割点。
 * @if u 是根，且有两个或以上的孩子节点，则 u 是割点。
 * @link https://www.luogu.com.cn/problem/P3388
 * 
 * @brief 割边 Bridge
 * @brief 对于一个无向图，如果删掉一条边后图中的连通分量数增加了，则称这条边为桥或者割边。
 * @note 和割点类似，如果 low[v] > dfn[u], 则 (u, v) 是桥，并且不需要考虑根节点的问题。
 * @note bridge[u] is true -> (father[u], u) is a bridge.
 * @cite https://oi-wiki.org/graph/cut/
*/
auto get_cut(const std::vector<std::vector<int>>& g) {
    int n = g.size();
    std::vector<bool> cut(n);
    std::vector<bool> bridge(n);
    int cur = 0;
    std::vector<int> low(n), dfn(n, -1);
    auto tarjan = [&](auto &&self, int u, int f) -> void {
        low[u] = dfn[u] = cur++;
        int child = 0;
        for (auto v : g[u]) {
            if (dfn[v] == -1) {
                child++;
                self(self, v, u);
                low[u] = std::min(low[u], low[v]);
                // 如果 u 不是根节点，不能回到祖先
                if (f != -1 && low[v] >= dfn[u]) {
                    cut[u] = true;
                }
                if (low[v] > dfn[u]) {
                    bridge[v] = true;
                }
            } else if (v != f) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        // 如果 u 是根节点，有两个或以上的孩子节点
        if (f == -1 && child >= 2) {
            cut[u] = true;
        }
    };
    for (int i = 0; i < n; i++) {
        if (dfn[i] == -1) {
            tarjan(tarjan, i, -1);
        }
    }
    return std::pair{cut, bridge};
}
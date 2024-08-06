#pragma once
#include <bits/stdc++.h>

/**
 * @brief 强连通分量 tarjan
 * @link https://judge.yosupo.jp/problem/scc
 * @link 缩点：https://www.luogu.com.cn/problem/P3387
 * @note 按照逆拓扑序获得强连通分量
*/
auto scc(const std::vector<std::vector<int>>& g) {
    int n = g.size();
    int cur = 0;
    std::vector<int> low(n), dfn(n, -1), stk;
    std::vector<int> color(n, -1);
    int cnt = 0;

    auto tarjan = [&](auto &&self, int u) -> void {
        low[u] = dfn[u] = cur++;
        stk.push_back(u);
        for (auto v : g[u]) {
            if (dfn[v] == -1) self(self, v);
            if (color[v] == -1) low[u] = std::min(low[u], low[v]);
        }
        if (low[u] == dfn[u]) {
            int v;
            do {
                v = stk.back();
                stk.pop_back();
                color[v] = cnt;
            } while (u != v);
            cnt++;
        }
    };
    for (int i = 0; i < n; i++) if (dfn[i] == -1) tarjan(tarjan, i);
    return std::pair(cnt, color);
}
/**Note that strictly speaking this is not the tarjan's original algorithm
 * because we use a slightly different definition for lowlink. However this
 * algorithm is still correctly and easier to code.
 * In the tarjan's original algorithm, low means the smallest dfn that can be reached through *at most one reverse edge*,
 * but in this code, definition of low no longer limit at most one reverse edge.
 * before: 
 * if (dfn[v] == -1) {
        tarjan(v);
        low[u] = min(low[u], low[v]);
    } else if (color[v] == -1) {
        low[u] = min(low[u], dfn[v]);
    }
  * update:
  * if (dfn[v] == -1) tarjan(v);
    if (color[v] == -1) low[u] = min(low[u], low[v]);
*/
#pragma once
#include <bits/stdc++.h>

/**
 * @brief 强连通分量 tarjan
 * @link https://judge.yosupo.jp/problem/scc
 * @link 缩点：https://www.luogu.com.cn/problem/P3387
 * @note low[u] 的定义和原始的不同，不再限制只能通过一条反向边到达，而是可以通过任意数量的反向边到达的最小的 dfn 值
 * @note Components are numbered in *reverse topological order*, starting from 0. 
 * We no longer need on_stk array, we can replace `if (on_stk[v])` with `color[v] == -1` when update low array, 
 * because **if a node have dfn but do not have color, it must on stack**.
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
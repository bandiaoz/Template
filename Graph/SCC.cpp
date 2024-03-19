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
#include <bits/stdc++.h>

/**
 * Find strongly connected components of graph g. Components are numbered in *reverse topological order*,
 * starting from 0. It returns the number of components and an array which indicates which component
 * component each vertex belongs to.
 * We no longer need on_stk array, we can replace `if (on_stk[v])` with `color[v] == -1` when update low array, 
 * because if a node have dfn but do not have color, it must on stack.
*/
struct Strongly_Connected_Components {
    int n;
    std::vector<int> color;
    std::vector<std::vector<int>> components;

    Strongly_Connected_Components(const std::vector<std::vector<int>>& g) : n(g.size()), color(n, -1) {
        int cur = 0;
        std::vector<int> low(n), dfn(n, -1), stk;
        auto tarjan = [&](auto &&slf, int u) -> void {
            low[u] = dfn[u] = cur++;
            stk.push_back(u);
            for (auto v : g[u]) {
                if (dfn[v] == -1) slf(slf, v);
                if (color[v] == -1) low[u] = std::min(low[u], low[v]);
            }
            if (low[u] == dfn[u]) {
                std::vector<int> component;
                int v;
                do {
                    v = stk.back();
                    stk.pop_back();
                    color[v] = components.size();
                    component.push_back(v);
                } while (u != v);
                components.push_back(component);
            }
        };
        for (int i = 0; i < n; i++) if (dfn[i] == -1) tarjan(tarjan, i);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        g[u].push_back(v);
    }

    Strongly_Connected_Components scc(g);
    auto ans = scc.components;

    std::cout << ans.size() << "\n";
    for (int i = ans.size() - 1; i >= 0; --i) {
        std::cout << ans[i].size() << " ";
        for (int j = 0; j < ans[i].size(); ++j) {
            std::cout << ans[i][j] << " \n"[j == ans[i].size() - 1];
        }
    }

    return 0;
}

// test problem: https://judge.yosupo.jp/problem/scc
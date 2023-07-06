#include <bits/stdc++.h>

struct Strongly_Connected_Components {
    int n;
    std::vector<int> color;
    std::vector<std::vector<int>> components;

    
    Strongly_Connected_Components(const std::vector<std::vector<int>>& g) : n(g.size()), color(n, -1) {
        int cur = 0;
        std::vector<int> low(n), dfn(n, -1), stk;
        std::function<void(int)> tarjan = [&](int u) {
            low[u] = dfn[u] = cur++;
            stk.push_back(u);
            for (auto v : g[u]) {
                if (dfn[v] == -1) tarjan(v);
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
        for (int i = 0; i < n; i++) if (dfn[i] == -1) tarjan(i);
    }
};

/**
 * @brief 2-SAT
 * @note all of (u is f or v is g) is satisfiable
 * @note 2 * u + 1 means u is true, 2 * u means u is false
 * @note if (u is true or v is false), addClause(u, true, v, false)
 * @note as Tarjan's algorithm finds node in reverse topological order,
 *       id[2 * i] > id[2 * i + 1] => there might be a path fron !i to i
 *       so it's safe to set i = true
*/
struct TwoSat {
private:
    int n;
    std::vector<std::vector<int>> G;
    std::vector<bool> ans;

public:
    TwoSat(int n) : n(n), G(2 * n), ans(n) {}
    void addClause(int u, bool f, int v, bool g) {
        G[2 * u + !f].push_back(2 * v + g);
        G[2 * v + !g].push_back(2 * u + f);
    }
    bool satisfiable() {
        Strongly_Connected_Components scc(G);
        auto id = scc.color;
        for (int i = 0; i < n; ++i) {
            if (id[2 * i] == id[2 * i + 1]) return false;
            ans[i] = id[2 * i] > id[2 * i + 1];
        }
        return true;
    }
    std::vector<bool> answer() { return ans; }
};
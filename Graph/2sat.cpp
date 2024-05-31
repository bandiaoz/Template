#pragma once
#include "SCC.cpp"

/**
 * @brief 2-SAT
 * @note all of (u is f or v is g) is satisfiable
 * @note 2 * u + 1 means u is true, 2 * u means u is false
 * @note if (u is true or v is false), addClause(u, true, v, false)
 * @note as Tarjan's algorithm finds node in reverse topological order,
 *       id[2 * i] > id[2 * i + 1] => there might be a path from !i to i
 *       so it's safe to set i = true
 * @link https://www.luogu.com.cn/problem/P4782
 * @link https://judge.yosupo.jp/problem/two_sat
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
        auto [_, id] = scc(G);
        for (int i = 0; i < n; ++i) {
            if (id[2 * i] == id[2 * i + 1]) return false;
            ans[i] = id[2 * i] > id[2 * i + 1];
        }
        return true;
    }
    std::vector<bool> answer() { return ans; }
};
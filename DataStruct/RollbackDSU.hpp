#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

struct RollbackDSU {
    std::vector<int> f;
    std::vector<std::pair<int, int>> joins;

    RollbackDSU() {}
    explicit RollbackDSU(int n) { init(n); }
    void init(int n) { f.assign(n, -1); }
    int leader(int x) const { return f[x] < 0 ? x : leader(f[x]); }
    bool same(int x, int y) const { return leader(x) == leader(y); }
    int size(int x) const { return -f[leader(x)]; }
    int time() const { return joins.size(); }
    bool merge(int x, int y) {
        x = leader(x), y = leader(y);
        if (x == y) return false;
        if (f[x] > f[y]) std::swap(x, y);
        joins.emplace_back(y, f[y]);
        f[x] += f[y], f[y] = x;
        return true;
    }
    void rollback(int t) {
        while (joins.size() > t) {
            auto [i, pi] = joins.back();
            joins.pop_back();
            assert(f[f[i]] < 0);
            f[f[i]] -= pi;
            f[i] = pi;
        }
    }
};

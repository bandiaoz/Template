#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

struct DSU {
    std::vector<int> f, siz;

    DSU() {}
    DSU(int n) { init(n); }
    void init(int n) {
        f.resize(n);
        std::iota(f.begin(), f.end(), 0);
        siz.assign(n, 1);
    }
    int leader(int x) { return f[x] == x ? x : f[x] = leader(f[x]); }
    bool same(int x, int y) { return leader(x) == leader(y); }
    int size(int x) { return siz[leader(x)]; }
    bool merge(int x, int y) {
        x = leader(x);
        y = leader(y);
        if (x == y) {
            return false;
        }
        siz[x] += siz[y];
        f[y] = x;
        return true;
    }
    std::vector<int> leader_set() {
        std::vector<int> res;
        for (int i = 0; i < f.size(); i++) {
            if (f[i] == i) {
                res.push_back(i);
            }
        }
        return res;
    }
    friend DSU merge(const DSU &a, const DSU &b) {
        assert(a.f.size() == b.f.size());
        DSU res = a;
        for (int i = 0; i < b.f.size(); i++) {
            res.merge(i, b.f[i]);
        }
        return res;
    }
};
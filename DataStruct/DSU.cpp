#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct DSU {
    vector<int> f, sz;
    DSU(int n) : f(n), sz(n, 1) { iota(f.begin(), f.end(), 0); }
    int findR(int x) { return x == f[x] ? x : f[x] = findR(f[x]); }
    bool same(int x, int y) { return findR(x) == findR(y); }
    bool merge(int x, int y) {
        x = findR(x), y = findR(y);
        if (x == y) return false;
        sz[x] += sz[y], f[y] = x;
        return true;
    }
    int size(int x) { return sz[findR(x)]; }
};
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct Tree {
    vector<int> sz, top, dep, parent, in, out;
    int cur;
    vector<vector<int>> e;
    Tree(int n) : sz(n), top(n), dep(n), parent(n, -1), in(n), out(n), cur(0), e(n) {}
    void addEdge(int u, int v) {
        e[u].push_back(v);
        e[v].push_back(u);
    }
    void init() {
        dfsSz(0);
        dfsHLD(0);
    }
    void dfsSz(int u) {
        if (parent[u] != -1) {
            e[u].erase(find(e[u].begin(), e[u].end(), parent[u]));
        }
        sz[u] = 1;
        for (int &v : e[u]) {
            parent[v] = u;
            dep[v] = dep[u] + 1;
            dfsSz(v);
            sz[u] += sz[v];
            if (sz[v] > sz[e[u][0]]) {
                swap(v, e[u][0]);
            }
        }
    }
    void dfsHLD(int u) {
        in[u] = cur++;
        for (int v : e[u]) {
            top[v] = (v == e[u][0] ? top[u] : v);
            dfsHLD(v);
        }
        out[u] = cur;
    }
    bool isAncestor(int u, int v) {
        return in[v] > in[u] && out[v] <= out[u];
    }
    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                swap(u, v);
            }
            u = parent[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }
};
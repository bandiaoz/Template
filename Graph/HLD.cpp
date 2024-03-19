#include <bits/stdc++.h>

struct HLD {
    int n;
    std::vector<int> sz, top, dep, fa, in, out, seq;
    std::vector<std::vector<int>> g;
    int cur;
    
    HLD() {}
    HLD(int n) {
        init(n);
    }
    void init(int n) {
        this->n = n;
        sz.resize(n);
        top.resize(n);
        dep.resize(n);
        fa.resize(n);
        in.resize(n);
        out.resize(n);
        seq.resize(n);
        cur = 0;
        g.assign(n, {});
    }
    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void work(int root = 0) {
        top[root] = root;
        dep[root] = 0;
        fa[root] = -1;
        dfs1(root);
        dfs2(root);
    }
    void dfs1(int u) {
        if (fa[u] != -1) {
            g[u].erase(std::find(g[u].begin(), g[u].end(), fa[u]));
        }
        
        sz[u] = 1;
        for (auto &v : g[u]) {
            fa[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) {
                std::swap(v, g[u][0]);
            }
        }
    }
    void dfs2(int u) {
        in[u] = cur++;
        seq[in[u]] = u;
        for (auto v : g[u]) {
            top[v] = v == g[u][0] ? top[u] : v;
            dfs2(v);
        }
        out[u] = cur;
    }
    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] > dep[top[v]]) {
                u = fa[top[u]];
            } else {
                v = fa[top[v]];
            }
        }
        return dep[u] < dep[v] ? u : v;
    }
    
    int dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }
    
    int jump(int u, int k) {
        if (dep[u] < k) return -1;
        
        int d = dep[u] - k;
        while (dep[top[u]] > d) {
            u = fa[top[u]];
        }
        
        return seq[in[u] - dep[u] + d];
    }
    
    bool isAncestor(int u, int v) {
        return in[u] <= in[v] && in[v] < out[u];
    }
    
    int rootedChild(int u, int v) {
        if (u == v) {
            return u;
        }
        if (!isAncestor(u, v)) {
            return fa[u];
        }
        auto it = std::upper_bound(g[u].begin(), g[u].end(), v, [&](int x, int y) {
            return in[x] < in[y];
        }) - 1;
        return *it;
    }
    
    int rootedSize(int u, int v) {
        if (u == v) {
            return n;
        }
        if (!isAncestor(v, u)) {
            return sz[v];
        }
        return n - sz[rootedChild(v, u)];
    }
    
    int rootedLca(int a, int b, int c) {
        return lca(a, b) ^ lca(b, c) ^ lca(c, a);
    }
};

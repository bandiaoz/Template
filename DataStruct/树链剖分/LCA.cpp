#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<vector<int>> g(n);
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<int> fa(n, -1), dep(n), sz(n);
    int clk = 0;
    vector<int> top(n), in(n), out(n);
    auto pre = a;

    function<void(int)> dfs1 = [&](int u) {
        if (u > 0) {
            g[u].erase(find(g[u].begin(), g[u].end(), fa[u]));
        }
        sz[u] = 1;
        for (auto &v : g[u]) {
            pre[v] ^= pre[u];
            fa[v] = u, dep[v] = dep[u] + 1;
            dfs1(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) swap(g[u][0], v);
        }
    };
    dfs1(0);

    function<void(int)> dfs2 = [&](int u) {
        in[u] = clk++;
        for (auto v : g[u]) {
            top[v] = v == g[u][0] ? top[u] : v;
            dfs2(v);
        }
        out[u] = clk;
    };
    dfs2(0);
    
    auto lca = [&](int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            u = fa[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    };
    
    while (q--) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        cout << (pre[u] ^ pre[v] ^ a[lca(u, v)]) << "\n";
    }

    return 0;
}
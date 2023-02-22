#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> a(n);
    vector<vector<int>> g(n);
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

    vector<int> fa(n, -1), sz(n, 1);
    function<void(int)> dfs_son = [&](int u) {
        if (u > 0) {
            g[u].erase(find(g[u].begin(), g[u].end(), fa[u]));
        }
        for (auto &v : g[u]) {
            fa[v] = u;
            dfs_son(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) {
                swap(v, g[u][0]);
            }
        }
    };
    
    dfs_son(0);

    int flag = -1, maxx = 0;
    vector<int> cnt(n + 1);
    vector<ll> ans(n);
    ll sum = 0;
    function<void(int, int)> count = [&](int u, int val) {
        cnt[a[u]] += val;
        if (cnt[a[u]] > maxx) {
            maxx = cnt[a[u]];
            sum = a[u];
        } else if (cnt[a[u]] == maxx) {
            sum += a[u];
        }
        for (auto v : g[u]) {
            if (v == flag) continue;
            count(v, val);
        }
    };

    function<void(int, bool)> dfs_dsu = [&](int u, bool keep) {
        // 搞轻儿子及其子树算答案删贡献
        for (auto v : g[u]) {
            if (v == g[u][0]) continue;
            dfs_dsu(v, 0);
        }
        // 搞重儿子及其子树算答案不删贡献
        if (g[u].size()) {
            dfs_dsu(g[u][0], true);
            flag = g[u][0];
        }
        // 暴力统计 u 及其所有轻儿子的贡献合并到刚算出的重儿子信息里
        count(u, 1);
        flag = -1;
        ans[u] = sum;
        // 把需要删除的贡献删一删
        if (!keep) {
            count(u, -1);
            sum = maxx = 0;
        }
    };

    dfs_dsu(0, false);

    for (int i = 0; i < n; ++i) {
        cout << ans[i] << " \n"[i == n - 1];
    }
    
    return 0;
}

// https://codeforces.com/problemset/problem/600/E
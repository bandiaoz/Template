#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <typename T>
struct Fenwick {
    const int n;
    vector<T> a;
    Fenwick(int n) : n(n), a(n) {}
    void add(int x, T v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] += v;
        }
    }
    // return the sum of [0, x)
    T sum(int x) {
        T ans = 0;
        for (int i = x; i > 0; i -= i & -i) {
            ans += a[i - 1];
        }
        return ans;
    }
    // return the sum of [l, r)
    T rangeSum(int l, int r) {
        return sum(r) - sum(l);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<vector<pair<int, int>>> g(n);
    vector<int> w(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v >> w[i];
        u--, v--;
        g[u].emplace_back(v, i);
        g[v].emplace_back(u, i);
    }

    int k;
    cin >> k;

    vector<int> sz(n);
    vector<bool> vis(n);
    Fenwick<int> fen(k + 1);
    function<void(int, int, int, int&)> dfs_rt = [&](int u, int f, int tot, int &rt) {
        int maxx = 0;
        sz[u] = 1;
        for (auto [v, j] : g[u]) {
            if (v == f || vis[v]) continue;
            dfs_rt(v, u, tot, rt);
            sz[u] += sz[v];
            maxx = max(maxx, sz[v]);
        }
        maxx = max(maxx, tot - sz[u]);
        if (maxx * 2 <= tot) {
            rt = u;
        }
    };

    function<void(int, int)> dfs_sz = [&](int u, int f) {
        sz[u] = 1;
        for (auto [v, j] : g[u]) {
            if (v == f || vis[v]) continue;
            dfs_sz(v, u);
            sz[u] += sz[v];
        }
    };

    vector<int> d;
    function<void(int, int, int)> dfs_dis = [&](int u, int f, int dis) {
        d.push_back(dis);
        for (auto [v, j] : g[u]) {
            if (v == f || vis[v]) continue;
            dfs_dis(v, u, dis + w[j]);
        }
    };

    function<void(int, int, int)> dfs_clear = [&](int u, int f, int dis) {
        if (dis) fen.add(dis, -1);
        for (auto [v, j] : g[u]) {
            if (v == f || vis[v]) continue;
            dfs_clear(v, u, dis + w[j]);
        }
    };
    
    function<int(int, int)> work = [&](int u, int tot) {
        int rt = u;
        dfs_rt(u, -1, tot, rt);
        dfs_sz(rt, -1);
        vis[rt] = true;

        int ans = 0;
        for (auto [v, j] : g[rt]) {
            if (vis[v]) continue;
            d.clear();
            dfs_dis(v, rt, w[j]);
            for (auto dd : d) {
                if (dd <= k) {
                    ans += fen.sum(k - dd + 1) + 1;
                }
            }
            for (auto dd : d) {
                fen.add(dd, 1);
            }
        }
        dfs_clear(rt, -1, 0);
        for (auto [v, j] : g[rt]) {
            if (vis[v]) continue;
            ans += work(v, sz[v]);
        }
        return ans;
    };

    cout << work(0, n) << "\n";

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P4178
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> g(n);
    vector<int> w(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v >> w[i];
        u--, v--;
        g[u].emplace_back(v, i);
        g[v].emplace_back(u, i);
    }

    vector<int> ans(m), Q(m);
    for (int i = 0; i < m; ++i) {
        cin >> Q[i];
    }

    vector<int> sz(n);
    vector<bool> vis(n);
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


    vector<bool> mpd(10000001);
    int cnt;
    vector<int> d(n);

    function<void(int, int, int)> dfs_ans = [&](int u, int f, int dis) {
        ++cnt;
        d[u] = dis;
        for (int i = 0; i < m; ++i) {
            if (d[u] == Q[i]) {
                ans[i] = true;
            } else if (d[u] < Q[i]) {
                ans[i] |= mpd[Q[i] - d[u]];
            }
        }
        for (auto [v, j] : g[u]) {
            if (v == f || vis[v]) continue;
            dfs_ans(v, u, dis + w[j]);
        }
    };

    function<void(int, int, int)> dfs_dis = [&](int u, int f, int flag) {
        for (int i = 0; i < m; ++i) {
            if (d[u] <= Q[i]) {
                mpd[d[u]] = (flag == 1);
            }
        }
        for (auto [v, j] : g[u]) {
            if (v == f || vis[v]) continue;
            dfs_dis(v, u, flag);
        }
    };


    function<void(int, int)> work = [&](int u, int tot) {
        int rt = u;
        dfs_rt(u, -1, tot, rt);
        dfs_sz(rt, -1);
        vis[rt] = true;


        for (auto [v, j] : g[rt]) {
            if (vis[v]) continue;
            dfs_ans(v, rt, w[j]);
            dfs_dis(v, rt, 1);
        }

        dfs_dis(rt, -1, -1);
        
        for (auto [v, j] : g[rt]) {
            if (vis[v]) continue;
            work(v, sz[v]);
        }
    };

    work(0, n);

    for (int i = 0; i < m; ++i) {
        cout << (ans[i] ? "AYE" : "NAY") << "\n";
    }

    return 0;
}
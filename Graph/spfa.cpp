#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const int inf = 1e9;

void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> g(n);
    vector<int> w(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v >> w[i];
        u--, v--;
        g[u].emplace_back(v, i);
        if (w[i] >= 0) {
            g[v].emplace_back(u, i);
        }
    }

    auto spfa = [&](int s) {
        vector<int> dis(n, inf), cnt(n);
        vector<bool> vis(n);
        dis[s] = 0;
        vis[s] = true;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            vis[u] = false;
            for (auto [v, j] : g[u]) {
                if (dis[v] > dis[u] + w[j]) {
                    dis[v] = dis[u] + w[j];
                    cnt[v] = cnt[u] + 1;
                    if (cnt[v] >= n) {
                        return false;
                    }
                    if (vis[v] == false) {
                        q.push(v);
                        vis[v] = true;
                    }
                }
            }
        }

        return true;
    };

    cout << (spfa(0) ? "NO\n" : "YES\n");
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}

// test problem: https://www.luogu.com.cn/problem/P3385
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// O(n_1 \cdot m + n_2) 
// if n1 < n2: swap(n1, n2) 可以提高效率
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n1, n2, m;
    cin >> n1 >> n2 >> m;

    vector<vector<int>> g(n1);
    while (m--) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[u].push_back(v);
    }

    int ans = 0;
    vector<int> matchu(n1, -1), matchv(n2, -1);
    for (int i = 0; i < n1; ++i) {
        vector<int> vis(n2);
        
         function<bool(int)> find = [&](int u) {
            for (auto v : g[u]) {
                if (vis[v]) continue;
                vis[v] = true;
                if (matchv[v] == -1 || find(matchv[v])) {
                    matchv[v] = u;
                    matchu[u] = v;
                    return true;
                }
            }
            return false;
        };

        if (find(i)) {
            ans++;
        }
    }
    
    cout << ans << "\n";
    for (int i = 0; i < n1; ++i) {
        cout << matchu[i] + 1 << " \n"[i == n1 - 1];
    }

    return 0;
}
// test problem: https://uoj.ac/problem/78
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// O(sqrt(n)*m)
struct HopcroftKarp {
    vector<int> g, l, r;
    int ans;
    HopcroftKarp(int n, int m, const vector<pair<int, int>>& e)
        : g(e.size()), l(n, -1), r(m, -1), ans(0) {
        vector<int> deg(n + 1);
        for (auto& [x, y] : e) deg[x]++;
        for (int i = 1; i <= n; i++) deg[i] += deg[i - 1];
        for (auto& [x, y] : e) g[--deg[x]] = y;

        vector<int> a, p, q(n);
        for (;;) {
            a.assign(n, -1), p.assign(n, -1);
            int t = 0;
            for (int i = 0; i < n; i++)
                if (l[i] == -1) q[t++] = a[i] = p[i] = i;

            bool match = false;
            for (int i = 0; i < t; i++) {
                int x = q[i];
                if (~l[a[x]]) continue;
                for (int j = deg[x]; j < deg[x + 1]; j++) {
                    int y = g[j];
                    if (r[y] == -1) {
                        while (~y) r[y] = x, swap(l[x], y), x = p[x];
                        match = true, ans++;
                        break;
                    }

                    if (p[r[y]] == -1) q[t++] = y = r[y], p[y] = x, a[y] = a[x];
                }
            }

            if (!match) break;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, r, m;
    cin >> l >> r >> m;
    vector<pair<int, int>> e(m);
    for (auto& [x, y] : e) {
        cin >> x >> y;
        x--, y--;
    }

    HopcroftKarp hk(l, r, e);
    cout << hk.ans << "\n";

    for (int i = 0; i < l; i++) {
        cout << hk.l[i] + 1 << " \n"[i == l - 1];
    }

    return 0;
}
// test problem: https://uoj.ac/problem/78
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

/** Modified from:
 * https://oi-wiki.org/graph/cut/
 * If the node is a root node and has at least 2 child nodes, 
 * or the node have a child node that `low[v] >= dfn[u]`, can go to father node at most.
 * If you need to cut_edge, just change one thing: `low[v] > dfn[u]`, and you don't need to think about the root node.
 * if (low[v] > dfn[u]) {
        isbridge[v] = true;
        ++cnt_bridge;
    }
 * isbridge[x] is true -> (father[u], u) is a bridge.
*/
struct Cut_Point {
    int n;
    vector<bool> is_cut;

    Cut_Point(const vector<vector<int>>& g) : n(g.size()), is_cut(n) {
        int cur = 0;
        vector<int> low(n), dfn(n, -1);
        function<void(int, int)> tarjan = [&](int u, int f) {
            low[u] = dfn[u] = cur++;
            int child = 0;
            for (auto v : g[u]) {
                if (dfn[v] == -1) {
                    child++;
                    tarjan(v, u);
                    low[u] = min(low[u], low[v]);
                    if (u != f && low[v] >= dfn[u]) {
                        is_cut[u] = true;
                    }
                } else if (v != f) {
                    low[u] = min(low[u], dfn[v]);
                }
            }
            if (u == f && child >= 2) {
                is_cut[u] = true;
            }
        };
        for (int i = 0; i < n; i++) if (dfn[i] == -1) tarjan(i, i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    Cut_Point cut_point(g);
    cout << accumulate(cut_point.is_cut.begin(), cut_point.is_cut.end(), 0) << "\n";
    for (int i = 0; i < n; ++i) {
        if (cut_point.is_cut[i]) {
            cout << i + 1 << " ";
        }
    }

    return 0;
}
// test problem: https://www.luogu.com.cn/problem/P3388
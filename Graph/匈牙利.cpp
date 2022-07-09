#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 505;
int n1, n2, m, match[maxn];
vector<int> g[maxn];
bool vis[maxn];
bool find(int u) {
    for (auto v : g[u]) {
        if (vis[v]) continue;
        vis[v] = 1;
        if (match[v] == 0 || find(match[v])) {
            match[v] = u;
            return 1;
        }
    }
    return 0;
}
int main() {
    scanf("%d%d%d", &n1, &n2, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
    }
    int ans = 0;
    for (int i = 1; i <= n1; ++i) {
        memset(vis, false, sizeof(vis));
        if (find(i)) ++ans;
    }
    printf("%d\n", ans);
    return 0;
}
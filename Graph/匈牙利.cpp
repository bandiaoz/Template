#include <bits/stdc++.h>
#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)
#ifdef LOCAL
#include "Print.h"
#define de(...) W('[', #__VA_ARGS__,"] =", __VA_ARGS__)
#else
#define de(...)
#endif
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
int case_Test() {
    scanf("%d%d%d", &n1, &n2, &m);
    while (m--) {
        int u, v;
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
    }
    int ans = 0;
    rep(i, 1, n1) {
        memset(vis, false, sizeof(vis));
        if (find(i)) ++ans;
    }
    printf("%d\n", ans);
    return 0;
}
int main() {
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    clock_t start = clock();
#endif
    int _ = 1;
    // scanf("%d", &_);
    while (_--) case_Test();
#ifdef LOCAL
    printf("Time used: %.3lfs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
#endif
    return 0;
}
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
const int maxn = 3e4 + 5;
int fa[maxn], sz[maxn], d[maxn]; // d 表示与父亲结点的关系
int findR(int x) {
    if (x == fa[x]) return x;
    int rt = findR(fa[x]);
    d[x] += d[fa[x]];
    return fa[x] = rt;
}
void link(int x, int y, int f) {
    int xx = findR(x), yy = findR(y);
    fa[xx] = yy, d[xx] += sz[yy];
    sz[yy] += sz[xx];
}
int query(int x, int y) {
    if (x == y) return 0;
    int xx = findR(x), yy = findR(y);
    if (xx != yy) return -1;
    return abs(d[x] - d[y]) - 1;
}
int main() {
    int T;
    scanf("%d", &T);
    rep(i, 1, maxn - 1) fa[i] = i, sz[i] = 1;
    while (T--) {
        char op[5]; int x, y;
        scanf("%s%d%d", op + 1, &x, &y);
        if (op[1] == 'M') link(x, y, 1);
        else printf("%d\n", query(x, y));
    }
    return 0;
}

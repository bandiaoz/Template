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
const int maxn = 1e5 + 5;
int n, q, a[maxn];
vector<int> g[maxn];
int sz[maxn], id[maxn], idd[maxn], cnt;
void dfs(int u, int f) {
    sz[u] = 1, id[u] = ++cnt, idd[cnt] = u;
    for (auto v : g[u]) {
        if (v == f) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}
struct segtree{
#define tr t[root]
#define lson t[root << 1]
#define rson t[root << 1 | 1]
    struct node {
        int l, r, maxx, minn;
        int add, cov;
    } t[maxn << 2];
    void build(int root, int l, int r) {
        tr.l = l, tr.r = r, tr.add = 0, tr.cov = -1;
        if (l == r) {
            tr.maxx = tr.minn = a[idd[l]];
            return;
        }
        int mid = (l + r) >> 1;
        build(root << 1, l, mid);
        build(root << 1 | 1, mid + 1, r);
        pushup(root);
    }
    void pushup(int root) {
        tr.maxx = max(lson.maxx, rson.maxx);
        tr.minn = min(lson.minn, rson.minn);
    }
    void spdCov(int root) {
        lson.minn = rson.minn = tr.cov;
        lson.maxx = rson.maxx = tr.cov;
        lson.cov = rson.cov = tr.cov;
    }
    void spdAdd(int root) {
        if (~lson.cov) {
            if (lson.l != lson.r) spdCov(root << 1);
            lson.cov = -1, lson.add = 0;
        }
        if (~rson.cov) {
            if (rson.l != rson.r) spdCov(root << 1 | 1);
            rson.cov = -1, rson.add = 0;
        }
        lson.minn += tr.add, rson.minn += tr.add;
        lson.maxx += tr.add, rson.maxx += tr.add;
        lson.add += tr.add, rson.add += tr.add;
    }
    void spread(int root) {
        if (~tr.cov) {
            if (tr.l != tr.r) spdCov(root);
            tr.cov = -1, tr.add = 0;
        }
        if (tr.add) {
            if (tr.l != tr.r) spdAdd(root);
            tr.add = 0;
        }
    }
    void cov(int root, int l, int r, int x) {
        spread(root);
        if (l <= tr.l && tr.r <= r) {
            tr.minn = x, tr.maxx = x;
            tr.add = 0, tr.cov = x;
            return;
        }
        int mid = (tr.l + tr.r) >> 1;
        if (l <= mid) cov(root << 1, l, r, x);
        if (r > mid) cov(root << 1 | 1, l, r, x);
        pushup(root);
    }
    void add(int root, int l, int r, int x) {
        spread(root);
        if (l <= tr.l && tr.r <= r) {
            tr.minn += x, tr.maxx += x;
            tr.add += x;
            return;
        }
        int mid = (tr.l + tr.r) >> 1;
        if (l <= mid) add(root << 1, l, r, x);
        if (r > mid) add(root << 1 | 1, l, r, x);
        pushup(root);
    }
    int qmax(int root, int l, int r) {
        spread(root);
        if (l <= tr.l && tr.r <= r) return tr.maxx;
        int mid = (tr.l + tr.r) >> 1, ans = 0;  
        if (l <= mid) ans = max(ans, qmax(root << 1, l, r));
        if (r > mid) ans = max(ans, qmax(root << 1 | 1, l, r));
        return ans;
    }
    int qmin(int root, int l, int r) {
        spread(root);
        if (l <= tr.l && tr.r <= r) return tr.minn;
        int mid = (tr.l + tr.r) >> 1, ans = 2e9;
        if (l <= mid) ans = min(ans, qmin(root << 1, l, r));
        if (r > mid) ans = min(ans, qmin(root << 1 | 1, l, r));
        return ans;
    }
} Tr;
inline void add(int u, int val) { Tr.add(1, id[u], id[u] + sz[u] - 1, val); }
inline void cov(int u, int val) { Tr.cov(1, id[u], id[u] + sz[u] - 1, val); }
inline int qry(int u) {
    int l = id[u], r = id[u] + sz[u] - 1;
    return Tr.qmax(1, l, r) - Tr.qmin(1, l, r);
}
int case_Test() {
    scanf("%d%d", &n, &q);
    rep(i, 1, n) scanf("%d", &a[i]);
    rep(i, 1, n - 1) {
        int u, v;
        scanf("%d%d", &u, &v);
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    dfs(1, 0), Tr.build(1, 1, n);
    while (q--) {
        int op, x, V;
        scanf("%d%d", &op, &x);
        if (op == 0) scanf("%d", &V), add(x, V);
        if (op == 1) scanf("%d", &V), cov(x, V);
        if (op == 2) printf("%d\n", qry(x));
    }
    return 0;
}
int main() {
#ifdef LOCAL
    freopen("/Users/chenjinglong/cpp_code/in.in", "r", stdin);
    freopen("/Users/chenjinglong/cpp_code/out.out", "w", stdout);
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
// 【月下“毛景树“】https://www.luogu.com.cn/problem/P4315
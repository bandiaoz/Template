//洛谷P3384
#pragma region
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
using namespace std;
typedef long long ll;
#define tr t[root]
#define lson t[root << 1]
#define rson t[root << 1 | 1]
#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)
namespace fastIO {
#define BUF_SIZE 100000
#define OUT_SIZE 100000
//fread->R
bool IOerror = 0;
//inline char nc(){char ch=getchar();if(ch==-1)IOerror=1;return ch;}
inline char nc() {
    static char buf[BUF_SIZE], *p1 = buf + BUF_SIZE, *pend = buf + BUF_SIZE;
    if (p1 == pend) {
        p1 = buf;
        pend = buf + fread(buf, 1, BUF_SIZE, stdin);
        if (pend == p1) {
            IOerror = 1;
            return -1;
        }
    }
    return *p1++;
}
inline bool blank(char ch) { return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t'; }
template <class T>
inline bool R(T &x) {
    bool sign = 0;
    char ch = nc();
    x = 0;
    for (; blank(ch); ch = nc())
        ;
    if (IOerror)
        return false;
    if (ch == '-')
        sign = 1, ch = nc();
    for (; ch >= '0' && ch <= '9'; ch = nc())
        x = x * 10 + ch - '0';
    if (sign)
        x = -x;
    return true;
}
inline bool R(double &x) {
    bool sign = 0;
    char ch = nc();
    x = 0;
    for (; blank(ch); ch = nc())
        ;
    if (IOerror)
        return false;
    if (ch == '-')
        sign = 1, ch = nc();
    for (; ch >= '0' && ch <= '9'; ch = nc())
        x = x * 10 + ch - '0';
    if (ch == '.') {
        double tmp = 1;
        ch = nc();
        for (; ch >= '0' && ch <= '9'; ch = nc())
            tmp /= 10.0, x += tmp * (ch - '0');
    }
    if (sign)
        x = -x;
    return true;
}
inline bool R(char *s) {
    char ch = nc();
    for (; blank(ch); ch = nc())
        ;
    if (IOerror)
        return false;
    for (; !blank(ch) && !IOerror; ch = nc())
        *s++ = ch;
    *s = 0;
    return true;
}
inline bool R(char &c) {
    c = nc();
    if (IOerror) {
        c = -1;
        return false;
    }
    return true;
}
template <class T, class... U>
bool R(T &h, U &... t) { return R(h) && R(t...); }
#undef OUT_SIZE
#undef BUF_SIZE
};  // namespace fastIO
using namespace fastIO;
template <class T>
void _W(const T &x) { cout << x; }
void _W(const int &x) { printf("%d", x); }
void _W(const int64_t &x) { printf("%lld", x); }
void _W(const double &x) { printf("%.16f", x); }
void _W(const char &x) { putchar(x); }
void _W(const char *x) { printf("%s", x); }
template <class T, class U>
void _W(const pair<T, U> &x) { _W(x.F), putchar(' '), _W(x.S); }
template <class T>
void _W(const vector<T> &x) {
    for (auto i = x.begin(); i != x.end(); _W(*i++))
        if (i != x.cbegin()) putchar(' ');
}
void W() {}
template <class T, class... U>
void W(const T &head, const U &... tail) { _W(head), putchar(sizeof...(tail) ? ' ' : '\n'), W(tail...); }
#pragma endregion
const int maxn = 1e5 + 5;
int n, m, r, mod;
int w[maxn];
vector<int> g[maxn];
int fa[maxn], sz[maxn], dep[maxn], son[maxn];
int id[maxn], cnt, wt[maxn], top[maxn];
void init() {
    rep(i, 1, n) {
        g[i].clear();
        son[i] = 0;
    }
}
void dfs1(int u, int f, int deep) {
    dep[u] = deep, fa[u] = f, sz[u] = 1;
    for (auto v : g[u]) {
        if (v == f) continue;
        dfs1(v, u, deep + 1);
        sz[u] += sz[v];
        if (sz[v] > sz[son[u]]) son[u] = v;
    }
}
void dfs2(int u, int topf) {
    id[u] = ++cnt, wt[cnt] = w[u], top[u] = topf;
    if (!son[u]) return;
    dfs2(son[u], topf);
    for (auto v : g[u]) {
        if (v == fa[u] || v == son[u]) continue;
        dfs2(v, v);
    }
}
struct segtree {
    int l, r, val, lazy;
} t[maxn << 2];
void build(int root, int l, int r) {
    tr.l = l, tr.r = r, tr.lazy = 0;
    if (l == r) {
        tr.val = wt[l] % mod;
        return;
    }
    int mid = (l + r) >> 1;
    build(root << 1, l, mid);
    build(root << 1 | 1, mid + 1, r);
    tr.val = (lson.val + rson.val) % mod;
}
void spread(int root) {
    if (tr.lazy) {
        lson.val = (lson.val + tr.lazy * (lson.r - lson.l + 1)) % mod;
        rson.val = (rson.val + tr.lazy * (rson.r - rson.l + 1)) % mod;
        lson.lazy = (lson.lazy + tr.lazy) % mod;
        rson.lazy = (rson.lazy + tr.lazy) % mod;
        tr.lazy = 0;
    }
}
int query(int root, int l, int r) {
    if (l <= tr.l && tr.r <= r) return tr.val % mod;
    spread(root);
    int ans = 0;
    int mid = (tr.l + tr.r) >> 1;
    if (l <= mid) ans = (ans + query(root << 1, l, r)) % mod;
    if (r > mid) ans = (ans + query(root << 1 | 1, l, r)) % mod;
    return ans;
}
void update(int root, int l, int r, int x) {
    if (l <= tr.l && tr.r <= r) {
        tr.val = (tr.val + x * (tr.r - tr.l + 1)) % mod;
        tr.lazy = (tr.lazy + x) % mod;
        return;
    }
    spread(root);
    int mid = (tr.l + tr.r) >> 1;
    if (l <= mid) update(root << 1, l, r, x);
    if (r > mid) update(root << 1 | 1, l, r, x);
    tr.val = (lson.val + rson.val) % mod;
}
int qSon(int x) { return query(1, id[x], id[x] + sz[x] - 1); }
void updSon(int x, int k) { update(1, id[x], id[x] + sz[x] - 1, k); }
int qRange(int x, int y) {
    int ans = 0;
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        ans = (ans + query(1, id[top[x]], id[x])) % mod;
        x = fa[top[x]];
    }
    if (dep[x] > dep[y]) swap(x, y);
    ans = (ans + query(1, id[x], id[y])) % mod;
    return ans;
}
void updRange(int x, int y, int k) {
    k %= mod;
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        update(1, id[top[x]], id[x], k);
        x = fa[top[x]];
    }
    if (dep[x] > dep[y]) swap(x, y);
    update(1, id[x], id[y], k);
}
int main() {
    R(n, m, r, mod);
    rep(i, 1, n) R(w[i]);
    rep(i, 1, n - 1) {
        int u, v;
        R(u, v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(r, 0, 1);
    dfs2(r, r);
    build(1, 1, n);
    while (m--) {
        int op, x, y, z;
        R(op);
        if (op == 1)
            R(x, y, z), updRange(x, y, z);
        else if (op == 2)
            R(x, y), W(qRange(x, y));
        else if (op == 3)
            R(x, y), updSon(x, y);
        else
            R(x), W(qSon(x));
    }
}
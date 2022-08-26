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
namespace fhq {
#define tr t[root]
#define lson t[tr.lc]
#define rson t[tr.rc]
mt19937 rnd(233);
struct node {
    int lc, rc, val, key, sz;
    bool tag;
} t[maxn];
int cnt, Root;
// 重新计算以 root 为根的子树大小
inline void update(int root) { tr.sz = lson.sz + rson.sz + 1; }
// 新建一个权值为val的结点
int newNode(int val) {
    t[++cnt] = {0, 0, val, (int)rnd(), 1, 0};
    return cnt;
}
inline void pushdown(int root) {
    swap(tr.lc, tr.rc);
    lson.tag ^= 1, rson.tag ^= 1;
    tr.tag = false;
}
// 合并成小根堆，参数保证x树的权值严格小于y树的权值
int merge(int x, int y) {
    if (!x || !y) return x + y;
    if (t[x].key < t[y].key) {
        if (t[x].tag) pushdown(x);
        t[x].rc = merge(t[x].rc, y);
        update(x); return x;
    } else {
        if (t[y].tag) pushdown(y);
        t[y].lc = merge(x, t[y].lc);
        update(y); return y;
    }
}
// 在以 root 为根的子树内树按值分裂，x树的大小等于k
void split_sz(int root, int k, int &x, int &y) {
    if (!root) x = y = 0;
    else {
        if (tr.tag) pushdown(root);
        if (k <= lson.sz) y = root, split_sz(tr.lc, k, x, tr.lc); 
        else x = root, split_sz(tr.rc, k - lson.sz - 1, tr.rc, y);
        update(root);
    }
}
void reverse(int l, int r) {
    int x, y, z;
    split_sz(Root, l - 1, x, y);
    split_sz(y, r - l + 1, y, z);
    t[y].tag ^= 1;
    Root = merge(merge(x, y), z);
}
void ldr(int root) {
    if (!root) return;
    if (tr.tag) pushdown(root);
    ldr(tr.lc);
    printf("%d ", tr.val);
    ldr(tr.rc);
}
#undef tr
#undef lson
#undef rson
} // namespace fhq
int case_Test() {
    int n, m;
    scanf("%d%d", &n, &m);
    rep(i, 1, n) fhq::Root = fhq::merge(fhq::Root, fhq::newNode(i));
    while (m--) {
        int l, r;
        scanf("%d%d", &l, &r);
        fhq::reverse(l, r);
    }
    fhq::ldr(fhq::Root);
    return 0;
}
int main() {
#ifdef LOCAL
    freopen("/Users/chenjinglong/Desktop/cpp_code/in.in", "r", stdin);
    freopen("/Users/chenjinglong/Desktop/cpp_code/out.out", "w", stdout);
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
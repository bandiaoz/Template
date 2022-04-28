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
const int maxn = 2e5 + 5;

namespace fhq {
#define tr t[root]
#define lson t[tr.lc]
#define rson t[tr.rc]
mt19937 rnd(233);
struct node {
    int lc, rc, val, key, sz;
} t[maxn];
int cnt, Root;
// 重新计算以 root 为根的子树大小
inline void update(int root) { tr.sz = lson.sz + rson.sz + 1; }
// 新建一个权值为val的结点
int newNode(int val) {
    t[++cnt] = {0, 0, val, (int)rnd(), 1};
    return cnt;
}
// 合并成小根堆，参数保证x树的权值严格小于y树的权值
int merge(int x, int y) {
    if (!x || !y) return x + y;
    if (t[x].key < t[y].key) {
        t[x].rc = merge(t[x].rc, y);
        update(x); return x;
    } else {
        t[y].lc = merge(x, t[y].lc);
        update(y); return y;
    }
}
// 在以 root 为根的子树内树按值分裂，x树的权值小于等于val，y树的权值大于val
void split_val(int root, int val, int &x, int &y) {
    if (!root) x = y = 0;
    else {
        if (val < tr.val) y = root, split_val(tr.lc, val, x, tr.lc);
        else x = root, split_val(tr.rc, val, tr.rc, y);
        update(root);
    }
}
// 在以 root 为根的子树内树按值分裂，x树的大小等于k
void split_sz(int root, int k, int &x, int &y) {
    if (!root) x = y = 0;
    else {
        if (k <= lson.sz) y = root, split_sz(tr.lc, k, x, tr.lc); 
        else x = root, split_sz(tr.rc, k - lson.sz - 1, tr.rc, y);
        update(root);
    }
}
// 以 root 为根的子树中，名次为 rnk 的权值
int Getnum(int root, int rnk) {
    if (rnk == lson.sz + 1) return tr.val;
    if (rnk <= lson.sz) return Getnum(tr.lc, rnk);
    return Getnum(tr.rc, rnk - lson.sz - 1);
}
int x, y, z;
// 在以 Root 为根的子树内添加权值为 val 节点
inline void insert(int val) {
    split_val(Root, val, x, y);
    Root = merge(merge(x, newNode(val)), y);
}
// 从以 Root 为根子树移除权值为 val 节点
inline void del(int val) {
    split_val(Root, val, x, z);
    split_val(x, val - 1, x, y);
    y = merge(t[y].lc, t[y].rc);
    Root = merge(merge(x, y), z);
}
// 以 Root 为根的子树中，权值为 val 的名次
inline int getrnk(int val) {
    split_val(Root, val - 1, x, y);
    int rnk = t[x].sz + 1;
    Root = merge(x, y);
    return rnk;
}
// 以 Root 为根的子树中，名次为 rnk 的权值
inline int getnum(int rnk) { return Getnum(Root, rnk); }
// 以 Root 为根的子树中，查val的前驱
inline int getpre(int val) {
    split_val(Root, val - 1, x, y);
    int ans = Getnum(x, t[x].sz);
    Root = merge(x, y);
    return ans;
}
// 以 Root 为根的子树中，查val的后继
inline int getnex(int val) {
    split_val(Root, val, x, y);
    int ans = Getnum(y, 1);
    Root = merge(x, y);
    return ans;
}
#undef tr
#undef lson
#undef rson
} // namespace fhq
int case_Test() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int op, x;
        scanf("%d%d", &op, &x);
        if (op == 1) fhq::insert(x);
        if (op == 2) fhq::del(x);
        if (op == 3) printf("%d\n", fhq::getrnk(x));
        if (op == 4) printf("%d\n", fhq::getnum(x));
        if (op == 5) printf("%d\n", fhq::getpre(x)); 
        if (op == 6) printf("%d\n", fhq::getnex(x));
    }
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
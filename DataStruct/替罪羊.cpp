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
namespace tzy {
#define tr t[root]
#define lson t[tr.lc]
#define rson t[tr.rc]
const double alpha = 0.75;
int cnt, Root;
struct node {
    int val, lc, rc;
    int num, sz, csz, dsz;
} t[maxn];
// 重新计算以 root 为根的子树大小
void Calc(int root) {
    tr.sz = lson.sz + rson.sz + 1;
    tr.csz = lson.csz + rson.csz + tr.num;
    tr.dsz = lson.dsz + rson.dsz + (tr.num != 0);
}
// 判断节点 root 是否需要重构
inline bool CanRbu(int root) {
    return tr.num && (max(lson.sz, rson.sz) >= alpha * tr.sz || tr.dsz <= alpha * tr.sz);
}
int ldr[maxn];
// 中序遍历展开以 root 节点为根子树
void getLdr(int &len, int root) {
    if (!root) return;
    getLdr(len, tr.lc);
    if (tr.num) ldr[len++] = root;
    getLdr(len, tr.rc);
}
// 将 ldr[] 数组内 [l, r) 区间重建成树，返回根节点
int lift(int l, int r) {
    int mid = (l + r) >> 1, R = ldr[mid];
    if (l >= r) return 0;
    t[R].lc = lift(l, mid);
    t[R].rc = lift(mid + 1, r);
    Calc(R);
    return R;
}
// 重构节点 root 的全过程
void rebuild(int &root) {
    if (!CanRbu(root)) return;
    int len = 0;
    getLdr(len, root);
    root = lift(0, len);
}
// 在以 root 为根的子树内添加权值为 val 节点
void Insert(int &root, int val) {
    if (!root) {
        root = ++cnt;
        if (!Root) Root = 1;
        tr.val = val, tr.lc = tr.rc = 0;
        tr.num = tr.sz = tr.csz = tr.dsz = 1;
    } else {
        if (val == tr.val) tr.num++;
        else if (val < tr.val) Insert(tr.lc, val);
        else Insert(tr.rc, val); 
        Calc(root), rebuild(root);
    }
}
// 从以 root 为根子树移除权值为 val 节点
void Del(int &root, int val) {
    if (!root) return;
    if (tr.val == val) {
        if (tr.num) tr.num--;
    } else {
        if (val < tr.val) Del(tr.lc, val);
        else Del(tr.rc, val);
    }
    Calc(root), rebuild(root);
}
// 在以 root 为根子树中，大于 val 的最小数的名次
int MyUprBd(int root, int val) {
    if (!root) return 1;
    if (val == tr.val && tr.num) return lson.csz + 1 + tr.num;
    if (val < tr.val) return MyUprBd(tr.lc, val);
    return lson.csz + tr.num + MyUprBd(tr.rc, val);
}
// 权值严格小于某值的最大名次
int MyUprGrt(int root, int val) {
    if (!root) return 0;
    if (val == tr.val) return lson.csz;
    if (val < tr.val) return MyUprGrt(tr.lc, val);
    return lson.csz + tr.num + MyUprGrt(tr.rc, val);
}
// 以 root 为根的子树中，名次为 rnk 的权值
int Getnum(int root, int rnk) {
    if (!root) return 0;
    if (lson.csz < rnk && rnk <= lson.csz + tr.num) return tr.val;
    if (lson.csz >= rnk) return Getnum(tr.lc, rnk);
    return Getnum(tr.rc, rnk - lson.csz - tr.num);
}
inline void insert(int val) { Insert(Root, val); }
inline void del(int val) { Del(Root, val); }
inline int getnum(int rnk) { return Getnum(Root, rnk); }
inline int getrnk(int val) { return MyUprGrt(Root, val) + 1; }
inline int lowerRnk(int val) { return MyUprGrt(Root, val); }
inline int upperRnk(int val) { return MyUprBd(Root, val); }
inline int getpre(int val) { return getnum(lowerRnk(val)); }
inline int getnex(int val) { return getnum(upperRnk(val)); }
#undef tr
#undef lson
#undef rson
} // namespace tzy
int case_Test() {
    int _; scanf("%d", &_);
    while (_--) {
        int op, x;
        scanf("%d%d", &op, &x);
        if (op == 1) tzy::insert(x);
        if (op == 2) tzy::del(x);
        if (op == 3) printf("%d\n", tzy::getrnk(x));
        if (op == 4) printf("%d\n", tzy::getnum(x));
        if (op == 5) printf("%d\n", tzy::getpre(x));
        if (op == 6) printf("%d\n", tzy::getnex(x));
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
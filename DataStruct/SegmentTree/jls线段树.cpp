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
//HDU - 5306 Gorgeous Sequence(jls线段树)
const int maxn = 1e6 + 5;
int n, m, a[maxn];
struct segtree {
    int l, r, maxx, semax, cmax;
    ll sum;
} t[maxn << 2];
inline void pushup(int root) {
    tr.sum = lson.sum + rson.sum;
    tr.maxx = max(lson.maxx, rson.maxx);
    tr.semax = max(lson.semax, rson.semax);
    tr.cmax = 0;
    if (lson.maxx != rson.maxx) tr.semax = max(tr.semax, min(lson.maxx, rson.maxx));
    if (tr.maxx == lson.maxx) tr.cmax += lson.cmax;
    if (tr.maxx == rson.maxx) tr.cmax += rson.cmax;
}
void build(int root, int l, int r) {
    tr.l = l, tr.r = r;
    if (l == r) {
        tr.sum = tr.maxx = a[l];
        tr.cmax = 1;
        tr.semax = -1;
        return;
    }
    int mid = (l + r) >> 1;
    build(root << 1, l, mid);
    build(root << 1 | 1, mid + 1, r);
    pushup(root);
}
inline void dec_tag(int root, int x) {  //更新maxx和sum
    if (x >= tr.maxx) return;
    tr.sum += 1LL * (x - tr.maxx) * tr.cmax;
    tr.maxx = x;
}
inline void spread(int root) {
    dec_tag(root << 1, tr.maxx);
    dec_tag(root << 1 | 1, tr.maxx);
}
void update(int root, int l, int r, int x) {
    if (x >= tr.maxx) return;                      //不会产生影响，退出
    if (l <= tr.l && tr.r <= r && x > tr.semax) {  //只影响最大值，更新，打标记退出
        dec_tag(root, x);
        return;
    }
    //无法更新，递归搜索
    spread(root);
    int mid = (tr.l + tr.r) >> 1;
    if (l <= mid) update(root << 1, l, r, x);
    if (r > mid) update(root << 1 | 1, l, r, x);
    pushup(root);
}
int qmax(int root, int l, int r) {
    if (l <= tr.l && tr.r <= r) return tr.maxx;
    spread(root);
    int mid = (tr.l + tr.r) >> 1;
    int maxx = 0;
    if (l <= mid) maxx = max(maxx, qmax(root << 1, l, r));
    if (r > mid) maxx = max(maxx, qmax(root << 1 | 1, l, r));
    return maxx;
}
ll qsum(int root, int l, int r) {
    if (l <= tr.l && tr.r <= r) return tr.sum;
    spread(root);
    ll ans = 0;
    int mid = (tr.l + tr.r) >> 1;
    if (l <= mid) ans += qsum(root << 1, l, r);
    if (r > mid) ans += qsum(root << 1 | 1, l, r);
    return ans;
}
int main() {
    int T;
    R(T);
    while (T--) {
        R(n, m);
        rep(i, 1, n) R(a[i]);
        build(1, 1, n);
        while (m--) {
            int op, l, r, x;
            R(op, l, r);
            if (op == 0) R(x), update(1, l, r, x);  //区间 a[i]=min(a[i],x)
            if (op == 1) W(qmax(1, l, r));
            if (op == 2) W(qsum(1, l, r));
        }
    }
}
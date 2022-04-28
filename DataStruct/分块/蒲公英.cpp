#pragma region
#include <algorithm>
#include <cmath>
#include <cstdio>
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
bool R(T &h, U &... tmp) { return R(h) && R(tmp...); }
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
//https://www.acwing.com/problem/content/251/
//区间众数 强制在线分块做法
const int maxn = 1e5 + 5;
const int maxb = 1005;
int n, q, B, a[maxn], b[maxn];
int cnt[maxn], ans[maxb][maxb];  //候选答案
vector<int> pos[maxn];
inline int getcnt(const int &l, const int &r, const int &x) {
    return upper_bound(pos[x].begin(), pos[x].end(), r) - lower_bound(pos[x].begin(), pos[x].end(), l);
}
int query(int l, int r) {
    int idl = l / B, idr = r / B;
    int num = 0x3f3f3f3f, cnum = 0;
    if (idl == idr) {
        rep(i, l, r) {
            int tmp = getcnt(l, r, a[i]);
            if (tmp > cnum || (tmp == cnum && a[i] < num)) num = a[i], cnum = tmp;
        }
    } else {
        num = ans[idl + 1][idr - 1], cnum = getcnt(l, r, num);
        rep(i, l, (idl + 1) * B - 1) {
            int tmp = getcnt(l, r, a[i]);
            if (tmp > cnum || (tmp == cnum && a[i] < num)) num = a[i], cnum = tmp;
        }
        rep(i, idr * B, r) {
            int tmp = getcnt(l, r, a[i]);
            if (tmp > cnum || (tmp == cnum && a[i] < num)) num = a[i], cnum = tmp;
        }
    }
    return b[num];
}
int main() {
    R(n, q);
    if (n >= 1000)
        B = max(1, (int)(n / sqrt(q * log2(n))));
    else
        B = sqrt(n);
    rep(i, 1, n) R(a[i]);
    memcpy(b, a, sizeof(b));
    sort(b + 1, b + 1 + n);
    int len = unique(b + 1, b + 1 + n) - b - 1;
    rep(i, 1, n) {
        a[i] = lower_bound(b + 1, b + 1 + len, a[i]) - b;  //b[a[i]]
        pos[a[i]].push_back(i);
    }

    for (int l = B; l <= n; l += B) {
        memset(cnt, 0, sizeof(cnt));
        int tmp = 0;
        rep(i, l, n) {
            ++cnt[a[i]];
            if (cnt[a[i]] > cnt[tmp] || (cnt[a[i]] == cnt[tmp] && a[i] < tmp)) tmp = a[i];
            if (i / B * B + B - 1 == i || i == n) ans[l / B][i / B] = tmp;
        }
    }

    int last = 0;
    while (q--) {
        int l, r;
        R(l, r);
        l = (l + last - 1) % n + 1;
        r = (r + last - 1) % n + 1;
        if (l > r) swap(l, r);
        last = query(l, r);
        W(last);
    }
}
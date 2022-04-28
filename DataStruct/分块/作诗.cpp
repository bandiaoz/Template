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
//https://www.luogu.com.cn/problem/P4135
//区间偶数次的数 强制在线
const int maxn = 1e5 + 5;
const int maxb = 320;
int n, c, B, q;
int a[maxn];
int s[maxb][maxn], ans[maxb][maxb];
int cnt[maxn];
bool vis[maxn];
int query(int l, int r) {
    int idl = l / B, idr = r / B;
    int res = 0;
    // rep(i, 0, c) cnt[i] = 0, vis[i] = 0;
    if (idl == idr) {
        rep(i, l, r) {
            cnt[a[i]]++;
            if (!(cnt[a[i]] & 1))
                res++;
            else if (cnt[a[i]] != 1)
                res--;
        }
        rep(i, l, r) cnt[a[i]]--;
    } else {
        res = ans[idl + 1][idr - 1];
        rep(i, l, B * (idl + 1) - 1) cnt[a[i]]++;
        rep(i, idr * B, r) cnt[a[i]]++;
        rep(i, l, B * (idl + 1) - 1) {
            if (vis[a[i]]) continue;
            vis[a[i]] = 1;
            int x = cnt[a[i]], y = s[idr - 1][a[i]] - s[idl][a[i]];
            if ((x & 1) && (y & 1))
                res++;
            else if ((x & 1) && (!(y & 1)) && y)
                res--;
            else if (x && (!(x & 1)) && !y)
                res++;
        }
        rep(i, idr * B, r) {
            if (vis[a[i]]) continue;
            vis[a[i]] = 1;
            int x = cnt[a[i]], y = s[idr - 1][a[i]] - s[idl][a[i]];
            if ((x & 1) && (y & 1))
                res++;
            else if ((x & 1) && (!(y & 1)) && y)
                res--;
            else if (x && (!(x & 1)) && !y)
                res++;
        }
        rep(i, l, B * (idl + 1) - 1) cnt[a[i]]--, vis[a[i]] = 0;
        rep(i, idr * B, r) cnt[a[i]]--, vis[a[i]] = 0;
    }
    return res;
}
int main() {
    R(n, c, q);
    B = sqrt(n);
    rep(i, 1, n) {
        R(a[i]);
        if (i % B == 0) rep(j, 0, c) s[i / B][j] = s[i / B - 1][j];
        s[i / B][a[i]]++;
    }
    for (int l = B; l <= n; l += B) {
        int tmp = 0;
        rep(r, l, n) {
            cnt[a[r]]++;
            if (!(cnt[a[r]] & 1))
                tmp++;
            else if (cnt[a[r]] != 1)
                tmp--;
            if (r % B == B - 1) ans[l / B][r / B] = tmp;
        }
        rep(r, l, n) cnt[a[r]]--;
    }

    int last = 0;
    while (q--) {
        int l, r;
        R(l, r);
        l = (l + last) % n + 1;
        r = (r + last) % n + 1;
        if (l > r) swap(l, r);
        last = query(l, r);
        W(last);
    }
}
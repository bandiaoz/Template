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
// 区间众数 n*sqrt(n)
const int maxn = 1e5 + 5;
const int maxb = 1005;
int n, B, a[maxn], b[maxn];
int ans[maxb][maxb], s[maxb][maxn];
int cnt[maxn];
int query(int l, int r) {
    int idl = l / B, idr = r / B;
    memset(cnt, 0, sizeof(cnt));
    int num = ((1LL << 31) - 1), cnum = 0;
    if (idl == idr) {
        rep(i, l, r) {
            ++cnt[a[i]];
            if (cnt[a[i]] > cnum || (cnt[a[i]] == cnum && a[i] < num)) num = a[i], cnum = cnt[a[i]];
        }
    } else {
        rep(i, l, (idl + 1) * B - 1)++ cnt[a[i]];
        rep(i, idr * B, r)++ cnt[a[i]];
        num = ans[idl + 1][idr - 1];
        cnum = s[idr - 1][num] - s[idl][num] + cnt[num];
        rep(i, l, (idl + 1) * B - 1) {
            int ccnt = cnt[a[i]] + s[idr - 1][a[i]] - s[idl][a[i]];
            if (ccnt > cnum || (ccnt == cnum && a[i] < num)) num = a[i], cnum = ccnt;
        }
        rep(i, idr * B, r) {
            int ccnt = cnt[a[i]] + s[idr - 1][a[i]] - s[idl][a[i]];
            if (ccnt > cnum || (ccnt == cnum && a[i] < num)) num = a[i], cnum = ccnt;
        }
    }
    return b[num];
}
void init() {
    for (int l = B; l <= n; l += B) {
        memset(cnt, 0, sizeof(cnt));
        int tmp = 0;
        for (int r = l; r <= n; ++r) {
            ++cnt[a[r]];
            if (cnt[a[r]] > cnt[tmp] || (cnt[a[r]] == cnt[tmp] && a[r] < tmp)) tmp = a[r];
            if (r % B == B - 1) ans[l / B][r / B] = tmp;
        }
    }
    memset(cnt, 0, sizeof(cnt));
    rep(i, 1, n) {
        if (i % B == 0) rep(j, 1, n) s[i / B][j] = s[i / B - 1][j];
        ++s[i / B][a[i]];
    }
}
int main() {
    R(n);
    B = sqrt(n);
    rep(i, 1, n) R(a[i]);
    memcpy(b, a, sizeof(b));
    sort(b + 1, b + 1 + n);
    rep(i, 1, n) a[i] = lower_bound(b + 1, b + 1 + n, a[i]) - b;  //b[a[i]]
    init();
    rep(i, 1, n) {
        int l, r;
        R(l, r);
        W(query(l, r));
    }
}
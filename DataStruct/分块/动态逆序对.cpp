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
//分块实现
const int maxn = 2e5 + 5;
int n, q, B;
int a[maxn];
vector<int> v[maxn];
ll work(int l, int r) {
    if (l == r) return 0;
    int x = a[l], y = a[r];
    int idx = l / B, idy = r / B;
    int minn = min(x, y), maxx = max(x, y);
    int f = 1;
    if (x > y) f = -1;
    ll ans = 1;
    if (idx == idy) {
        rep(i, l + 1, r - 1) if (a[i] >= minn && a[i] <= maxx) ans += 2;
    } else {
        rep(i, l + 1, (idx + 1) * B - 1) if (a[i] >= minn && a[i] <= maxx) ans += 2;
        rep(id, idx + 1, idy - 1) ans += 2 * (upper_bound(v[id].begin(), v[id].end(), maxx) - lower_bound(v[id].begin(), v[id].end(), minn));
        rep(i, idy * B, r - 1) if (a[i] >= minn && a[i] <= maxx) ans += 2;
    }
    int pos = lower_bound(v[idx].begin(), v[idx].end(), x) - v[idx].begin();
    v[idx][pos] = y;
    sort(v[idx].begin(), v[idx].end());
    pos = lower_bound(v[idy].begin(), v[idy].end(), y) - v[idy].begin();
    v[idy][pos] = x;
    sort(v[idy].begin(), v[idy].end());
    swap(a[l], a[r]);
    return f * ans;
}
int main() {
    R(n, q);
    B = sqrt(n);
    rep(i, 1, n) {
        a[i] = i;
        v[i / B].push_back(i);
    }
    ll ans = 0;
    while (q--) {
        int l, r;
        R(l, r);
        if (l > r) swap(l, r);
        ans += work(l, r);
        W(ans);
    }
}
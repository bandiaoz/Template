#pragma region
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;
typedef long long ll;
#define tr t[root]
#define lson t[root << 1]
#define rson t[root << 1 | 1]
#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)
#pragma endregion
const int maxn = 1e5 + 5;
int n, k;
ll lc[maxn], rc[maxn];
ll L[maxn], R[maxn], U[maxn], D[maxn];
priority_queue<ll, vector<ll>, greater<ll> > q;
struct node {
    ll x, y;
} a[maxn];
ll sq(ll x) { return x * x; }
ll dis(int i, int j) { return sq(a[i].x - a[j].x) + sq(a[i].y - a[j].y); }
int cmpx;
void maintain(int id) {
    L[id] = R[id] = a[id].x;
    D[id] = U[id] = a[id].y;
    if (lc[id])
        L[id] = min(L[id], L[lc[id]]), R[id] = max(R[id], R[lc[id]]),
        D[id] = min(D[id], D[lc[id]]), U[id] = max(U[id], U[lc[id]]);
    if (rc[id])
        L[id] = min(L[id], L[rc[id]]), R[id] = max(R[id], R[rc[id]]),
        D[id] = min(D[id], D[rc[id]]), U[id] = max(U[id], U[rc[id]]);
}
int build(int l, int r) {
    if (l > r) return 0;
    int mid = (l + r) >> 1;
    double avx = 0, avy = 0, vax = 0, vay = 0;
    rep(i, l, r) avx += a[i].x, avy += a[i].y;
    avx /= r - l + 1, avy /= r - l + 1;
    rep(i, l, r) vax += sq(avx - a[i].x), vay += sq(avy - a[i].y);
    cmpx = (vax > vay);
    nth_element(a + l, a + mid, a + r + 1, [](const node &A, const node &B) { return cmpx ? A.x < B.x : A.y < B.y; });
    lc[mid] = build(l, mid - 1), rc[mid] = build(mid + 1, r);
    maintain(mid);
    return mid;
}
ll f(ll p, ll id) {
    return max(sq(a[p].x - L[id]), sq(a[p].x - R[id])) +
           max(sq(a[p].y - D[id]), sq(a[p].y - U[id]));
}
void query(int l, int r, int x) {
    if (l > r) return;
    int mid = (l + r) >> 1;
    ll tmp = dis(x, mid);
    if (tmp > q.top()) q.pop(), q.push(tmp);
    ll dl = f(x, lc[mid]), dr = f(x, rc[mid]);
    if (dl > q.top() && dr > q.top()) {
        if (dl > dr) {
            query(l, mid - 1, x);
            if (dr > q.top()) query(mid + 1, r, x);
        } else {
            query(mid + 1, r, x);
            if (dl > q.top()) query(l, mid - 1, x);
        }
    } else {
        if (dl > q.top()) query(l, mid - 1, x);
        if (dr > q.top()) query(mid + 1, r, x);
    }
}
int main() {
    scanf("%d%d", &n, &k);
    k *= 2;
    rep(i, 1, k) q.push(0);
    rep(i, 1, n) scanf("%lld%lld", &a[i].x, &a[i].y);
    build(1, n);
    rep(i, 1, n) query(1, n, i);
    printf("%lld\n", q.top());
}
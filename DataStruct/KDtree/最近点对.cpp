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
const int maxn = 2e5 + 5;
int n, lc[maxn], rc[maxn];
double L[maxn], R[maxn], U[maxn], D[maxn];
double ans = 2e18;
struct node {
    double x, y;
} a[maxn];
double sq(double x) {
    return x * x;
}
double dis(int i, int j) {
    return sq(a[i].x - a[j].x) + sq(a[i].y - a[j].y);
}
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
    if (l >= r) return 0;
    int mid = (l + r) >> 1;
    double avx = 0, avy = 0, vax = 0, vay = 0;
    rep(i, l, r) avx += a[i].x, avy += a[i].y;
    avx /= r - l + 1, avy /= r - l + 1;
    rep(i, l, r) vax += sq(a[i].x - avx), vay += sq(a[i].y - vay);
    cmpx = (vax > vay);
    nth_element(a + l, a + mid, a + r + 1, [](const node& A, const node& B) { return cmpx ? A.x < B.x : A.y < B.y; });
    lc[mid] = build(l, mid - 1), rc[mid] = build(mid + 1, r);
    maintain(mid);
    return mid;
}
double f(int p, int id) {
    double ans = 0;
    if (a[p].x < L[id]) ans += sq(L[id] - a[p].x);
    if (a[p].x > R[id]) ans += sq(a[p].x - R[id]);
    if (a[p].y < D[id]) ans += sq(D[id] - a[p].y);
    if (a[p].y > U[id]) ans += sq(a[p].y - U[id]);
    return ans;
}
void query(int l, int r, int p) {
    if (l > r) return;
    int mid = (l + r) >> 1;
    if (p != mid) ans = min(ans, dis(p, mid));
    if (l == r) return;
    double dl = f(p, lc[mid]), dr = f(p, rc[mid]);
    if (dl < ans && dr < ans) {
        if (dl < dr) {
            query(l, mid - 1, p);
            if (dr < ans) query(mid + 1, r, p);
        } else {
            query(mid + 1, r, p);
            if (dl < ans) query(l, mid - 1, p);
        }
    } else {
        if (dl < ans) query(l, mid - 1, p);
        if (dr < ans) query(mid + 1, r, p);
    }
}
int main() {
    scanf("%d", &n);
    rep(i, 1, n) scanf("%lf%lf", &a[i].x, &a[i].y);
    build(1, n);
    rep(i, 1, n) query(1, n, i);
    printf("%.4lf\n", ans);
}
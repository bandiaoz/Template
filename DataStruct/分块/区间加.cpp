#pragma region
#include <cmath>
#include <cstdio>
using namespace std;
typedef long long ll;
#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)
#pragma endregion
const int maxn = 1e5 + 5;
ll a[maxn], B;
ll base[maxn], res[maxn];
ll query(int l, int r) {
    int idl = l / B, idr = r / B;
    ll ans = 0;
    if (idl == idr) {
        rep(i, l, r) ans += a[i] + base[idl];
    } else {
        rep(i, l, (idl + 1) * B - 1) ans += a[i] + base[idl];
        rep(id, idl + 1, idr - 1) ans += res[id];
        rep(i, idr * B, r) ans += a[i] + base[idr];
    }
    return ans;
}
void update(int l, int r, ll x) {
    int idl = l / B, idr = r / B;
    if (idl == idr) {
        rep(i, l, r) a[i] += x, res[idl] += x;
    } else {
        rep(i, l, (idl + 1) * B - 1) a[i] += x, res[idl] += x;
        rep(id, idl + 1, idr - 1) base[id] += x, res[id] += x * B;
        rep(i, idr * B, r) a[i] += x, res[idr] += x;
    }
}
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    B = sqrt(n);
    rep(i, 1, n) {
        scanf("%lld", &a[i]);
        res[i / B] += a[i];
    }
    while (q--) {
        ll l, r, x;
        char op[10];
        scanf("%s", op);
        if (op[0] == 'Q') {
            scanf("%lld%lld", &l, &r);
            printf("%lld\n", query(l, r));
        } else {
            scanf("%lld%lld%lld", &l, &r, &x);
            update(l, r, x);
        }
    }
}
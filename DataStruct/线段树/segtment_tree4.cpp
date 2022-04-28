#pragma region
#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>
typedef long long ll;
#define tr t[root]
#define lson t[root << 1]
#define rson t[root << 1 | 1]
#define rep(i, a, n) for (long long i = a; i <= n; i++)
#define IO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
using namespace std;
/*
 *                                                     __----~~~~~~~~~~~------___
 *                                    .  .   ~~//====......          __--~ ~~
 *                    -.            \_|//     |||\\  ~~~~~~::::... /~
 *                 ___-==_       _-~o~  \/    |||  \\            _/~~-
 *         __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\        _/~
 *     _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /
 *   .~       .~       |   \\ -_    /  /-   /   ||      \   /
 *  /  ____  /         |     \\ ~-_/  /|- _/   .||       \ /
 *  |~~    ~~|--~~~~--_ \     ~==-/   | \~--===~~        .\
 *           '         ~-|      /|    |-~\~~       __--~~
 *                       |-~~-_/ |    |   ~\_   _-~            /\
 *                            /  \     \__   \/~                \__
 *                        _--~ _/ | .-~~____--~-/                  ~~==.
 *                       ((->/~   '.|||' -_|    ~~-/ ,              . _||
 *                                  -_     ~\      ~~---l__i__i__i--~~_/
 *                                  _-~-__   ~)  \--______________--~~
 *                                //.-~~~-~_--~- |-------~~~~~~~~
 *                                       //.-~~~--\
 *                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#pragma endregion
const int maxn = 5e5 + 500;
ll a[maxn], mod, n, m;
struct segtree {
    ll l, r, val;
    ll mul, add;
} t[maxn << 2];
void build(int root, int l, int r) {
    tr.l = l, tr.r = r, tr.add = 0, tr.mul = 1;
    if (l == r) {
        tr.val = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(root << 1, l, mid);
    build(root << 1 | 1, mid + 1, r);
    tr.val = (lson.val + rson.val) % mod;
}
void spread(int root) {
    lson.val = (lson.val * tr.mul + (lson.r - lson.l + 1) * tr.add) % mod;
    rson.val = (rson.val * tr.mul + (rson.r - rson.l + 1) * tr.add) % mod;
    lson.add = (tr.add + tr.mul * lson.add) % mod;
    rson.add = (tr.add + tr.mul * rson.add) % mod;
    lson.mul = lson.mul * tr.mul % mod;
    rson.mul = rson.mul * tr.mul % mod;
    tr.add = 0, tr.mul = 1;
}
void updateadd(int root, int l, int r, ll x) {
    if (l <= tr.l && tr.r <= r) {
        tr.val = (tr.val + x * (tr.r - tr.l + 1)) % mod;
        tr.add = (tr.add + x) % mod;
        return;
    }
    spread(root);
    int mid = (tr.l + tr.r) >> 1;
    if (l <= mid) updateadd(root << 1, l, r, x);
    if (r > mid) updateadd(root << 1 | 1, l, r, x);
    tr.val = (lson.val + rson.val) % mod;
}
void updatemul(int root, int l, int r, ll x) {
    if (l <= tr.l && tr.r <= r) {
        tr.val = tr.val * x % mod;
        tr.add = tr.add * x % mod;
        tr.mul = tr.mul * x % mod;
        return;
    }
    spread(root);
    int mid = (tr.l + tr.r) >> 1;
    if (l <= mid) updatemul(root << 1, l, r, x);
    if (r > mid) updatemul(root << 1 | 1, l, r, x);
    tr.val = (lson.val + rson.val) % mod;
}
ll query(ll root, ll l, ll r) {
    if (l <= tr.l && tr.r <= r) return tr.val % mod;
    spread(root);
    ll mid = (tr.l + tr.r) >> 1;
    ll ans = 0;
    if (l <= mid) ans = (ans + query(root << 1, l, r)) % mod;
    if (r > mid) ans = (ans + query(root << 1 | 1, l, r)) % mod;
    return ans;
}
int main() {
    IO;
    cin >> n >> m >> mod;
    rep(i, 1, n) cin >> a[i];
    build(1, 1, n);
    while (m--) {
        ll op, l, r, x;
        cin >> op >> l >> r;
        if (op == 1) {
            cin >> x;
            updatemul(1, l, r, x);
        } else if (op == 2) {
            cin >> x;
            updateadd(1, l, r, x);
        } else
            cout << query(1, l, r) << endl;
    }
}
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
#define rep(i, a, n) for (long long i = a; i <= n; ++i)
#define per(i, a, n) for (long long i = n; i >= a; --i)
#define IO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
using namespace std;
#pragma endregion
const int maxn = 2005;
struct node {
    ll x, y, w;
    bool operator<(const node& A) {
        return x < A.x;
    }
} a[maxn];
ll b[maxn];
struct segtree {
    int l, r;
    ll lans, rans, ans, val;
} t[maxn << 2];
void spread(int root) {
    tr.val = lson.val + rson.val;
    tr.lans = max(lson.lans, lson.val + rson.lans);
    tr.rans = max(rson.rans, rson.val + lson.rans);
    tr.ans = max({lson.ans, rson.ans, lson.rans + rson.lans});
}
void build(int root, int l, int r) {
    tr.l = l, tr.r = r;
    tr.val = tr.lans = tr.rans = tr.ans = 0;
    if (l == r) return;
    int mid = (l + r) >> 1;
    build(root << 1, l, mid);
    build(root << 1 | 1, mid + 1, r);
}
void update(int root, int pos, ll x) {
    if (tr.l == tr.r) {
        tr.val = tr.lans = tr.rans = tr.ans = tr.ans + x;
        return;
    }
    int mid = (tr.l + tr.r) >> 1;
    if (pos <= mid) update(root << 1, pos, x);
    if (pos > mid) update(root << 1 | 1, pos, x);
    spread(root);
}
int main() {
    IO;
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        rep(i, 1, n) {
            cin >> a[i].x >> a[i].y >> a[i].w;
            b[i] = a[i].y;
        }
        sort(b + 1, b + 1 + n);
        int len = unique(b + 1, b + 1 + n) - b - 1;
        rep(i, 1, n) a[i].y = lower_bound(b + 1, b + 1 + len, a[i].y) - b;
        sort(a + 1, a + 1 + n);
        ll ans = 0;
        rep(i, 1, n) {
            if (i >= 2 && a[i].x == a[i - 1].x) continue;
            build(1, 1, len);
            rep(j, i, n) {
                if (j != i && a[j].x != a[j - 1].x) ans = max(ans, t[1].ans);
                update(1, a[j].y, a[j].w);
            }
            ans = max(ans, t[1].ans);
        }
        cout << ans << endl;
    }
}
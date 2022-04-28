#pragma region
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>
typedef long long ll;
#define rep(i, a, n) for (ll i = a; i <= n; ++i)
#define per(i, a, n) for (ll i = n; i >= a; --i)
#define IO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
using namespace std;
#pragma endregion
const int maxn = 5e5 + 5;
ll b[maxn][32], pos[maxn][32];     //插入a[i]后[1,i]直间的线性基第j位的数字是由原数组中哪一个数字得到的(最右)
inline bool insert(int r, ll x) {  //维护区间[1,r]内的线性基
    for (int i = 0; i <= 31; i++) 
        b[r][i] = b[r - 1][i], pos[r][i] = pos[r - 1][i];
    ll tmp = r;
    for (int i = 31; i >= 0; i--) {
        if (x & (1LL << i)) {
            if (b[r][i]) {
                if (pos[r][i] < tmp) {
                    swap(pos[r][i], tmp);
                    swap(b[r][i], x);
                }
                x ^= b[r][i];
            } else {
                b[r][i] = x;
                pos[r][i] = tmp;
                return 1;
            }
        }
    }
    return 0;
}
ll get_max(int l, int r) {  //求区间[l,r]内的最大异或值
    int ans = 0;
    for (int i = 31; i >= 0; i--)
        if (pos[r][i] >= l && (ans ^ b[r][i]) > ans)
            ans ^= b[r][i];
    return ans;
}
int main() {
    IO;
    int T;
    cin >> T;
    while (T--) {
        int n, m, ans = 0;
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            ll x;
            cin >> x;
            insert(i, x);
        }
        for (int i = 0; i < m; i++) {
            ll op, l, r;
            cin >> op;
            if (!op) {
                cin >> l >> r;
                l = (l ^ ans) % n + 1;
                r = (r ^ ans) % n + 1;
                if (l > r) swap(l, r);
                ans = get_max(l, r);
                cout << ans << endl;
            } else {
                ll x;
                cin >> x;
                x ^= ans;
                insert(++n, x);
            }
        }
    }
}
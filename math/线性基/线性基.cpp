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
#define rep(i, a, n) for (long long i = a; i <= n; ++i)
#define per(i, a, n) for (long long i = n; i >= a; --i)
#define IO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
using namespace std;
#pragma endregion
const int maxn = 70;
ll b[maxn], p[maxn];  //线性基，重构线性基
int cnt, flag;        //重构线性基的大小,是否异或为0
inline void insert2(ll x) {
    for (int i = 62; i >= 0; --i){
        if (x & (1LL << i)) {
            if (b[i])
                x ^= b[i];
            else {
                b[i] = x;
                return;
            }
        }
    }
    flag = 1;
    return;
}
ll getmax() {
    ll ans = 0;
    for (int i = 62; i >= 0; --i)
        if ((ans ^ b[i]) > ans) ans ^= b[i];
    return ans;
}
ll getmin() {
    if (flag) return 0;
    for (int i = 0; i <= 62; ++i)
        if (b[i]) return b[i];
    return 0;
}
inline void rebuild() {
    for (int i = 62; i >= 1; --i) {
        if (b[i])
            for (int j = i - 1; j >= 0; --j)
                if (b[i] & (1LL << j)) b[i] ^= b[j];
    }
    //异或p[i]对名次的贡献为1<<i
    for (int i = 0; i <= 62; ++i)
        if (b[i]) p[cnt++] = b[i];
}
ll kth(ll k) {
    if (flag) --k;
    if (!k) return 0;
    ll ans = 0;
    if (k >= (1LL << cnt)) return -1;
    for (int i = 0; i <= cnt; ++i)
        if (k & (1LL << i)) ans ^= p[i];
    return ans;
}
int main() {
    IO;
    int T;
    cin >> T;
    rep(cas, 1, T) {
        cout << "Case #" << cas << ":\n";
        memset(b, 0, sizeof(b));
        memset(p, 0, sizeof(p));
        cnt = 0, flag = 0;
        int n;
        cin >> n;
        rep(i, 1, n) {
            ll x;
            cin >> x;
            insert2(x);
        }
        rebuild();
        int q;
        cin >> q;
        while (q--) {
            int k;
            cin >> k;
            cout << kth(k) << endl;
        }
    }
}
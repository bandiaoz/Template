#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll Mod = 1e9 + 7;
ll exgcd(ll a, ll b, ll &x, ll &y) {  //扩展欧几里得算法
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll ret = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return ret;
}
ll getInv(int a, int mod) {  //求a在mod下的逆元，不存在逆元返回-1s
    ll x, y;
    ll d = exgcd(a, mod, x, y);
    return d == 1 ? (x % mod + mod) % mod : -1;
}

int main() {
    ll x = getInv(24, Mod);
    int T;
    scanf("%d", &T);
    while (T--) {
        ll n;
        ll ans = 1;
        scanf("%lld", &n);
        for (ll i = n; i <= n + 3; ++i)
            ans = (ans * i) % Mod;
        ans = ans * x % Mod;
        printf("%lld\n", ans);
    }
}
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll mod = 1e9 + 7;
const int maxn = 1e6 + 10;
ll t, n, m, l, r;
ll a[maxn], sum[maxn], pre[maxn], suf[maxn], fac[maxn];
ll ksm(ll x, ll n, ll p)  //计算x的n次幂%p
{
    ll res = 1;
    while (n) {
        if (n & 1)
            res = (res * x) % p;
        x = (x * x) % p;
        n >>= 1;
    }
    return res;
}
/*  cal 函数，用从 0 到 n，一共 n+1 个数，
    传入的 a 数组代表 yi
    插值出原来的 n 次方的多项式
    并返回 原来的多项式在 x 出的取值。
*/
ll cal(ll x, ll *a, ll n) {
    if (x <= n)
        return a[x];
    ll ans = 0;
    pre[0] = x;
    suf[n + 1] = 1;
    for (int i = 1; i <= n; i++)  //求前缀积
        pre[i] = pre[i - 1] * (x - i) % mod;
    for (int i = n; i >= 0; i--)  //求后缀积
        suf[i] = suf[i + 1] * (x - i) % mod;
    for (int i = 0; i <= n; i++) {         //对于每一个去计算
        ll f = fac[n - i] * fac[i] % mod;  //分母
        if ((n - i) % 2 == 1)
            f *= -1;  //正负
        if (i == 0)
            ans = (ans + a[i] * f % mod * 1LL * suf[i + 1] % mod) % mod;  //乘上分子和Y值
        else
            ans = (ans + a[i] * f % mod * pre[i - 1] % mod * suf[i + 1] % mod) % mod;
    }
    return (ans + mod) % mod;  //可能为负值，加mod
}

void init() {
    fac[0] = 1;
    for (int i = 1; i < maxn; i++)  //求N的阶乘
        fac[i] = fac[i - 1] * i % mod;
    for (int i = 0; i < maxn; i++)  //费马小定理求对应逆元
        fac[i] = ksm(fac[i], mod - 2, mod);
}

int main() {
    init();
    ll n, k;
    scanf("%lld %lld", &n, &k);
    ll sum = 0;
    a[0] = 0;
    for (int i = 1; i <= k + 2; i++) {
        sum = (sum + ksm(i, k, mod)) % mod;
        a[i] = sum;
    }
    printf("%lld\n", cal(n, a, k + 1)); 
}

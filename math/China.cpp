#include <bits/stdc++.h>
using namespace std;
#define IO ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
typedef long long ll;
using namespace std;
/**
 *gcd(a,mod)=d;则存在x,y,使d=ax+by
 *extended_euclid(a,mod)=ax+by
 */
ll extended_euclid(ll a, ll mod, ll &x, ll &y)
{ //扩张欧几里的算法
    int d;
    if (mod == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    d = extended_euclid(mod, a % mod, y, x);
    y = y - a / mod * x;
    return d;
}
/**
 *x=mod[i](modw[i]) o<i<len
 *prime[i]>0
 */
ll chinese_remainder(int mod[], int prime[], int len)
{
    ll res, i, d, x, y, n, m;
    res = 0;
    n = 1;
    for (i = 0; i < len; i++)
        n *= prime[i];
    for (i = 0; i < len; i++)
    {
        m = n / prime[i];
        extended_euclid(prime[i], m, x, y);
        res = (res + y * m * mod[i]) % n;
    }
    return (n + res % n) % n;
}

int main()
{
    int len, mod[12], prime[12];
    while (cin >> len)
    {
        for (int i = 0; i < len; i++)
            cin >> prime[i] >> mod[i];
        cout << chinese_remainder(mod, prime, len) << endl;
    }
}
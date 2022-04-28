#include <bits/stdc++.h>
using namespace std;
#define ll long long
const int maxn = 2e5 + 10;
const ll mod = 998244353, g = 3;
int rev[maxn];
ll ksm(ll x, ll n, ll mod) {  //计算x的n次幂%mod
    ll res = 1;
    while (n) {
        if (n & 1) res = (res * x) % mod;
        x = (x * x) % mod;
        n >>= 1;
    }
    return res;
}
void NTT(ll y[], int len, int on) {
    int bit = 0;
    while ((1 << bit) < len) bit++;
    for (int i = 0; i <= len - 1; i++)  //对每一位y处理出递归结束后的位置,然后进行交换
    {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
        if (i < rev[i])
            swap(y[i], y[rev[i]]);  //不加这条if会交换两次（就是没交换）
    }
    //前面和FFT相同
    for (int h = 2; h <= len; h <<= 1)  // h为合并后的区间长度
    {
        ll wn = ksm(g, (mod - 1) / h, mod);        //单位根
        if (on == -1) wn = ksm(wn, mod - 2, mod);  //求逆元
        for (int j = 0; j < len; j += h)           //遍历每个区间的开端
        {
            ll w = 1;
            for (int k = j; k < j + h / 2; k++)  //对小区间进行计算
            {
                ll u = y[k];
                ll t = (w * y[k + h / 2]) % mod;
                y[k] = (u + t) % mod;                //前半区间
                y[k + h / 2] = (u - t + mod) % mod;  //后半区间
                w = (w * wn) % mod;
            }
        }
    }
    if (on == -1) {
        ll t = ksm(len, mod - 2, mod);
        for (int i = 0; i < len; i++) y[i] = (y[i] * t) % mod;
    }
}
char st[maxn], st1[maxn];
ll A[maxn], B[maxn];
int n;
int main() {
    while (~scanf("%s %s", st, st1)) {
        int len = strlen(st), len1 = strlen(st1);
        n = 1;
        while (n < (len << 1) || n < (len1 << 1)) n <<= 1;
        for (int i = 0; i < len; i++) A[len - 1 - i] = st[i] - '0';
        for (int i = len; i <= n; i++) A[i] = 0;
        for (int i = 0; i < len1; i++) B[len1 - 1 - i] = st1[i] - '0';
        for (int i = len1; i <= n; i++) B[i] = 0;
        NTT(A, n, 1);
        NTT(B, n, 1);
        for (int i = 0; i <= n - 1; i++) A[i] = A[i] * B[i] % mod;
        NTT(A, n, -1);
        for (int i = 0; i <= n - 1; i++) {
            A[i + 1] += A[i] / 10;
            A[i] %= 10;
        }
        n--;
        while (A[n] / 10) A[n + 1] += A[n] / 10, A[n++] %= 10;
        while (!A[n] && n > 0) n--;
        for (int i = n; i >= 0; i--) printf("%lld", A[i]);
        printf("\n");
    }
    return 0;
}
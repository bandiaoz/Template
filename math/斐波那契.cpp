#include <bits/stdc++.h>
#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)
#ifdef LOCAL
#include "Print.h"
#define de(...) W('[', #__VA_ARGS__,"] =", __VA_ARGS__)
#else
#define de(...)
#endif
using namespace std;
typedef long long ll;
const int maxn = 2e5 + 5;
const ll mod = 1e9 + 9;
void add(ll &x, ll y) { if ((x += y) >= mod) x -= mod; }
void sub(ll &x, ll y) { if ((x -= y) < 0) x += mod; }
struct mat {
    ll a[3][3];
    mat(int op) {
        if (op == 1) a[1][1] = a[2][2] = 1, a[1][2] = a[2][1] = 0;
        if (op == 0) a[1][1] = a[1][2] = a[2][1] = a[2][2] = 0;
    }
    mat operator*(const mat &A) {
        mat ans(0);
        rep(i, 1, 2) rep(j, 1, 2) rep(k, 1, 2) 
            add(ans.a[i][j], a[i][k] * A.a[k][j] % mod);
        return ans;
    }
};
mat powmod(mat a, ll b) {
    mat ans(1);
    while (b) {
        if (b & 1) ans = ans * a;
        b >>= 1; a = a * a;
    }
    return ans;
}
ll powmod(ll a, ll b) {
    ll ans = 1;
    while (b) {
        if (b & 1) ans = ans * a % mod;
        b >>= 1; a = a * a % mod;
    }
    return ans;
}
int case_Test() {
    auto f = [&](ll n) -> ll {
        if (n == 1) return 1;
        if (n == 2) return 2;
        mat A(0);
        A.a[1][1] = A.a[1][2] = A.a[2][1] = 1;
        A = powmod(A, n - 2);
        return (A.a[1][1] * 2 + A.a[1][2]) % mod;
    };
    ll n;
    scanf("%lld", &n);
    printf("%lld\n", f(n));
    return 0;
}
int main() {
#ifdef LOCAL
    freopen("/Users/chenjinglong/cpp_code/in.in", "r", stdin);
    freopen("/Users/chenjinglong/cpp_code/out.out", "w", stdout);
    clock_t start = clock();
#endif
    int _ = 1;
    scanf("%d", &_);
    while (_--) case_Test();
#ifdef LOCAL
    printf("Time used: %.3lfs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
#endif
    return 0;
}
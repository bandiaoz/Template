#include <bits/stdc++.h>

using namespace std;
using ll = long long;

constexpr int mod = 998244353;
// assume -mod <= x < 2P
int norm(int x) {
    if (x < 0) x += mod;
    if (x >= mod) x -= mod;
    return x;
}
template<class T>
T power(T a, int b) {
    T res = 1;
    for (; b; b /= 2, a *= a)
        if (b % 2) res *= a;
    return res;
}
struct Z {
    int x;
    Z(int x = 0) : x(norm(x)) {}
    Z(ll x) : x(norm(x % mod)) {}
    int val() const {
        return x;
    }
    Z operator-() const {
        return Z(norm(mod - x));
    }
    Z inv() const {
        assert(x != 0);
        return power(*this, mod - 2);
    }
    Z &operator*=(const Z &rhs) {
        x = ll(x) * rhs.x % mod;
        return *this;
    }
    Z &operator+=(const Z &rhs) {
        x = norm(x + rhs.x);
        return *this;
    }
    Z &operator-=(const Z &rhs) {
        x = norm(x - rhs.x);
        return *this;
    }
    Z &operator/=(const Z &rhs) {
        return *this *= rhs.inv();
    }
    friend Z operator*(const Z &lhs, const Z &rhs) {
        Z res = lhs;
        res *= rhs;
        return res;
    }
    friend Z operator+(const Z &lhs, const Z &rhs) {
        Z res = lhs;
        res += rhs;
        return res;
    }
    friend Z operator-(const Z &lhs, const Z &rhs) {
        Z res = lhs;
        res -= rhs;
        return res;
    }
    friend Z operator/(const Z &lhs, const Z &rhs) {
        Z res = lhs;
        res /= rhs;
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int N = 1e6;
    
    vector<Z> fac(N + 1), invfac(N + 1);
    fac[0] = 1;
    for (int i = 1; i <= N; i++) {
        fac[i] = fac[i - 1] * i;
    }
    invfac[N] = fac[N].inv();
    for (int i = N; i; i--) {
        invfac[i - 1] = invfac[i] * i;
    }
    
    auto binom = [&](int n, int m) {
        if (m < 0 || n < m) return Z(0);
        return fac[n] * invfac[m] * invfac[n - m];
    };
    

    return 0;
}
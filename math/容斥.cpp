#include <bits/stdc++.h>

using namespace std;
using ll = long long;

constexpr int mod = 998244353;
// assume -mod <= x < 2mod
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
    Z(ll x) : x(x % mod) {}
    
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
    
    int n, L;
    cin >> n >> L;
    vector<int> s(n);
    for (int i = 0; i < n; ++i) {
        string t;
        cin >> t;
        for (auto c : t) {
            s[i] |= 1 << (c - 'a');
        }
    }
    
    Z ans = 0;
    vector<Z> f(1 << n);
    for (int mask = 1; mask < (1 << n); ++mask) {
        int cur = (1 << 26) - 1;
        for (int i = 0; i < n; ++i) {
            if (mask >> i & 1) {
                cur &= s[i];
            }
        }
        f[mask] = power(Z(__builtin_popcount(cur)), L);
        ans += (__builtin_popcount(mask) & 1 ? 1 : -1) * f[mask];
    }

    cout << ans.val() << "\n";
    
    return 0;
}

// test problem: https://atcoder.jp/contests/abc246/tasks/abc246_f
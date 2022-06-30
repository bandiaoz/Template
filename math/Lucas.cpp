#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int P = 1e9 + 7;
// assume -P <= x < P
int norm(int x) {
    if (x < 0) x += P;
    if (x >= P) x -= P;
    return x;
}
template<class T>
T power(T a, ll b) {
    T res = 1;
    for (; b; b /= 2, a *= a) {
        if (b % 2) res *= a;
    }
    return res;
}
struct Z {
    int x;
    Z(int x = 0) : x(norm(x)) {}
    Z(int64_t x) : x(x % P) {}
    int val() const {
        return x;
    }
    Z operator-() const {
        return Z(norm(P - x));
    }
    Z inv() const {
        assert(x != 0);
        return power(*this, P - 2);
    }
    Z &operator*=(const Z &rhs) {
        x = int64_t(x) * rhs.x % P;
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
    friend istream &operator>>(istream &is, Z &a) {
        int64_t v;
        is >> v;
        a = Z(v);
        return is;
    }
    friend ostream &operator<<(ostream &os, const Z &a) {
        return os << a.val();
    }
};

struct Binom {
    const int N;
    vector<Z> fac, invfac;
    Binom(int n) : N(n), fac(N + 1), invfac(N + 1) {
        fac[0] = 1;
        for (int i = 1; i <= N; i++) {
            fac[i] = fac[i - 1] * i;
        }
        invfac[N] = fac[N].inv();
        for (int i = N; i; i--) {
            invfac[i - 1] = invfac[i] * i;
        }
    }
    
    Z get(int n, int m) {
        if (m < 0 || n < m) return Z(0);
        return fac[n] * invfac[m] * invfac[n - m];
    };
};

void solve() {
    int n, m;
    cin >> n >> m >> P;

    Binom binom(P - 1);

    function<ll(int, int, int)> Lucas = [&](int n, int m, int P) {
        if (m == 0) return 1LL;
        return 1LL * binom.get(n % P, m % P).val() * Lucas(n / P, m / P, P) % P;
    };

    cout << Lucas(n + m, m, P) << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
// test problem: https://www.luogu.com.cn/problem/P3807
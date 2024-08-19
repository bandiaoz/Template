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
T power(T a, int64_t b) {
    T res = 1;
    for (; b; b /= 2, a *= a) {
        if (b % 2) res *= a;
    }
    return res;
}
struct Z {
    int x;
    Z(int x = 0) : x(norm(x)) {}
    Z(int64_t x) : x(x % mod) {}
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
        x = int64_t(x) * rhs.x % mod;
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    vector<int> a;
    vector<Z> g;
    auto insert = [&](int x, int y) {
        a.push_back(x);
        int n = a.size();
        vector<Z> pre{1};
        for (int i = 0; i < n - 1; ++i) {
            pre.push_back(pre.back() * (a[i] - x));
        }
        Z inv = pre.back().inv();
        g.push_back((n % 2 == 0 ? -1 : 1) * y * inv);

        for (int i = n - 2; i >= 0; --i) {
            g[i] *= inv * pre[i];
            inv *= a[i] - x;
        }
    };

    auto query = [&](int k) {
        int n = a.size();
        vector<Z> suf(n);
        suf.back() = 1;
        for (int i = n - 1; i >= 1; --i) {
            suf[i - 1] = suf[i] * (k - a[i]);
        }

        Z ans = 0, j = 1;
        for (int i = 0; i < n; ++i) {
            ans += j * suf[i] * g[i];
            j *= k - a[i];
        }

        return ans;
    };

    for (int i = 0; i < n; ++i) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, y;
            cin >> x >> y;
            insert(x, y);
        } else {
            int k;
            cin >> k;
            cout << query(k) << "\n";
        }
    }

    return 0;
}
// test problem: https://loj.ac/p/166
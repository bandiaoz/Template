#include <bits/stdc++.h>

using namespace std;
using ll = long long;

void solve() {
    ll a, b, c;
    cin >> a >> b >> c;

    // ax + by = gcd(a, b) = d
    // return tuple(d, x, y)
    function<tuple<int64_t, int64_t, int64_t>(int64_t, int64_t)> exgcd = [&](int64_t a, int64_t b) {
        if (b == 0) {
            return tuple(a, 1LL, 0LL);
        }
        auto [d, x, y] = exgcd(b, a % b);
        return tuple(d, y, x - a / b * y);
    };

    auto [d, x, y] = exgcd(a, b);

    if (c % d != 0) {
        cout << "-1\n";
    } else {
        x *= c / d;
        y *= c / d;

        ll dx = b / d;
        ll dy = a / d;

        ll l = ceil(1.0 * (-x + 1) / dx);
        ll r = floor(1.0 * (y - 1) / dy);

        if (l > r) {
            cout << x + l * dx << " " << y - r * dy << "\n";
        } else {
            ll minx = x + l * dx, maxx = x + r * dx;
            ll miny = y - r * dy, maxy = y - l * dy;
            cout << r - l + 1 << " " << minx << " " << miny << " " << maxx << " " << maxy << "\n";
        }
    }
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

// test problem: https://www.luogu.com.cn/problem/P5656
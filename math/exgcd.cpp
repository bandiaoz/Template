#include <bits/stdc++.h>

/**
 * @brief Extended Euclid Algorithm
 * @param a, b: int64_t
 * @return tuple(g, x, y)
 * @note ax + by = gcd(a, b) = g
 * @note g = gcd(a, b) = gcd(b, a % b)
*/
std::tuple<int64_t, int64_t, int64_t> exgcd(int64_t a, int64_t b) {
    if (b == 0) {
        return std::tuple(a, 1, 0);
    }
    auto [g, x, y] = exgcd(b, a % b);
    return std::tuple(g, y, x - a / b * y);
}

void solve() {
    int64_t a, b, c;
    std::cin >> a >> b >> c;

    auto [g, x, y] = exgcd(a, b);

    if (c % g != 0) {
        std::cout << "-1\n";
    } else {
        a /= g, b /= g, c /= g;
        x *= c, y *= c;

        // a(x + bk) + b(y - ak) = c, satisfy gcd(a, b) = 1
        // we get -x / b < k < y / a, so k in [ceil((-x + 1) / b), floor((y - 1) / a]
        int64_t l = ceil(1. * (-x + 1) / b);
        int64_t r = floor(1. * (y - 1) / a);

        if (l > r) {
            std::cout << x + l * b << " " << y - r * a << "\n";
        } else {
            int64_t minx = x + l * b, maxx = x + r * b;
            int64_t miny = y - r * a, maxy = y - l * a;
            std::cout << r - l + 1 << " " << minx << " " << miny << " " << maxx << " " << maxy << "\n";
        }
    }
}

// test problem: https://www.luogu.com.cn/problem/P5656
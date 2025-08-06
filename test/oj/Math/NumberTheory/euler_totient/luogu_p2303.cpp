#include <bits/stdc++.h>
#include "src/Math/NumberTheory/PollardRho.h"

/*
[P2303 SDOI2012 Longge的问题](https://www.luogu.com.cn/problem/P2303)
[status](https://www.luogu.com.cn/record/229210001)
*/

/*
给定 $$n(1 \leq n < 2^{32})$$，求 $$\sum_{i = 1}^n \gcd(i, n)$$

枚举 $$g= \gcd(i, n)$$，$$g$$ 是 $$n$$ 的因子，则有

$$g \times \sum_{i = 1}^n [\gcd(i, n) = g]$$

令 $$l = n / g$$，则有

$$g \sum_{i = 1}^l [\gcd(i, l) = 1]$$

$$g \times \varphi(l)$$
*/

OY::PollardRho rho;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t n;
    std::cin >> n;

    auto get = [&](uint64_t g) {
        return g * rho.get_Euler_Phi(n / g);
    };

    uint64_t ans = 0;
    for (uint64_t x = 1; x * x <= n; x++) {
        if (n % x == 0) {
            ans += get(x);
            if (x * x != n) {
                ans += get(n / x);
            }
        }
    }

    std::cout << ans << '\n';

    return 0;
}
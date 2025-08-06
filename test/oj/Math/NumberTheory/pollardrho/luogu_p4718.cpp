#include <bits/stdc++.h>
#include "src/Math/NumberTheory/PollardRho.h"

/**
 * [P4718【模板】Pollard-Rho](https://www.luogu.com.cn/problem/P4718)
 * [status](https://www.luogu.com.cn/record/173870688)
 */

void solve() {
    uint64_t n;
    std::cin >> n;

    uint64_t max = 0;
    OY::PollardRho::enumerate_prime_factors(n, [&](uint64_t prime) {
        max = std::max(max, prime);
    });

    if (max == n) {
        std::cout << "Prime\n";
    } else {
        std::cout << max << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
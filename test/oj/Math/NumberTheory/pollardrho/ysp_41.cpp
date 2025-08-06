#include <bits/stdc++.h>
#include "src/Math/NumberTheory/PollardRho.h"

using ll = long long;

/*
[Factorize](https://judge.yosupo.jp/problem/factorize)(https://github.com/yosupo06/library-checker-problems/issues/41)
[status](https://judge.yosupo.jp/submission/246034)
*/
/**
 * 本题为质因数分解模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    while (t--) {
        ll x;
        std::cin >> x;
        std::vector<ll> ans;
        OY::PollardRho::enumerate_prime_factors(x, [&](auto p) {
            ans.push_back(p);
        });
        std::sort(ans.begin(), ans.end());
        std::cout << ans.size();
        for (const auto& p : ans) {
            std::cout << ' ' << p;
        }
        std::cout << '\n';
    }

    return 0;
}
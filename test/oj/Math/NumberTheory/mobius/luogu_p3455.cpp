#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"

using ll = long long;

/*
[P3455 [POI2007]ZAP-Queries](https://www.luogu.com.cn/problem/P3455)
[status](https://www.luogu.com.cn/record/230909646)
*/
/*
给定 $$n, m$$，求 $$\sum_{i = 1}^n \sum_{j = 1}^m [\gcd(i, j) = g]$$

多测，$$1 \leq T, n, m, g \leq 50000$$

$$\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} [\gcd(i, j) = 1]$$

根据反演公式，有

$$\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} \sum_{d \mid \gcd(i, j)} \mu(d)$$

枚举 $$d$$，有

$$\sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \left\lfloor \frac{n}{dg} \right\rfloor \left\lfloor \frac{m}{dg} \right\rfloor$$

*/

OY::MOBIUS::Table<50000, true> mobius;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        int n, m, g;
        std::cin >> n >> m >> g;
        if (n > m) std::swap(n, m);

        ll ans = 0;
        for (auto range : OY::PairSqrtDecomposition<uint32_t>(n / g, m / g)) {
            int l = range.left(), r = range.right();
            int q1 = range.quot1(), q2 = range.quot2();
            ans += 1LL * q1 * q2 * mobius.query_mobius(l, r + 1);
        }
        std::cout << ans << '\n';
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"
#include "src/Math/NumberTheory/Mobius.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[P2522 [HAOI2011] Problem b](https://www.luogu.com.cn/problem/P2522)
[status](https://www.luogu.com.cn/record/230661594)
*/
/*
有 $$n$$ 个询问，每次求 $$\sum_{i = a}^b \sum_{j = c}^d [\gcd(i, j) = k]$$

$$1 \leq n, k \leq 5 \times 10^4, 1 \leq a \leq b \leq 5 \times 10^4, 1 \leq c \leq d \leq 5 \times 10^4$$

求出 $$f(n, m) = \sum_{i = 1}^n \sum_{j = 1}^m [\gcd(i, j) = k]$$ 即可，可以通过容斥得到答案。

$$f(n, m) = \sum_{i = 1}^n \sum_{j = 1}^m [\gcd(i, j) = k] = \sum_{i = 1}^{\left\lfloor \frac{n}{k} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{k} \right\rfloor} [\gcd(i, j) = 1]$$

通过反演公式，得到

$$\sum_{i = 1}^{\left\lfloor \frac{n}{k} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{k} \right\rfloor} \sum_{d \mid \gcd(i, j)} \mu(d)$$


枚举 $$d$$，得到

$$\sum_{d = 1}^{\left\lfloor \frac{n}{k} \right\rfloor} \mu(d) \left\lfloor \frac{n}{dk} \right\rfloor \left\lfloor \frac{m}{dk} \right\rfloor$$

*/

OY::MOBIUS::Table<50000, true> mobius;

void solve() {
    int a, b, c, d, k;
    std::cin >> a >> b >> c >> d >> k;

    auto work = [&](int n, int m) -> ll {
        if (n > m) std::swap(n, m);
        n /= k, m /= k;
        if (n == 0) return 0;
        ll ans = 0;
        for (auto range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            int l = range.left(), r = range.right();
            int q1 = range.quot1(), q2 = range.quot2();
            ans += 1LL * q1 * q2 * mobius.query_mobius(l, r + 1);
        }
        return ans;
    };

    std::cout << work(b, d) - work(a - 1, d) - work(b, c - 1) + work(a - 1, c - 1) << '\n';
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
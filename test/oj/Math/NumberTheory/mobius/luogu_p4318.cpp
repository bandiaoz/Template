#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"

using ll = long long;

/*
[P4318 完全平方数](https://www.luogu.com.cn/problem/P4318)
[status](https://www.luogu.com.cn/record/241032510)
*/
/*
$$t(1 \leq t \leq 50)$$ 次询问，每次询问第 $$k(1 \leq k \leq 10^9)$$ 个没有完全平方因子因子的正整数。

即求最小的 $$n$$，满足 $$\sum_{i = 1}^n \mu^2(i) \geq k$$

二分答案，然后快速求出 $$\sum_{i = 1}^n \mu^2(i)$$ 即可

由 $$\mu^2(n) = \sum_{i^2 \mid n} \mu(i)$$，可以得到

$$\sum_{i = 1}^n \mu^2(i) = \sum_{i = 1}^n \mu(i) \left\lfloor \frac{n}{i^2} \right\rfloor$$

*/

const int N = 10'000'000;
OY::MOBIUS::Table<N> mobius;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    
    while (t--) {
        int k;
        std::cin >> k;

        auto judge = [&](ll mid) {
            ll sum = 0;
            for (int i = 1; 1LL * i * i <= mid; i++) {
                sum += mobius.query_mobius(i) * (mid / (1LL * i * i));
            }
            debug(mid, sum);
            return sum >= k;
        };

        ll l = 1, r = 1e10;
        while (l < r) {
            ll mid = (l + r) / 2;
            if (judge(mid)) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }

        std::cout << l << '\n';
    }
    

    return 0;
}
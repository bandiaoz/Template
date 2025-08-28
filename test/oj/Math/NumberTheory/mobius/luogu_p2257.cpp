#include <bits/stdc++.h>
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"
#include "src/Math/NumberTheory/Mobius.h"
#include "src/DataStruct/AdjDiff.h"

using ll = long long;

/*
[P2257 YY的GCD](https://www.luogu.com.cn/problem/P2257)
[status](https://www.luogu.com.cn/record/230654255)
*/
/*
给定 $$n, m$$，求 $$1 \leq x \leq n, 1 \leq y \leq m$$ 且 $$\gcd(x, y)$$ 为质数的 $$(x, y)$$ 有多少对。
$$T \leq 10^4, n, m \leq 10^7$$

题意为求 $$\sum_{i = 1}^n \sum_{j = 1}^m [\gcd(i, j) \in \text{prime}]$$

不妨 $$n \leq m$$ 枚举质数 $$(i, j) = p \leq n$$

则答案为

$$\sum_{p = 1}^n \sum_{i = 1}^{n} \sum_{j = 1}^{m} [(i, j) = p]$$


$$\sum_{p = 1}^n \sum_{i = 1}^{\left\lfloor \frac{n}{p} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{p} \right\rfloor} [(i, j) = 1]$$


根据反演公式有：

$$\sum_{p = 1}^n \sum_{i = 1}^{\left\lfloor \frac{n}{p} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{p} \right\rfloor} \sum_{d \mid \gcd(i, j)} \mu(d)$$


枚举 $$d$$，提到前面，得到

$$\sum_{p = 1}^n \sum_{d = 1}^{\left\lfloor \frac{n}{p} \right\rfloor} \mu(d) \left\lfloor \frac{n}{dp} \right\rfloor \left\lfloor \frac{m}{dp} \right\rfloor$$


令 $$t = dp$$，则

$$\sum_{p = 1}^n \sum_{d = 1}^{\left\lfloor \frac{n}{p} \right\rfloor} \mu(d) \left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor$$


$$\sum_{t = 1}^n \left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor \sum_{p \mid t} \mu(\frac{t}{p})$$


$$f(t) = \sum_{p \mid t} \mu(\frac{t}{p})$$ 是可以预处理的。

*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    const int N = 1e7;
    OY::MOBIUS::Table<N> mu;
    std::vector<int> f(N + 1);
    for (int p = 2; p <= N; p++) {
        if (!mu.is_prime(p)) continue;
        for (int t = p; t <= N; t += p) {
            f[t] += mu.query_mobius(t / p);
        }
    }
    OY::AdjSumTable<int> table(f.begin(), f.end());

    while (t--) {
        int n, m;
        std::cin >> n >> m;
        if (n > m) std::swap(n, m);
    
        ll ans = 0;
        for (auto range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            int l = range.left(), r = range.right();
            int q1 = range.quot1(), q2 = range.quot2();
            ans += 1LL * q1 * q2 * table.query(l, r + 1);
        }

        std::cout << ans << '\n';
    }

    return 0;
}
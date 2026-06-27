#include <bits/stdc++.h>
#include "src/Math/NumberTheory/EulerSieve.h"

using ll = long long;

/*
[P1447 [NOI2010] 能量采集](https://www.luogu.com.cn/problem/P1447)
[status](https://www.luogu.com.cn/record/240681377)
*/
/*
给定 $$n, m(1 \leq n, m \leq 10^5)$$，求 $$\sum_{i=1}^{n} \sum_{j=1}^{m} \left(\gcd(i, j) - 1\right) \times 2 + 1$$


$$\sum_{i=1}^{n} \sum_{j=1}^{m} \left(\gcd(i, j) - 1\right) \times 2 + 1 = 2\sum_{i=1}^{n} \sum_{j=1}^{m} \gcd(i, j) - n \times m$$


接下来求解 $$\sum_{i=1}^{n} \sum_{j=1}^{m} \gcd(i, j)$$

根据欧拉反演，有

$$\sum_{i=1}^{n} \sum_{j=1}^{m} \sum_{d|(i, j)} \varphi(d)$$

枚举 $$d$$，有

$$\sum_{d = 1}^n \varphi(d) \left\lfloor\frac{n}{d}\right\rfloor \left\lfloor\frac{m}{d}\right\rfloor$$

*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    if (n > m) {
        std::swap(n, m);
    }

    const int N = 100'000;
    OY::EulerSieve::Sieve<N, true> sieve;
    ll ans = 0;
    for (int i = 1; i <= std::min(n, m); i++) {
        ans += 1LL * sieve.get_Euler_Phi(i) * (n / i) * (m / i);
    }

    std::cout << 2 * ans - 1LL * n * m << '\n';

    return 0;
}
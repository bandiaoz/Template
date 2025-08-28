#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::mint1000000007;

/*
[P3704 [SDOI2017]数字表格](https://www.luogu.com.cn/problem/P3704)
[status](https://www.luogu.com.cn/record/230946006)
*/
/*
给定 $$n, m$$，求 $$\prod_{i = 1}^n \prod_{j = 1}^m fib(\gcd(i, j))$$，其中 $$fib(n)$$ 表示斐波那契数列的第 $$n$$ 项

多测，$$1 \leq T \leq 10^3, 1 \leq n, m \leq 10^6$$

枚举 $$g = \gcd(i, j)$$，则有

$$\prod_{g = 1}^n fib(g)^{\sum_{i = 1}^{n} \sum_{j = 1}^{m} [\gcd(i, j) = g]}$$

其中指数部分为

$$\sum_{i = 1}^{n} \sum_{j = 1}^{m} [(i, j) = g]$$


$$\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} [(i, j) = 1]$$

根据反演公式，有

$$\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} \sum_{d \mid \gcd(i, j)} \mu(d)$$

枚举 $$d$$，有

$$\sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \left\lfloor \frac{n}{dg} \right\rfloor \left\lfloor \frac{m}{dg} \right\rfloor$$


所以，所求为

$$\prod_{g = 1}^n fib(g)^{\sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \left\lfloor \frac{n}{dg} \right\rfloor \left\lfloor \frac{m}{dg} \right\rfloor}$$

令 $$t = gd$$，则有

$$\prod_{g = 1}^n fib(g)^{\sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor}$$


$$\prod_{t = 1}^n \prod_{g \mid t} fib(g)^{\mu(\frac{t}{g}) \left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor}$$

令 $$f(t) = \prod_{g \mid t} fib(g)^{\mu(\frac{t}{g})}$$，预处理出 $$f(t)$$，复杂度为 $$O(n \log n)$$，则有

$$\prod_{t = 1}^n f(t)^{\left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor}$$

*/

const int N = 1000000;
OY::MOBIUS::Table<N> mobius;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    std::vector<Z> fib(N + 1), fib_inv(N + 1);
    fib[1] = fib[2] = 1;
    fib_inv[1] = fib_inv[2] = 1;
    for (int i = 3; i <= N; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
        fib_inv[i] = fib[i].inv();
    }
    
    std::vector<Z> f(N + 1, 1);
    for (int g = 1; g <= N; g++) {
        for (int t = g; t <= N; t += g) {
            if (int mu = mobius.query_mobius(t / g); mu == 1) {
                f[t] *= fib[g];
            } else if (mu == -1) {
                f[t] *= fib_inv[g];
            }
        }
    }

    std::vector<Z> f_pre_prod(N + 1, 1);
    for (int i = 1; i <= N; i++) {
        f_pre_prod[i] = f_pre_prod[i - 1] * f[i];
    }

    while (t--) {
        int n, m;
        std::cin >> n >> m;
        if (n > m) std::swap(n, m);

        Z ans = 1;
        for (auto range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            int l = range.left(), r = range.right();
            int q1 = range.quot1(), q2 = range.quot2();
            ans *= (f_pre_prod[r] / f_pre_prod[l - 1]).pow(1LL * q1 * q2);
        }
        std::cout << ans << '\n';
    }

    return 0;
}
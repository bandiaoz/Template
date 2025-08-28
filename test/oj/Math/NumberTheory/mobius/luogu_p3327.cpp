#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"
#include "src/Math/NumberTheory/SqrtDecomposition.h"

using ll = long long;

/*
[P3327 [SDOI2015] 约数个数和](https://www.luogu.com.cn/problem/P3327)
[status](https://www.luogu.com.cn/record/230905722)
*/
/*
给定 $$n, m$$，求 $$\sum_{i = 1}^n \sum_{j = 1}^m d(ij)$$

多测，$$1 \leq T, n, m \leq 50000$$

根据 $$d(ij) = \sum_{x \mid i} \sum_{y \mid j} [(x, y) = 1]$$，则有

$$\sum_{i = 1}^n \sum_{j = 1}^m \sum_{x \mid i} \sum_{y \mid j} [(x, y) = 1]$$

先枚举 $$x, y$$，有

$$\sum_{x = 1}^n \sum_{y = 1}^m \left\lfloor \frac{n}{x} \right\rfloor \left\lfloor \frac{m}{y} \right\rfloor [(x, y) = 1]$$

变量名称换回 $$i, j$$，则有

$$\sum_{i = 1}^n \sum_{j = 1}^m \left\lfloor \frac{n}{i} \right\rfloor \left\lfloor \frac{m}{j} \right\rfloor [(i, j) = 1]$$

根据反演公式，有

$$\sum_{i = 1}^n \sum_{j = 1}^m \left\lfloor \frac{n}{i} \right\rfloor \left\lfloor \frac{m}{j} \right\rfloor \sum_{d \mid \gcd(i, j)} \mu(d)$$

枚举 $$d$$，得到

$$\sum_{d = 1}^n \mu(d) \sum_{i = 1}^{\left\lfloor \frac{n}{d} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{d} \right\rfloor} \left\lfloor \frac{n}{id} \right\rfloor \left\lfloor \frac{m}{jd} \right\rfloor$$

所求即为三项的乘积

$$\sum_{d = 1}^n \mu(d) \left(\sum_{i = 1}^{\left\lfloor \frac{n}{d} \right\rfloor} \left\lfloor \frac{n}{id} \right\rfloor\right) \left(\sum_{j = 1}^{\left\lfloor \frac{m}{d} \right\rfloor} \left\lfloor \frac{m}{jd} \right\rfloor\right)$$


令 $$f(x) = \sum_{i = 1}^x \left\lfloor \frac{x}{i} \right\rfloor$$，可以预处理出 $$f$$ 数组，则有

$$\sum_{d = 1}^n \mu(d) f\left(\left\lfloor \frac{n}{d} \right\rfloor\right) f\left(\left\lfloor \frac{m}{d} \right\rfloor\right)$$

*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    const int N = 50000;
    OY::MOBIUS::Table<N, true> mobius;

    std::vector<ll> f(N + 1);
    for (int i = 1; i <= N; i++) {
        for (auto range : OY::SqrtDecomposition<uint32_t>(i)) {
            int l = range.left(), r = range.right();
            int q = range.quot();
            f[i] += (r - l + 1) * q;
        }
    }

    while (t--) {
        int n, m;
        std::cin >> n >> m;
        if (n > m) std::swap(n, m);

        ll ans = 0;
        for (auto range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            int l = range.left(), r = range.right();
            int q1 = range.quot1(), q2 = range.quot2();
            ans += mobius.query_mobius(l, r + 1) * f[q1] * f[q2];
        }
        std::cout << ans << '\n';
    }    

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/Modular/StaticModInt32.h"
#include "src/DataStruct/AdjDiff.h"
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"

using Z = OY::StaticModInt32<20101009, true>;

/*
[P1829 [国家集训队] Crash的数字表格 / JZPTAB](https://www.luogu.com.cn/problem/P1829)
[status](https://www.luogu.com.cn/record/230714336)
*/
/*
求 $$\sum_{i = 1}^n \sum_{j = 1}^m \mathrm{lcm}(i, j)$$，其中 $$n, m \leq 10^7$$


$$\sum_{i = 1}^n\sum_{j = 1}^m \mathrm{lcm}(i, j) = \sum_{i = 1}^n\sum_{j = 1}^m \frac{ij}{(i, j)}$$


枚举 $$g = (i, j)$$，则

$$\sum_{g = 1}^n \sum_{i = 1}^n\sum_{j = 1}^m \frac{ij}{g}[(i, j) = g]$$


$$\sum_{g = 1}^n \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} ijg[(i, j) = 1]$$


利用反演公式，得到

$$\sum_{g = 1}^n \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} ijg \sum_{d \mid (i, j)} \mu(d)$$


枚举 $$d$$，得到

$$\sum_{g = 1}^n \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) d^2 g \sum_{i = 1}^{\left\lfloor \frac{n}{gd} \right\rfloor} i \sum_{j = 1}^{\left\lfloor \frac{m}{gd} \right\rfloor} j$$


后两项为等差数列求和，令 $$\operatorname{sum}(n) = \frac{n(n + 1)}{2}$$，则

$$\sum_{g = 1}^n \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) d^2 g \operatorname{sum}(\left\lfloor \frac{n}{gd} \right\rfloor) \operatorname{sum}(\left\lfloor \frac{m}{gd} \right\rfloor)$$


令 $$t = gd$$，则

$$\sum_{g = 1}^n \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) dt \operatorname{sum}(\left\lfloor \frac{n}{t} \right\rfloor) \operatorname{sum}(\left\lfloor \frac{m}{t} \right\rfloor)$$


枚举 $$t$$，得到

$$\sum_{t = 1}^n \left(\operatorname{sum}(\left\lfloor \frac{n}{t} \right\rfloor) \operatorname{sum}(\left\lfloor \frac{m}{t} \right\rfloor) \times t \sum_{d \mid t} \mu(d) d\right)$$


前半部分可以通过数论分块求解，后半部分 $$f(t) = t\sum_{d \mid t} \mu(d) d$$ 通过积性函数预处理得到。
*/

const int N = 1e7;
OY::MOBIUS::Table<N, true> mobius;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    if (n > m) std::swap(n, m);

    auto query = [&](int d) { return mobius.query_mobius(d) * d; };
    auto calc_prime = [&](int p) { return query(1) + query(p); };
    auto calc_prime_pow = [&](int p, int c, int low) { return calc_prime(p); };
    auto f = OY::MOBIUS::Multiplicative<N>().solve<Z>(N, calc_prime, calc_prime_pow);
    OY::AdjSumTable<Z> table(N + 1, [&](int i) { return Z(i) * f[i]; });

    auto sum = [&](int n) -> Z { return Z(1) * n * (n + 1) / 2; };

    Z ans = 0;
    for (auto range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
        int l = range.left(), r = range.right();
        int q1 = range.quot1(), q2 = range.quot2();
        ans += Z(1) * sum(q1) * sum(q2) * table.query(l, r + 1);
    }

    std::cout << ans << "\n";

    return 0;
}
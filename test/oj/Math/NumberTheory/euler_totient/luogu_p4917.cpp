#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/NumberTheory/EulerSieve.h"
#include "src/Math/NumberTheory/SqrtDecomposition.h"
#include "src/DataStruct/AdjDiff.h"

using Z = OY::StaticModInt32<19260817, true>;
using EXP = OY::StaticModInt32<19260816, false>;

/*
[P4917 天守阁的地板](https://www.luogu.com.cn/problem/P4917)
[status](https://www.luogu.com.cn/record/240826088)
*/
/*
给定 $$n(1 \leq n \leq 10^6)$$，求 $$\prod_{i = 1}^n \prod_{j = 1}^n \frac{\operatorname{lcm}(i, j)\operatorname{lcm}(i, j)}{i \times j}$$
$$t(1 \leq t \leq 1000)$$ 次询问

$$\prod_{i = 1}^n \prod_{j = 1}^n \frac{\operatorname{lcm}(i, j)\operatorname{lcm}(i, j)}{i \times j} = \prod_{i = 1}^n \prod_{j = 1}^n \frac{ij}{(i, j)^2} = \prod_{i = 1}^n \prod_{i = 1}^n \frac{(n!)^{2n}}{(i, j)^2}$$


接下来求 $$\prod_{i = 1}^n\prod_{j = 1}^n (i, j)$$，枚举 $$g = (i, j)$$，则有

$$\prod_{g = 1}^n g^{\sum_{i = 1}^n \sum_{j = 1}^n [(i, j) = g]}$$


$$\prod_{g = 1}^n g^{\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} [(i, j) = 1]}$$


对于指数部分 $$\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} [(i, j) = 1]$$，根据欧拉函数的定义，有 

$$2 \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \varphi(i) - 1$$


带回原式，得到

$$\prod_{g = 1}^n g^{\left(2 \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \varphi(i) - 1\right)}$$


对 $$g$$ 进行分块即可
*/

const int N = 1'000'000;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    std::vector<Z> fac(N + 1);
    fac[0] = 1;
    for (int i = 1; i <= N; i++) {
        fac[i] = fac[i - 1] * i;
    }

    OY::EulerSieve::Sieve<N, true> sieve;
    OY::AdjSumTable<EXP> phi_presum(N + 1, [&](int x) { return sieve.get_Euler_Phi(x); });
    
    while (t--) {
        int n;
        std::cin >> n;

        Z num = fac[n].pow(2 * n);
        Z den = 1;
        for (auto [quot, left, right] : OY::SqrtDecomposition<uint32_t>(n)) {
            Z a = fac[right] / fac[left - 1];
            EXP b = phi_presum.query(1, quot + 1) * 2 - 1;
            den *= a.pow(b.val());
        }
        den *= den;

        std::cout << num / den << '\n';
    }

    return 0;
}
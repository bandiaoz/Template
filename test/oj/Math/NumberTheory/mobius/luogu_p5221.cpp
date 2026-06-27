#include <bits/stdc++.h>
#include "src/Math/NumberTheory/EulerSieve.h"
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/NumberTheory/SqrtDecomposition.h"

using ll = long long;
using Z = OY::StaticModInt32<104857601, true>;
using EXP = OY::StaticModInt32<104857600, false>;

/*
[P5221 Product](https://www.luogu.com.cn/problem/P5221)
[status phi](https://www.luogu.com.cn/record/230957411)
[status mobius](https://www.luogu.com.cn/record/230960924)
*/
/*

给定正整数 $$n(1 \leq n \leq 10^6)$$，求 $$\prod_{i=1}^n\prod_{j=1}^n\frac{\operatorname{lcm}(i,j)}{\gcd(i,j)}\pmod{104857601}$$

由 $$\operatorname{lcm}(i,j) = \frac{ij}{(i,j)}$$ 可得

$$\prod_{i=1}^n\prod_{j=1}^n\frac{ij}{(i,j)^2}$$


分别计算 $$\prod_{i = 1}^n \prod_{j = 1}^n ij$$ 和 $$\prod_{i = 1}^n \prod_{j = 1}^n (i, j)$$，然后做除法即可


对于 $$\prod_{i = 1}^n \prod_{j = 1}^n ij$$，可以拆分为 $$\prod_{i = 1}^n i^n \prod_{j = 1}^n j^n$$，即 $$(n!)^{2n}$$


对于 $$\prod_{i = 1}^n \prod_{j = 1}^n (i, j)$$，枚举 $$g = (i, j)$$，则


$$\prod_{g = 1}^n g^{\sum_{i = 1}^n \sum_{j = 1}^n [(i, j) = g]}$$


$$\prod_{g = 1}^n g^{\sum_{i = 1}^{\lfloor n / g \rfloor} \sum_{j = 1}^{\lfloor n / g \rfloor} [(i, j) = 1]}$$


其中指数部分 $$\sum_{i = 1}^{\lfloor \frac{n}{g} \rfloor} \sum_{j = 1}^{\lfloor \frac{n}{g} \rfloor} [(i, j) = 1]$$ 可以用欧拉函数转换为 $$2 \varphi(i) - 1$$，复杂度为 $$O(n)$$

也可以使用莫比乌斯反演

$$\sum_{i = 1}^{\lfloor \frac{n}{g} \rfloor} \sum_{j = 1}^{\lfloor \frac{n}{g} \rfloor} \sum_{d | (i, j)} \mu(d)$$


$$\sum_{d = 1}^{\lfloor \frac{n}{g} \rfloor} \mu(d) \left\lfloor \frac{n}{gd} \right\rfloor^2$$

再对 $$\left \lfloor \frac{n}{g} \right \rfloor$$ 做分块即可，复杂度为 $$O(n)$$
*/

const int N = 1000000;

void solve_phi() {
    int n;
    std::cin >> n;

    OY::EulerSieve::Sieve<N, true> sieve;
    std::vector<ll> phi_presum(n + 1);
    for (int i = 1; i <= n; i++) {
        phi_presum[i] = phi_presum[i - 1] + sieve.get_Euler_Phi(i);
    }

    Z num = 1;
    for (int i = 1; i <= n; i++) {
        num *= i;
    }
    num = num.pow(2 * n);

    Z den = 1;
    for (auto range : OY::SqrtDecomposition<uint32_t>(n)) {
        int l = range.left(), r = range.right();
        int q = range.quot();

        Z fac = 1;
        for (int i = l; i <= r; i++) {
            fac *= i;
        }
        EXP exp = EXP(phi_presum[q] * 2 - 1);
        den *= fac.pow(exp.val());
    }
    den *= den;

    std::cout << num / den << '\n';
}

void solve_mobius() {
    int n;
    std::cin >> n;

    Z num = 1;
    for (int i = 1; i <= n; i++) {
        num *= i;
    }
    num = num.pow(2 * n);

    OY::MOBIUS::Table<N, true> mobius(n);
    Z den = 1;
    for (auto range : OY::SqrtDecomposition<uint32_t>(n)) {
        int l = range.left(), r = range.right();
        int q = range.quot();
        
        Z fac = 1;
        for (int i = l; i <= r; i++) {
            fac *= i;
        }
        EXP exp = 0;
        for (auto range2 : OY::SqrtDecomposition<uint32_t>(q)) {
            int l2 = range2.left(), r2 = range2.right();
            int q2 = range2.quot();
            exp += EXP(1) * mobius.query_mobius(l2, r2 + 1) * q2 * q2;
        }
        den *= fac.pow(exp.val());
    }
    den *= den;

    std::cout << num / den << '\n';
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // solve_phi();
    solve_mobius();

    return 0;
}
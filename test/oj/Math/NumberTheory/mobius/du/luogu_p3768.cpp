#include <bits/stdc++.h>
#include "src/Math/Modular/DynamicModInt32.h"
#include "src/Math/NumberTheory/DuSieve.h"
#include "src/Math/NumberTheory/EulerSieve.h"
#include "src/Math/NumberTheory/SqrtDecomposition.h"

using Z = OY::DynamicModInt32<0>;

/*
[P3768 简单的数学题](https://www.luogu.com.cn/problem/P3768)
[status](https://www.luogu.com.cn/record/240477730)
*/
/*
给定整数 $$n, p$$，求 $$\left(\sum_{i=1}^n\sum_{j=1}^n ij \gcd(i,j)\right) \bmod p$$

$$n \leq 10^{10}$$，$$5 \times 10^8 \leq p \leq 1.1 \times 10^9$$ 且 $$p$$ 为质数。


枚举 $$g = (i, j)$$，则有 

$$\sum_{g = 1}^n \sum_{i = 1}^n \sum_{j = 1}^n ijg[(i, j) = g]$$


$$\sum_{g = 1}^n \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} ijg^3[(i, j) = 1]$$


根据莫比乌斯反演，有

$$\sum_{g = 1}^n g^3 \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} ij \sum_{d | (i, j)} \mu(d)$$


将 $$d$$ 提到前面，则有

$$\sum_{g = 1}^n g^3 \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \sum_{i = 1}^{\left\lfloor \frac{n}{dg} \right\rfloor} id \sum_{j = 1}^{\left\lfloor \frac{n}{dg} \right\rfloor} jd $$


令 $$\mathrm{sum}(n) = \sum_{i = 1}^n i$$，则有

$$\sum_{g = 1}^n g^3 \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) d^2 \mathrm{sum}\left(\left\lfloor \frac{n}{dg} \right\rfloor\right)^2$$


令 $$t = dg$$，则有

$$\sum_{g = 1}^n g^3 \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) d^2 \mathrm{sum}\left(\left\lfloor \frac{n}{t} \right\rfloor\right)^2$$


$$\sum_{t = 1}^n \mathrm{sum}\left(\left\lfloor \frac{n}{t} \right\rfloor\right)^2 \sum_{g \mid t} g^3 \mu\left(\frac{t}{g}\right) \left(\frac{t}{g}\right)^2$$


$$\sum_{t = 1}^n \mathrm{sum}\left(\left\lfloor \frac{n}{t} \right\rfloor\right)^2 t^2 \sum_{g \mid t} g \mu\left(\frac{t}{g}\right)$$

后面部分是 $$\mu \ast id = \varphi$$，故

$$\sum_{t = 1}^n \mathrm{sum}\left(\left\lfloor \frac{n}{t} \right\rfloor\right)^2 t^2 \varphi(t)$$

接下来考虑杜教筛求解 $$f(t) = \varphi(t) t^2$$ 的前缀和

令 $$g(t) = t^2 = \mathrm{id}^2$$，则有

$$(f \ast g)(t) = \sum_{d \mid t} \varphi(d) d^2 \left(\frac{t}{d}\right)^2 = t^2 \sum_{d \mid t} \varphi(d)$$

根据 $$\varphi \ast 1 = \mathrm{id}$$，则有

$$(f \ast g)(t) = t^3$$

套用杜教筛的公式：

$$S(n) = \frac{\sum_{i = 1}^n (f \ast g)(i) - \sum_{i = 2}^n g(i)S(\left\lfloor \frac{n}{i} \right\rfloor)}{g(1)}$$

*/

const uint32_t LIMIT = 10'000'000;
OY::EulerSieve::Sieve<LIMIT, true> sieve;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t p, n;
    std::cin >> p >> n;
    Z::set_mod(p, true);

    auto sum = [&](uint64_t n) -> Z { return Z(1) * n * (n + 1) / 2; };
    auto getH = [&](uint64_t n) -> Z { return sum(n) * sum(n); };
    auto getG = [&](uint64_t n) -> Z { return Z(1) * n * (n + 1) * (2 * n + 1) / 6; };
    auto f = [&](uint64_t n) -> Z { return Z(1) * sieve.get_Euler_Phi(n) * n * n; };

    AA::DU::WeightedPrefix solver(getH, getG, Z(1));
    solver.build_small_by_f(LIMIT, f);

    Z ans = 0;
    for (auto [quot, left, right] : OY::SqrtDecomposition<uint64_t>(n)) {
        ans += sum(quot) * sum(quot) * (solver.query(right) - solver.query(left - 1));
    }

    std::cout << ans << '\n';

    return 0;
}
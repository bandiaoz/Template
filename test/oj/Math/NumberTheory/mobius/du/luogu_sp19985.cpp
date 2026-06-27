#include <bits/stdc++.h>
#include "src/Math/NumberTheory/DuSieve.h"
#include "src/Math/NumberTheory/EulerSieve.h"
#include "src/Math/NumberTheory/SqrtDecomposition.h"

/*
[SP19985 GCDEX2 - GCD Extreme (hard)](https://www.luogu.com.cn/problem/SP19985)
[status](https://www.luogu.com.cn/record/241043687)
*/
/*
$$t(1 \leq t \leq 10^4)$$ 次询问，每次给定 $$n (1 \leq n \leq 235711131719 < 2.4 \times 10^{11})$$，求 $$\sum_{i = 1}^n \sum_{j = i + 1}^n \gcd(i, j)$$

枚举 $$g = (i, j)$$，则有 

$$\sum_{g = 1}^n g \sum_{i = 1}^n \sum_{j = i + 1}^n [(i, j) = g]$$


$$\sum_{g = 1}^n g\sum_{i = 1}^n \sum_{j = 1}^{i - 1} [(i, j) = g]$$


$$\sum_{g = 1}^n g \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{i - 1} [(i, j) = 1]$$


根据欧拉函数定义，有 

$$\sum_{g = 1}^n g \left(\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \varphi(i) - 1\right)$$


其中减一是为了减去 $$i = 1$$ 时的贡献

根据 $$\varphi \ast 1 = \mathrm{id}$$，$$f(x) = \varphi(x)$$ 可以用杜教筛算前缀和，整除分块枚举 $$g$$ 即可
*/

const uint32_t LIMIT = 10'000'000;
OY::EulerSieve::Sieve<LIMIT, true> sieve;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    auto f = [&](uint64_t n) -> uint64_t { return sieve.get_Euler_Phi(n); };
    auto getH = [&](uint64_t n) -> uint64_t { return 1ULL * n * (n + 1) / 2; };
    auto getG = [&](uint64_t n) -> uint64_t { return n; };
    AA::DU::WeightedPrefix solver(getH, getG, 1ULL);
    solver.build_small_by_f(LIMIT, f);

    while (t--) {
        uint64_t n;
        std::cin >> n;
        
        uint64_t ans = 0;
        auto sum = [&](uint64_t n) -> uint64_t { return 1ULL * n * (n + 1) / 2; };
        for (auto [quot, left, right] : OY::SqrtDecomposition<uint64_t>(n)) {
            ans += (sum(right) - sum(left - 1)) * (solver.query(quot) - 1);
        }

        std::cout << ans << '\n';
    }

    return 0;
}
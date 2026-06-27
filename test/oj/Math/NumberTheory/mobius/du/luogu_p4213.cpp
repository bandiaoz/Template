#include <bits/stdc++.h>
#include "src/Math/NumberTheory/DuSieve.h"
#include "src/Math/NumberTheory/EulerSieve.h"
#include "src/Math/NumberTheory/Mobius.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[P4213 【模板】杜教筛](https://www.luogu.com.cn/problem/P4213)
[status](https://www.luogu.com.cn/record/240482202)
*/
/*
给定正整数 $$n(1 \leq n \leq 2^{31}-1)$$，求 $$\sum_{i=1}^n \varphi(i)$$ 和 $$\sum_{i=1}^n \mu(i)$$，一共 $$t(1 \leq t \leq 10)$$ 组询问

对于 $$\sum_{i=1}^n \varphi(i)$$，令 $$f = \varphi$$，$$g = 1$$，则有 $$h = f \ast g = \varphi \ast 1 = \mathrm{id}$$


对于 $$\sum_{i=1}^n \mu(i)$$，令 $$f = \mu$$，$$g = 1$$，则有 $$h = f \ast g = \mu \ast 1 = \varepsilon$$

套用模板即可
*/

const uint32_t LIMIT = 10'000'000;
OY::EulerSieve::Sieve<LIMIT, true> sieve;
OY::MOBIUS::Table<LIMIT, true> mobius;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    auto sum_phi = [&](uint32_t n) -> ll {
        auto f = [&](uint32_t n) -> ll { return sieve.get_Euler_Phi(n); };
        auto getH = [&](uint32_t n) -> ll { return 1LL * n * (n + 1) / 2; };
        auto getG = [&](uint32_t n) -> ll { return n; };
        AA::DU::WeightedPrefix solver(getH, getG, 1LL);
        solver.build_small_by_f(LIMIT, f);
        return solver.query(n);
    };
    auto sum_mu = [&](uint32_t n) -> ll {
        auto f = [&](uint32_t n) -> ll { return mobius.query_mobius(n); };
        auto getH = [&](uint32_t n) -> ll { return n >= 1; };
        auto getG = [&](uint32_t n) -> ll { return n; };
        AA::DU::WeightedPrefix solver(getH, getG, 1LL);
        solver.build_small_by_f(LIMIT, f);
        return solver.query(n);
    };

    while (t--) {
        uint32_t n;
        std::cin >> n;

        std::cout << sum_phi(n) << ' ' << sum_mu(n) << '\n';
    }

    return 0;
}
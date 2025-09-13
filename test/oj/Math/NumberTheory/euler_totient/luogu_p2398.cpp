#include <bits/stdc++.h>
#include "src/Math/NumberTheory/EulerSieve.h"
#include "src/Math/NumberTheory/FastSieve.h"
#include "src/Math/Poly/FastTransform.h"

using ll = long long;

/*
[P2398 GCD SUM](https://www.luogu.com.cn/problem/P2398)
[status(phi)](https://www.luogu.com.cn/record/229169790)
[status(fwt)](https://www.luogu.com.cn/record/233782441)
*/

/*
给定正整数 $$n (1 \leq n \leq 10^5)$$，求 $$\sum_{x = 1}^n \sum_{y = 1}^n \gcd(x, y)$$


解法一：首先枚举 $$g = \gcd(x, y)$$，则答案等于

$$\sum_{x = 1}^n \sum_{y = 1}^n [\gcd(x, y) = g]$$

令 $$l = \left\lfloor \frac{n}{g} \right\rfloor$$，则答案等于

$$\sum_{x = 1}^l \sum_{y = 1}^l [\gcd(x, y) = 1]$$

根据对称性，改变 $$y$$ 的上界（当 $$x = y = 1$$ 时，有一次重复贡献 $$\gcd(x, y) = 1$$），得到

$$\left(\sum_{x = 1}^l \sum_{y = 1}^x 2[\gcd(x, y) = 1]\right) - 1$$

即

$$2\left(\sum_{x = 1}^l \varphi(x)\right) - 1$$


解法二：
令 $$c_k$$ 表示满足 $$\gcd(i, j) = k$$ 的数对 $$(i, j)$$ 的个数，则答案等于 $$\sum_{k = 1}^n k \cdot c_k$$


则 $$c_k = \sum_{(i, j) = k} a_i b_j$$，其中 $$a_i = 1, b_i = 1$$

*/

void solve_phi() {
    int n;
    std::cin >> n;
    
    OY::EulerSieve::Sieve<100'000, true> sieve;
    std::vector<ll> phi_pre(n + 1);
    for (int i = 1; i <= n; i++) {
        phi_pre[i] = phi_pre[i - 1] + sieve.get_Euler_Phi(i);
    }
    
    ll ans = 0;
    for (int g = 1; g <= n; g++) {
        int l = n / g;
        ans += 1LL * g * (2 * phi_pre[l] - 1);
    }
    std::cout << ans << '\n';
}

void solve_fwt() {
    int n;
    std::cin >> n;
    std::vector<uint64_t> a(n + 1, 1);

    OY::FASTSIEVE::Sieve<200'000> sieve;
    auto find_kth_prime = [&](int k) { return sieve.query_kth_prime(k); };
    OY::FASTTRANS::fast_gcd_transform<true>(a.begin(), a.end(), find_kth_prime);
    for (auto &i : a) {
        i *= i;
    }
    OY::FASTTRANS::fast_gcd_transform<false>(a.begin(), a.end(), find_kth_prime);

    uint64_t ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += i * a[i];
    }

    std::cout << ans << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_fwt();
    // solve_phi();

    return 0;
}
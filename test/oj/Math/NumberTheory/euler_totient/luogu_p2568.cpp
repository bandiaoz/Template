#include <bits/stdc++.h>
#include "src/Math/NumberTheory/EulerSieve.h"

using ll = long long;

/*
[P2568 GCD](https://www.luogu.com.cn/problem/P2568)
[status](https://www.luogu.com.cn/record/229164518)
*/
/*
给定正整数 $$n (1 \leq n \leq 10^7)$$，求 $$1 \leq x, y \leq n$$ 且 $$\gcd(x, y)$$ 为质数的数对 $$(x, y)$$ 的个数

首先枚举质数 $$p$$，此时答案等于

$$\sum_{x = 1}^n \sum_{y = 1}^n [\gcd(x, y) = p]$$

令 $$l = \left\lfloor \frac{n}{p} \right\rfloor$$，则答案等于

$$\sum_{x = 1}^l \sum_{y = 1}^l [\gcd(x, y) = 1]$$

根据对称性，改变 $$y$$ 的上界（当 $$x = y = 1$$ 时，有一次重复贡献 $$\gcd(x, y) = 1$$），得到

$$\left(\sum_{x = 1}^l \sum_{y = 1}^x 2[\gcd(x, y) = 1]\right) - 1$$

即 

$$2\left(\sum_{x = 1}^l \varphi(x)\right) - 1$$

*/

OY::EulerSieve::Sieve<10'000'000, true> sieve;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<ll> phi_pre(n + 1);
    for (int i = 1; i <= n; i++) {
        phi_pre[i] = phi_pre[i - 1] + sieve.get_Euler_Phi(i);
    }

    ll ans = 0;
    for (int i = 0; i < sieve.count() && sieve.query_kth_prime(i) <= n; i++) {
        int p = sieve.query_kth_prime(i);
        int l = n / p;
        ans += 2 * phi_pre[l] - 1;
    }
    std::cout << ans << '\n';

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/EulerSieve.h"

using ll = long long;

/*
[炫酷反演魔术](https://ac.nowcoder.com/acm/problem/244326)
[status](https://ac.nowcoder.com/acm/contest/view-submission?submissionId=78879482)
*/
/*
给定长度为 $$n(1 \leq n \leq 3 \times 10^5)$$ 的数组 $$a(1 \leq a_i \leq n)$$，求 $$\sum_{i = 1}^n\sum_{j = 1}^n \varphi(\gcd(a_i, a_j^3))$$

枚举 $$g = \gcd(a_i, a_j^3)$$，则有

$$\sum_{g = 1}^n \varphi(g) \sum_{i = 1}^n \sum_{j = 1}^n [(a_i, a_j^3) = g]$$


$$\sum_{g = 1}^n \varphi(g) \sum_{g \mid a_i} \sum_{g \mid a_j^3} [(\frac{a_i}{g}, \frac{a_j^3}{g}) = 1]$$

利用反演公式，有

$$\sum_{g = 1}^n \varphi(g) \sum_{g \mid a_i} \sum_{g \mid a_j^3} \sum_{d \mid (\frac{a_i}{g}, \frac{a_j^3}{g})} \mu(d)$$

将 $$d$$ 放到前面，有

$$\sum_{g = 1}^n \varphi(g) \sum_{d = 1}^n \mu(d) \sum_{i = 1}^n[gd \mid a_i] \sum_{j = 1}^n[gd \mid a_j^3]$$


令 $$t = gd$$，则有 

$$\sum_{t = 1}^n \sum_{g \mid t} \varphi(g) \mu(\frac{t}{g}) \sum_{i = 1}^n[t \mid a_i] \sum_{j = 1}^n[t \mid a_j^3]$$


令 $$f(t) = \sum_{g \mid t} \varphi(g) \mu(\frac{t}{g})$$，注意这里 $$f(t)$$ 是两个积性函数的 Dirichlet 卷积，所以 $$f(t)$$ 也是积性函数，可以 $$O(n)$$ 预处理。

令 $$c_1(t) = \sum_{i = 1}^n[t \mid a_i]$$，$$c_3(t) = \sum_{i = 1}^n[t \mid a_i^3]$$，则有

$$\sum_{t = 1}^n f(t)\ c_1(t)\ c_3(t)$$

其中

$$c_1(t) = \sum_{i = 1}^n[t \mid a_i] = \sum_{t \mid d} cnt[d]$$

$$cnt[d]$$ 表示 $$d$$ 在数组 $$a$$ 中出现的次数，故 $$c_1(t)$$ 可以使用 Dirichlet 后缀和处理。

$$c_3(t) = \sum_{i = 1}^n[t \mid a_i^3]$$

将 $$t$$ 唯一分解，得到 

$$c_3(t) = \sum_{i = 1}^n \left[\left(\prod_{j = 1}^k p_j^{c_j}\right) \mid a_i^3\right] = \sum_{i = 1}^n \left[\left(\prod_{j = 1}^k p_j^{\left\lceil \frac{c_j}{3} \right\rceil}\right) \mid a_i\right] = \sum_{get(t) \mid d} cnt[d]$$

故 $$c_3(t) = c_1(get(t))$$，其中 $$get(t)$$ 表示 $$t$$ 唯一分解后，每个质因子的指数都除以 $$3$$ 并向上取整。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    const int N = 300000;
    OY::EulerSieve::Sieve<N, true, true> sieve;
    OY::MOBIUS::Table<N * 2> mobius;
    
    auto calc_prime = [&](int p) { return p - 2; };
    auto calc_prime_pow = [&](int p, int c, int low) { return sieve.get_Euler_Phi(low * p) - sieve.get_Euler_Phi(low); };
    auto f = OY::MOBIUS::Multiplicative<N>().solve<ll>(n, calc_prime, calc_prime_pow);

    std::vector<int> cnt(n + 1);
    for (auto i : a) {
        cnt[i]++;
    }
    auto c1 = cnt;
    OY::MOBIUS::partial_sum_Dirichlet_multiple(n, c1.data(), [&](int k) { return mobius.query_kth_prime(k); });

    std::vector<int> c3(n + 1);
    auto get = [&](int x) {
        int res = 1;
        for (auto [p, c] : sieve.decomposite(x)) {
            c = (c + 2) / 3;
            for (int i = 0; i < c; i++) {
                res *= p;
            }
        }
        return res;
    };
    for (int i = 1; i <= n; i++) {
        c3[i] = c1[get(i)];
    }

    ll ans = 0;
    for (int t = 1; t <= n; t++) {
        ans += 1LL * f[t] * c1[t] * c3[t];
    }

    std::cout << ans << '\n';

    return 0;
}
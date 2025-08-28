#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"

using ll = long long;

/*
[P3911 最小公倍数之和](https://www.luogu.com.cn/problem/P3911)
[status](https://www.luogu.com.cn/record/233542388)
*/
/*
给定长度为 $$n(1 \leq n \leq 5 \times 10^4)$$ 的数组 $$a$$，求 $$\sum_{i=1}^n\sum_{j=1}^n \mathrm{lcm}(a_i,a_j)$$
$$1 \leq a_i \leq 5\times 10^4$$

值域为 $$N = 5 \times 10^4$$，$$c_i$$ 表示数组 $$a$$ 中值为 $$i$$ 的元素个数，则

$$\sum_{i = 1}^N \sum_{j = 1}^N \frac{ij}{(i, j)} c_i c_j$$

枚举 $$g = (i ,j)$$，则有

$$\sum_{g = 1}^N \sum_{i = 1}^N\sum_{j = 1}^N \frac{ij}{g} c_i c_j [(i, j) = g]$$


$$\sum_{g = 1}^{N} \sum_{i = 1}^{\left\lfloor \frac{N}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{N}{g} \right\rfloor} ijg c_{ig} c_{jg} [(i, j) = 1]$$


根据反演公式，有 

$$\sum_{g = 1} \sum_{i = 1}^{\left\lfloor \frac{N}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{N}{g} \right\rfloor} ijg c_{ig} c_{jg} \sum_{d \mid (i, j)} \mu(d)$$


枚举 $$d$$，则有

$$\sum_{g = 1}^N g \sum_{d = 1}^{\left\lfloor \frac{N}{g} \right\rfloor} \mu(d) d^2 \sum_{i = 1}^{\left\lfloor \frac{N}{dg} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{N}{dg} \right\rfloor} ij c_{igd} c_{jgd}$$


整理得 

$$\sum_{g = 1}^N g \sum_{d = 1}^{\left\lfloor \frac{N}{g} \right\rfloor} \mu(d) d^2 \left(\sum_{i = 1}^{\left\lfloor \frac{N}{dg} \right\rfloor} i c_{igd}\right)^2 $$


令 $$f(gd) = \sum_{i = 1}^{\left\lfloor \frac{N}{dg} \right\rfloor} i c_{igd}$$，则有


$$\sum_{g = 1}^N g \sum_{d = 1}^{\left\lfloor \frac{N}{g} \right\rfloor} \mu(d) d^2 f(dg)^2$$


令 $$t = gd$$，则有 

$$\sum_{t = 1}^N tf(t)^2 \sum_{d \mid t} d \mu(d)$$


其中 $$h(t) = \sum_{d \mid t} d \mu(d)$$ 可以预处理
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
    const int N = 50000;
    std::vector<int> c(N + 1);
    for (auto i : a) {
        c[i]++;
    }
    
    OY::MOBIUS::Table<N> mobius;

    std::vector<int> h(N + 1);
    for (int d = 1; d <= N; d++) {
        for (int t = d; t <= N; t += d) {
            h[t] += d * mobius.query_mobius(d);
        }
    }

    std::vector<ll> f(N + 1);
    for (int t = 1; t <= N; t++) {
        for (int i = 1; i <= N / t; i++) {
            f[t] += i * c[i * t];
        }
    }

    ll ans = 0;
    for (int t = 1; t <= N; t++) {
        ans += t * f[t] * f[t] * h[t];
    }

    std::cout << ans << '\n';

    return 0;
}
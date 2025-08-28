#include <bits/stdc++.h>
#include "src/DataStruct/AdjDiff.h"
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/SqrtDecomposition.h"

/*
[P6222 「P6156 简单题」加强版](https://www.luogu.com.cn/problem/P6222)
[status](https://www.luogu.com.cn/record/233569081)
*/
/*
$$T$$ 组询问，给定一个常数 $$K$$。每次询问单独给定 $$n$$，求 $$\sum_{i=1}^{n}\sum_{j=1}^{n} (i+j)^K \gcd(i,j) \mu^2(\gcd(i,j)) \pmod {2^{32}}$$

$$T = 10^4, 1 \leq K < 2^{31}, n \leq 10^7$$。

$$\sum_{i=1}^{n}\sum_{j=1}^{n} (i+j)^K \gcd(i,j) \mu^2(\gcd(i,j))$$

枚举 $$g = (i, j)$$，则有 

$$\sum_{g = 1}^n \sum_{i = 1}^n\sum_{j = 1}^n (i+j)^K g \mu^2(g) [(i, j) = g]$$


$$\sum_{g = 1}^n\sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor}\sum_{j = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} (i+j)^K g^{K + 1}\mu^2(g) [(i, j) = 1]$$


根据莫比乌斯反演，有 

$$\sum_{g = 1}^n g^{K + 1}\mu^2(g) \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor}\sum_{j = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} (i+j)^K \sum_{d \mid (i, j)} \mu(d)$$


将 $$d$$ 提到前面，则有 

$$\sum_{g = 1}^n g^{K + 1}\mu^2(g) \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) d^K \sum_{i = 1}^{\left\lfloor \frac{n}{dg} \right\rfloor}\sum_{j = 1}^{\left\lfloor \frac{n}{dg} \right\rfloor} (i+j)^K$$


令 $$t = gd$$，则有

$$\sum_{t = 1}^n t^K \sum_{g \mid t} g \mu^2(g) \mu(\frac{t}{g}) \sum_{i = 1}^{\left\lfloor \frac{n}{t} \right\rfloor}\sum_{j = 1}^{\left\lfloor \frac{n}{t} \right\rfloor} (i+j)^K$$


令 $$f(x) = \sum_{i = 1}^{x}\sum_{j = 1}^{x} (i+j)^K$$，$$h(t) = \sum_{g \mid t} g \mu^2(g) \mu(\frac{t}{g})$$，则有

$$\sum_{t = 1}^n t^K h(t) f\left(\left\lfloor \frac{n}{t} \right\rfloor\right)$$


其中 $$f(x)$$ 可以递推处理，$$h(t)$$ 是积性函数，线性筛处理
*/

constexpr static uint32_t power(uint32_t a, uint32_t b) {
    uint32_t res = 1;
    while (b) {
        if (b & 1) {
            res *= a;
        }
        b >>= 1;
        a *= a;
    }
    return res;
}

constexpr int N = 1e7;
struct node {
    uint32_t pow, h;
    node() = default;
    node(int) : pow(1), h(1) {}
    node(uint32_t pow, uint32_t h) : pow(pow), h(h) {}
    node operator*(const node &rhs) const {
        return {pow * rhs.pow, h * rhs.h};
    }
} pow_h[2 * N + 1];

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t t, MAX, K;
    std::cin >> t >> MAX >> K;

    OY::MOBIUS::Table<N> mobius;

    auto calc_prime = [&](uint32_t p) { 
        return node(power(p, K), p - 1); 
    };
    auto calc_prime_pow = [&](uint32_t p, uint32_t c, uint32_t low) { 
        return node(
            pow_h[low].pow * pow_h[p].pow, 
            p * mobius.query_mobius(low)
        );
    };
    OY::MOBIUS::Multiplicative<2 * N>().solve(2 * MAX, pow_h, calc_prime, calc_prime_pow);
    OY::AdjSumTable<uint32_t> table(2 * MAX + 1, [&](uint32_t t) { return pow_h[t].pow; });
    std::vector<uint32_t> f(MAX + 1);
    for (uint32_t x = 1; x <= MAX; x++) {
        f[x] = f[x - 1] + 2 * table.query(1 + x, 2 * x + 1) - pow_h[2 * x].pow;
    }

    OY::AdjSumTable<uint32_t> pre(MAX + 1, [&](uint32_t t) { return pow_h[t].pow * pow_h[t].h; });

    while (t--) {
        uint32_t n;
        std::cin >> n;

        uint32_t ans = 0;
        for (auto range : OY::SqrtDecomposition<uint32_t>(n)) {
            uint32_t l = range.left(), r = range.right();
            uint32_t q = range.quot();
            ans += pre.query(l, r + 1) * f[q];
        }
        std::cout << ans << '\n';
    }   

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"
#include "src/DataStruct/Fenwick/MonoBIT.h"

/*
[P3312 [SDOI2014] 数表](https://www.luogu.com.cn/problem/P3312)
[status](https://www.luogu.com.cn/record/230891835)
*/
/*
给定 $$n, m, a$$，求 $$\sum_{i = 1}^n \sum_{j = 1}^m \sigma(\gcd(i, j)) [\sigma(\gcd(i, j)) \leq a]$$

多测 $$t \leq 2 \times 10^4, 1 \leq n, m \leq 10^5, 1 \leq |a| \leq 10^9$$

先考虑求 $$\sum_{i = 1}^n \sum_{j = 1}^m \sigma(\gcd(i, j))$$

枚举 $$g = \gcd(i, j)$$，则

$$\sum_{g = 1}^n \sum_{i = 1}^n \sum_{j = 1}^m \sigma(g)[(i, j) = g]$$


$$\sum_{g = 1}^n \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} \sigma(g) [(i, j) = 1]$$


根据反演公式有：

$$\sum_{g = 1}^n \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} \sigma(g) \sum_{d \mid \gcd(i, j)} \mu(d)$$


将 $$d$$ 提到前面，则

$$\sum_{g = 1}^n \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \sigma(g) \left\lfloor \frac{n}{dg} \right\rfloor \left\lfloor \frac{m}{dg} \right\rfloor$$


令 $$t = dg$$，则

$$\sum_{g = 1}^n \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \sigma(g) \left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor$$


$$\sum_{t = 1}^n \left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor \sum_{g \mid t} \mu\left(\frac{t}{g}\right) \sigma(g)$$


后面部分可以预处理，$$f(t) = \sum_{g \mid t} \mu\left(\frac{t}{g}\right) \sigma(g)$$

再看限制 $$\sigma(\gcd(i, j)) \leq a$$，则只有 $$\sigma(g) \leq a$$ 的 $$f(t)$$ 有贡献，于是可以离线处理
*/

const int N = 100000;
struct node {
    int mu, sigma;
    node() = default;
    node(int) : mu(1), sigma(1) {}
    node(int m, int s) : mu(m), sigma(s) {}
    node operator*(const node &rhs) const {
        return {mu * rhs.mu, sigma * rhs.sigma}; 
    }
} mu_sigma[N + 1];

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;
    std::vector<std::array<int, 4>> queries(q);
    for (int i = 0; i < q; i++) {
        int n, m, a;
        std::cin >> n >> m >> a;
        queries[i] = {n, m, a, i};
    }

    std::sort(queries.begin(), queries.end(), [&](const auto &lhs, const auto &rhs) {
        return lhs[2] < rhs[2];
    });

    auto calc_prime = [&](int p) -> node { return {-1, p + 1}; };
    auto calc_prime_pow = [&](int p, int c, int low) -> node { return {0, mu_sigma[low].sigma + low * p}; };
    OY::MOBIUS::Multiplicative<N>().solve(N, mu_sigma, calc_prime, calc_prime_pow);

    std::vector<std::pair<int, int>> sigma_order(N);
    for (int i = 1; i <= N; i++) {
        sigma_order[i - 1] = {mu_sigma[i].sigma, i};
    }
    std::sort(sigma_order.begin(), sigma_order.end());

    OY::MonoSumBIT<uint32_t> fen(N + 1);
    std::vector<uint32_t> ans(q);
    int cur = 0;
    for (auto [n, m, a, idx] : queries) {
        while (cur < N && sigma_order[cur].first <= a) {
            auto [sigma, g] = sigma_order[cur++];
            for (int t = g; t <= N; t += g) {
                fen.add(t, mu_sigma[t / g].mu * sigma);
            }
        }
        for (auto range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            int l = range.left(), r = range.right();
            uint32_t q1 = range.quot1(), q2 = range.quot2();
            ans[idx] += q1 * q2 * fen.query(l, r + 1);
        }
    }

    for (auto res : ans) {
        std::cout << (res & 0x7fffffff) << "\n";
    }

    return 0;
}
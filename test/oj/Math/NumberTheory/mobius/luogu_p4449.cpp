#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/PairSqrtDecomposition.h"
#include "src/DataStruct/AdjDiff.h"

using Z = OY::mint1000000007;

/*
[P4449 于神之怒加强版](https://www.luogu.com.cn/problem/P4449)
[status](https://www.luogu.com.cn/record/240838349)
*/
/*
给定 $$n,m,k$$，求 $$\sum_{i=1}^n \sum_{j=1}^m \gcd(i,j)^k$$

$$1 \leq T \leq 2 \times 10^3$$，$$1 \leq n, m, k \leq 5 \times 10^6$$


$$\sum_{i=1}^n \sum_{j=1}^m (i,j)^k$$

枚举 $$g = (i,j)$$，则有

$$\sum_{g=1}^n g^k \sum_{i=1}^n \sum_{j=1}^m [(i,j)=g]$$


$$\sum_{g=1}^n g^k \sum_{i=1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j=1}^{\left\lfloor \frac{m}{g} \right\rfloor} [(i,j)=1]$$


根据反演公式，有

$$\sum_{g = 1}^n g^k \sum_{i = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \sum_{j = 1}^{\left\lfloor \frac{m}{g} \right\rfloor} \sum_{d \mid \gcd(i, j)} \mu(d)$$


枚举 $$d$$，有

$$\sum_{g = 1}^n g^k \sum_{d = 1}^{\left\lfloor \frac{n}{g} \right\rfloor} \mu(d) \left\lfloor \frac{n}{dg} \right\rfloor \left\lfloor \frac{m}{dg} \right\rfloor$$


令 $$t = dg$$，则有

$$\sum_{t = 1}^n  \left\lfloor \frac{n}{t} \right\rfloor \left\lfloor \frac{m}{t} \right\rfloor \sum_{g \mid t} g^k \mu\left(\frac{t}{g}\right)$$


令 $$f(t) = \sum_{g \mid t} g^k \mu\left(\frac{t}{g}\right)$$，可以预处理

*/

const int N = 5'000'000;
OY::MOBIUS::Table<N> mobius;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t, k;
    std::cin >> t >> k;

    auto calc_f = [&](int t, int g) {
        return Z(g).pow(k) * mobius.query_mobius(t / g);
    };
    auto calc_prime = [&](int p) {
        return calc_f(p, 1) + calc_f(p, p);
    };
    auto calc_prime_pow = [&](int p, int c, int low) {
        int t = p * low;
        return calc_f(t, low) + calc_f(t, t);
    };
    std::vector<Z> f = OY::MOBIUS::Multiplicative<N>().solve<Z>(N, calc_prime, calc_prime_pow);
    OY::AdjSumTable<Z> pre(f.begin(), f.end());

    while (t--) {
        int n, m;
        std::cin >> n >> m;

        Z ans = 0;
        for (auto range : OY::PairSqrtDecomposition<uint32_t>(n, m)) {
            int l = range.left(), r = range.right();
            int q1 = range.quot1(), q2 = range.quot2();
            ans += Z(1) * q1 * q2 * pre.query(l, r + 1);
        }
        std::cout << ans << '\n';
    }

    return 0;
}
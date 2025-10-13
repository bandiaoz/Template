#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/Poly/LagrangeInterpolation.h"

using Z = OY::mint1000000007;

/*
[CF622F The Sum of the k-th Powers](https://codeforces.com/contest/622/problem/F)
[status](https://codeforces.com/contest/622/submission/338924168)
*/
/*
给定 $$n$$ 和 $$k$$，求 $$1^k + 2^k + \cdots + n^k$$ 的值。
$$1 \leq n \leq 10^9, 1 \leq k \leq 10^6$$

$$f(n) = \sum_{i = 1}^n i^k$$ 是一个 $$k + 1$$ 次多项式，因此可以拉格朗日插值求出。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;

    OY::LagrangeInterpolation<Z> li(k + 3);
    Z pref = 0;
    for (int i = 1; i <= k + 2; i++) {
        pref += Z(i).pow(k);
        li.addPoint(i, pref);
    }

    li.prepareIfSpecialX();

    std::cout << li.calc(n) << '\n';

    return 0;
}
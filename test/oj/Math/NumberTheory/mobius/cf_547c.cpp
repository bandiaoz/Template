#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"
#include "src/Math/NumberTheory/EulerSieve.h"

using ll = long long;

/*
[CF547C Mike and Foam](https://codeforces.com/contest/547/problem/C)
[status](https://codeforces.com/contest/547/submission/340856669)
*/
/*
给定数组 $$a = \{a_1, a_2, \dots, a_n\}$$，有 $$q$$ 次操作，每次将 $$a_i$$ 加入或从集合中删除，问集合中有多少个数对 $$(i, j)$$ 满足 $$\gcd(a_i, a_j) = 1$$。
$$1 \leq n, q \leq 2 \times 10^5, 1 \leq a_i \leq 5 \times 10^5$$

答案为 $$\frac{tmp - c_1}{2}$$，其中 $$tmp = \sum_{i = 1}^N\sum_{j = 1}^N[(i, j) = 1]c_ic_j$$，$$c_i$$ 为当前集合中 $$i$$ 的出现次数。

接下来求 $$tmp$$:

由莫比乌斯反演，我们有：

$$\sum_{i = 1}^N\sum_{j = 1}^N\sum_{d \mid \gcd(i, j)} \mu(d)c_ic_j$$

将 $d$ 提到前面，得到：

$$\sum_{d = 1}^N \mu(d) \sum_{i = 1}^{\lfloor \frac{N}{d} \rfloor} \sum_{j = 1}^{\lfloor \frac{N}{d} \rfloor} c_{id}c_{jd}$$


令 $$f(d) = \sum_{i = 1}^{\lfloor \frac{N}{d} \rfloor} c_{id}$$，则有：

$$\sum_{d = 1}^N \mu(d) f(d)^2$$


每次操作 $$a_p$$，会影响所有的因子 $$d$$，暴力维护即可
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    
    const int N = 500'000;
    std::vector<int> cnt(N + 1);
    std::vector<ll> f(N + 1);

    OY::MOBIUS::Table<N> mobius;
    OY::EulerSieve::Sieve<N, false, true, true> sieve;
    ll ans = 0;
    std::vector<int> active(n);
    while (q--) {
        int p;
        std::cin >> p;
        p--;

        int x = a[p];
        sieve.enumerate_factors(x, [&](int d) {
            ans -= mobius.query_mobius(d) * f[d] * f[d];
            f[d] -= cnt[x];
            cnt[x] += active[p] ? -1 : 1;
            f[d] += cnt[x];
            ans += mobius.query_mobius(d) * f[d] * f[d];
        });
        active[p] ^= 1;

        std::cout << (ans - cnt[1]) / 2 << '\n';
    }

    return 0;
}
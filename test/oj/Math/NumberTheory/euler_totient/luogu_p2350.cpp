#include <bits/stdc++.h>
#include "src/Math/NumberTheory/EulerSieve.h"

using ll = long long;

/*
[P2350 HAOI2012 外星人](https://www.luogu.com.cn/problem/P2350)
[status](https://www.luogu.com.cn/record/229388390)
*/
/*
给定正整数 $$n = \prod_{i = 1}^m p_i^{c_i}$$，求最小的 $$x$$，使得 $$\varphi^x(n) = 1$$，其中 $$\varphi^x(N)$$ 表示 $$\varphi$$ 函数对 $$N$$ 进行 $$x$$ 次迭代的结果。

数据范围：$$T \leq 50, 1 \leq p_i \leq 10^5, 1 \leq c_i \leq 10^9, m \leq 2000$$

只有 $$n = 1$$ 或 $$n = 2$$ 时，$$\varphi(n) = 1$$

$$\varphi\left(\prod p_c^{c_i}\right) = \prod (p_i - 1) \times p_i^{c_i - 1}$$

每一个质数 $$p_i$$ 一定会先变成 2，最后变成 1，一次变换最多有一个 $$p_i = 2$$ 变成 1，因此对于每一个质因子处理出能变出多少个 2 即可。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const int N = 1e5;
    std::vector<int> f(N + 1);
    f[2] = 1;
    OY::EulerSieve::Sieve<N, false, true, false> sieve;
    for (int i = 3; i <= N; i++) {
        if (sieve.is_prime(i)) {
            f[i] = f[i - 1];
        } else {
            int p = sieve.query_smallest_factor(i);
            f[i] = f[i / p] + f[p];
        }
    }

    int t;
    std::cin >> t;
    while (t--) {
        int m;
        std::cin >> m;

        ll ans = 0;
        bool odd = true;
        for (int i = 0; i < m; i++) {
            int p, c;
            std::cin >> p >> c;
            ans += f[p] * c;
            if (p % 2 == 0) {
                odd = false;
            }
        }

        if (odd) {
            std::cout << ans + 1 << '\n';
        } else {
            std::cout << ans << '\n';
        }
    }

    return 0;
}
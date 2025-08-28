#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"

using ll = long long;

/*
[P2714 四元组统计](https://www.luogu.com.cn/problem/P2714)
[status](https://www.luogu.com.cn/record/233595287)
*/
/*
给定长度为 $$n(1 \leq n \leq 10^4)$$ 的数组 $$a(1 \leq a_i \leq 10^4)$$，求有多少个四元组满足 $$\gcd(a _ i, a _ j, a _ k, a _ l) = 1$$。
多测 $$T \leq 100$$

$$c_i$$ 表示数组中等于 $$i$$ 的元素个数

$$f(x)$$ 表示数组中满足 $$(a_i, a_j, a_k, a_l) = x$$ 的四元组个数，所求即为 $$f(1)$$

$$g(x)$$ 表示数组中满足 $$x \mid (a_i, a_j, a_k, a_l)$$ 的四元组个数，可以通过 $$g(x) = \binom{\sum_{x \mid d} c_d}{4}$$ 计算

并且有 $$g(x) = \sum_{x \mid d} f(d)$$，根据莫比乌斯反演，有 $$f(x) = \sum_{x \mid d} \mu\left(\frac{d}{x}\right) g(d)$$

带入 $$x = 1$$，则有

$$f(1) = \sum_{d = 1}^N \mu(d) g(d)$$

*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    while (std::cin >> n) {
        const int N = 10000;
        std::vector<int> c(N + 1);
        for (int i = 0; i < n; i++) {
            int x;
            std::cin >> x;
            c[x]++;
        }
        
        OY::MOBIUS::Table<N> mobius;
        std::vector<ll> g(N + 1);
        for (int i = 1; i <= N; i++) {
            int cnt = 0;
            for (int d = i; d <= N; d += i) {
                cnt += c[d];
            }
            g[i] = 1LL * cnt * (cnt - 1) * (cnt - 2) * (cnt - 3) / 24;
        }
        
        ll ans = 0;
        for (int i = 1; i <= N; i++) {
            ans += mobius.query_mobius(i) * g[i];
        }
        
        std::cout << ans << '\n';
    }

    return 0;
}
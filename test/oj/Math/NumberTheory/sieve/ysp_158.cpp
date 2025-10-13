#include <bits/stdc++.h>
#include "src/Math/NumberTheory/FastSieve.h"

/*
[Enumerate Primes](https://judge.yosupo.jp/problem/enumerate_primes)(https://github.com/yosupo06/library-checker-problems/issues/158)
[status](https://judge.yosupo.jp/submission/319158)
*/
/*
给定正整数 $$n$$，$$\pi(n)$$ 表示小于等于 $$n$$ 的质数个数
输出 $$\pi(n)$$ 和 第 $$a \times i + b$$ 个质数
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint32_t n, a, b;
    std::cin >> n >> a >> b;    
    
    const int N = 500'000'000;
    OY::FASTSIEVE::Sieve<N> sieve(n);

    int pi = sieve.count();
    std::vector<int> ans;
    for (int i = b; i < pi; i += a) {
        ans.push_back(sieve.query_kth_prime(i));
    }

    std::cout << pi << " " << ans.size() << "\n";
    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i] << " \n"[i == ans.size() - 1];
    }

    return 0;
}
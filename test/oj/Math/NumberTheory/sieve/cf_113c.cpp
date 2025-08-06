#include <bits/stdc++.h>
#include "src/Math/NumberTheory/FastSieve.h"

/*
[C. Double Happiness](https://codeforces.com/contest/113/problem/C)
[status](https://codeforces.com/contest/113/submission/326637530)
*/
/**
 * 查询 [l, r] 范围内，满足可以被表示为两数平方和 $$a^2 + b^2$$ 的质数个数
 * 
 * 根据费马平方和定理，质数 $p$ 可以被表示为两数平方和，当且仅当 $$p \equiv 1 \pmod 4$$ 或 $$p = 2$$
 * 使用质数筛，将 [l, r] 范围内的质数筛出来，然后统计满足条件的质数个数
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int l, r;
    std::cin >> l >> r;

    OY::FASTSIEVE::Sieve<300'000'000> sieve;
    auto is_prime = sieve.to_bitset();

    int ans = 0;
    if (l <= 2 && 2 <= r) ans++;
    while (l % 4 != 1) l++;
    for (int i = l; i <= r; i += 4) {
        if (is_prime[i]) {
            ans++;
        }
    }

    std::cout << ans << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/NumberTheory/Mobius.h"

/*
[P5495 【模板】Dirichlet 前缀和](https://www.luogu.com.cn/problem/P5495)
[status](https://www.luogu.com.cn/record/231096115)
*/
/*
给定长度为 $$n$$ 的数组 $$a$$，求出数组 $$b$$，其中 $$b_i = \sum_{d \mid i} a_d$$
*/

uint32_t seed;
inline uint32_t getnext() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n >> seed;
    std::vector<uint32_t> a(n + 1);
    for (int i = 1; i <= n; i++) {
        a[i] = getnext();
    }

    const int N = 2e7;
    OY::MOBIUS::Table<N> mobius;
    OY::MOBIUS::partial_sum_Dirichlet_divisor(n, a.data(), [&](uint32_t k) { return mobius.query_kth_prime(k); });
    
    std::cout << std::accumulate(a.begin(), a.end(), 0u, std::bit_xor<>{}) << '\n';

    return 0;
}
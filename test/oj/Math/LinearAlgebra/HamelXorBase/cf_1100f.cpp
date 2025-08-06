#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBaseTable.h"

/*
[CF1100F Ivan and Burgers](https://codeforces.com/contest/1100/problem/F)
[status](https://codeforces.com/contest/1100/submission/311622211)
*/
/**
 * 给定长度为 n 的数组 a，有 q 次询问
 * 每次询问区间 [l, r] 中选若干数，异或和最大是多少
 * 
 * 使用前缀异或线性基，枚举区间 [l, r) 中所有基向量，查询最大异或和，复杂度为 $$O(\omega)$$
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::StaticHamelXorBaseTable32<20> hxb(n, [](...) {
        int x;
        std::cin >> x;
        return x;
    });

    int q;
    std::cin >> q;
    while (q--) {
        int l, r;
        std::cin >> l >> r;
        l--;
        std::cout << hxb.query_max_bitxor(l, r) << '\n';
    }

    return 0;
}
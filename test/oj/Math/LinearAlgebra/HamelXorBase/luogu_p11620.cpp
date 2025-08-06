#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBaseZkwTree.h"

/*
[P11620 [Ynoi Easy Round 2025] TEST_34](https://www.luogu.com.cn/problem/P11620)
[status](https://www.luogu.com.cn/record/208211781)
*/
/**
 * 有一个长度为 n 的数组，有 q 次操作
 * 1. 将区间 [l, r] 内的数异或上 x
 * 2. 查询区间 [l, r] 内的数，任选若干个数，使得这些数的异或和最大，输出最大异或和
 * 
 * 使用 HamelXorBaseZkwTree 维护，区间异或复杂度为 $$O(\log n \cdot \omega^2)$$，查询复杂度为 $$O(\log n \cdot \omega^2)$$
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    auto read = [&](...) {
        int x;
        std::cin >> x;
        return x;
    };
    OY::StaticHamelXorBaseZkwTree32<30> hxb(n, read);
    while (q--) {
        int op;
        std::cin >> op;
        if (op == 1) {
            int l, r, x;
            std::cin >> l >> r >> x;
            l--;
            hxb.range_xor(l, r, x);
        } else {
            int l, r, v;
            std::cin >> l >> r >> v;
            l--;
            std::cout << hxb.query_max_bitxor(l, r, v) << '\n';
        }
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/DataStruct/Fenwick/KBIT.h"

// 111546435 是 3~25 之间所有素数的 lcm，乘个 9 让模数大于 1e9
using Z = OY::StaticModInt32<111546435 * 9, false>;

/*
[牛牛的等差数列](https://ac.nowcoder.com/acm/problem/204829)
[status](https://ac.nowcoder.com/acm/contest/view-submission?submissionId=75908531&returnHomeType=1&uid=966251048)
*/
/**
 * 给定一个长度为 n 的数组，有 q 次操作：
 * 1. 对区间 [l, r] 加一个首项为 a，公差为 d 的等差数列
 * 2. 询问 [l, r] 的区间和，对 m 取模，m 是 [3, 25] 之间的素数
 * 
 * 维护二阶差分数组，操作一等价于对区间加上 (i - l)d + a0 = (a0 - d * l) + d * i，系数为 {a0 - d * l, d}
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<Z> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    std::adjacent_difference(a.begin(), a.end(), a.begin());
    std::adjacent_difference(a.begin(), a.end(), a.begin());

    using KBIT = OY::KBIT::Tree<Z, 3>;
    KBIT::prepare();
    KBIT fen(a.begin(), a.end());

    int q;
    std::cin >> q;
    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            int l, r;
            Z a0, d;
            std::cin >> l >> r >> a0 >> d;
            l--;
            fen.add<2, 2>(l, r, {a0 - d * l, d});
        } else {
            int l, r, m;
            std::cin >> l >> r >> m;
            l--;
            auto res = fen.query<2>(l, r).val();
            std::cout << res % m << "\n";
        }
    }

    return 0;
}
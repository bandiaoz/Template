#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/DataStruct/Fenwick/KBIT.h"

using Z = OY::mint1000000007;

/*
[智乃酱的平方数列](https://ac.nowcoder.com/acm/problem/226172)
[status](https://ac.nowcoder.com/acm/contest/view-submission?submissionId=75908785&returnHomeType=1&uid=966251048)
*/
/**
 * 对于一个初始全为 0 的数组，有 q 次操作
 * 1. 将区间 [l, r] 加平方数列，即 a[l] += 1, a[l+1] += 4, a[l+2] += 9, ...
 * 2. 询问区间 [l, r] 的和
 * 
 * 维护三阶差分数组，操作一等价于对区间加上 (i - l + 1)^2 = i^2 + 2(1 - l)i + (1 - l)^2，系数为 {(1 - l)^2, 2(1 - l), 1}
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    
    using KBIT = OY::KBIT::Tree<Z, 4>;
    KBIT::prepare();
    KBIT fen(n);

    while (q--) {
        int op, l, r;
        std::cin >> op >> l >> r;
        l--;
        if (op == 1) {
            fen.add<3, 3>(l, r, {
                Z(1 - l) * (1 - l),
                Z(2) * (1 - l),
                1
            });
        } else {
            std::cout << fen.query<3>(l, r) << "\n";
        }
    }

    return 0;
}
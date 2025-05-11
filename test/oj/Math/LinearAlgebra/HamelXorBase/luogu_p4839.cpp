#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"
#include "src/DataStruct/Segtree/ZkwTree.h"

/*
[P4839 P 哥的桶](https://www.luogu.com.cn/problem/P4839)
[status](https://www.luogu.com.cn/record/208211661)
*/
/**
 * 有 n 个桶，有 q 次操作：
 * 1. 在第 k 个桶加入价值为 x 的球
 * 2. 查询 [l, r] 桶内，拿出若干球，组成的最大异或和
 * 
 * 用线段树维护线性基，线性基合并复杂度为 O(\omega^2)
 * 单点修改复杂度为 O(\log n \omega)，查询复杂度为 O(\log n \omega^2)
 */

using Info = OY::StaticHamelXorBase32<31>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, _;
    std::cin >> n >> _;

    auto zkw = OY::make_ZkwTree<Info>(n, std::plus<Info>());
    int q = n;
    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            int p, x;
            std::cin >> p >> x;
            p--;
            auto hxb = zkw.query(p);
            hxb.insert(x);
            zkw.modify(p, hxb);
        } else {
            int l, r;
            std::cin >> l >> r;
            l--;

            auto hxb = zkw.query(l, r);
            std::cout << hxb.query_max_bitxor() << '\n';
        }
    }

    return 0;
}
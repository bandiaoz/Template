#include <bits/stdc++.h>
#include "src/Math/Misc/power.hpp"
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/LinearAlgebra/StaticMatrix.h"
#include "src/DataStruct/Segtree/ZkwTree.h"

/*
[CF718C Sasha and Array](https://codeforces.com/contest/718/problem/C)
[status](https://codeforces.com/contest/718/submission/338195282)
*/
/*
斐波那契数列 $$\begin{cases}f(n) = 1 & n = 1, 2 \\ f(n) = f(n - 1) + f(n - 2) & n \geq 3\end{cases}$$

给定数组 $$a_1, \dots, a_n$$，有 $$q$$ 次操作：
1. 将区间 $$[l, r]$$ 的 $$a_i$$ 加上 $$x$$

2. 求区间 $$[l, r]$$ 的 $$\sum_{i=l}^r f(a_i)$$

$$1\leq n,m\leq 10^5$$, $$1\leq a_i, x\leq 10^9$$

使用线段树维护向量数组，每个向量为 $$\begin{pmatrix}f(a_i + 1) \\ f(a_i)\end{pmatrix}$$，修改操作就是向量左乘 $$\begin{pmatrix}1 & 1 \\ 1 & 0\end{pmatrix}^x$$，维护区间向量和。
*/

using Z = OY::mint1000000007;
using Info = OY::StaticMatrix<Z, 2, 1>;
using Tag = OY::StaticMatrix<Z, 2, 2>;
auto op = [](Info x, Info y) -> Info {
    return x + y;
};
auto map = [](Tag new_modify, Info old_val, uint32_t len) -> Info {
    return new_modify * old_val;
};
auto com = [](Tag new_modify, Tag old_modify) -> Tag {
    return new_modify * old_modify;
};

auto f(int64_t n) -> Z {
    if (n <= 2) return Z(1);
    OY::StaticMatrix<Z, 2, 2> base{{{1, 1}, {1, 0}}};
    auto res = power(base, n - 2);
    return res[0][0] * f(2) + res[0][1] * f(1);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
        i--;
    }
    
    Tag A{{{1, 1}, {1, 0}}};
    Tag identity{{{1, 0}, {0, 1}}};
    auto mapping = [&](int i) {
        return power(A, a[i]) * Info{{{1}, {1}}};
    };
    auto zkw = OY::make_lazy_ZkwTree<Info, Tag, true>(n, mapping, op, map, com, identity);

    while (q--) {
        int op, l, r;
        std::cin >> op >> l >> r;
        l--;

        if (op == 1) {
            int x;
            std::cin >> x;
            zkw.add(l, r, power<Tag>(A, x));
        } else {
            Info ans = zkw.query(l, r);
            std::cout << ans[1][0] << "\n";
        }
    }

    return 0;
}
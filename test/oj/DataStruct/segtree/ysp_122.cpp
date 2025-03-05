#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/DataStruct/Segtree/MonoZkwTree.h"

/*
[Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/122)
[status](https://judge.yosupo.jp/submission/253569)
*/
/**
 * 给定若干个线性函数 f_i(x) = a_i \times x + b_i，有 q 次操作：
 * 0. 将 f_p(x) 修改为 f_p(x) = c \times x + d
 * 1. 查询 f_{r - 1}(f_{r - 2}(...f_l(x)))
 * 
 * 维护线性操作而不是直接维护值，op 函数复合两个线性操作，线段树维护单点修改区间查询即可
 */

using Z = OY::mint998244353;
struct Info {
    Z mul, add;
};
auto op = [](const Info &a, const Info &b) -> Info {
    return Info{
        a.mul * b.mul,
        a.add * b.mul + b.add
    };
};
constexpr Info identity{1, 0};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    auto read = [&](auto ...) {
        int a, b;
        std::cin >> a >> b;
        return Info{a, b};
    };
    auto tree = OY::make_MonoZkw<Info, identity>(n, op, read);

    while (q--) {
        int op;
        std::cin >> op;

        if (op == 0) {
            int p, c, d;
            std::cin >> p >> c >> d;
            tree.modify(p, Info{c, d});
        } else {
            int l, r, x;
            std::cin >> l >> r >> x;
            auto [mul, add] = tree.query(l, r);
            std::cout << mul * x + add << '\n';
        }
    }

    return 0;
}
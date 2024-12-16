#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/DataStruct/Segtree/MonoZkwTree.h"

/*
[Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/122)
[status](https://judge.yosupo.jp/submission/253569)
*/
/**
 * 本题为单点修改，区间查询的线段树，使用 MonoZkwTree 实现
 * 将一次函数使用 op 操作复合
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
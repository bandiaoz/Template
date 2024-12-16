#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/AssignZkwTree.h"
#include "src/Math/Modular/StaticModInt32.h"

using ll = long long;
using Z = OY::mint998244353;

/*
[Range Set Range Composite](https://judge.yosupo.jp/problem/range_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/829)
[status](https://judge.yosupo.jp/submission/255564)
*/
/**
 * 本题为区间更新区间查询线段树模板题
 * 将一次函数进行区间修改 f_i <- c \times x + d
 * 询问区间复合值 f_{r-1}(f_{r-2}(...f_l(x)...))
 * 使用 AssignZkwTree 实现
 */

struct Info {
    Z mul, add;
    Z calc(Z x) const {
        return mul * x + add;
    }
};
auto op = [](const Info &a, const Info &b) -> Info {
    return Info{a.mul * b.mul, a.add * b.mul + b.add};
};
constexpr Info identity{1, 0};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    auto read = [&](auto...) {
        int mul, add;
        std::cin >> mul >> add;
        return Info{mul, add};
    };
    auto S = OY::make_lazy_AssignZkwTree<Info, identity>(n, op, read);
    while (q--) {
        int op;
        std::cin >> op;
        if (op == 0) {
            int l, r, mul, add;
            std::cin >> l >> r >> mul >> add;
            S.modify(l, r, {mul, add});
        } else {
            int l, r, x;
            std::cin >> l >> r >> x;
            std::cout << S.query(l, r).calc(x) << '\n';
        }
    }

    return 0;
}
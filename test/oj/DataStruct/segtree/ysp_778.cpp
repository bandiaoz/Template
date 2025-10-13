#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ZkwTree.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::mint998244353;

/*
[Range Affine Point Get](https://judge.yosupo.jp/problem/range_affine_point_get)(https://github.com/yosupo06/library-checker-problems/issues/778)
[status](https://judge.yosupo.jp/submission/318976)
*/
/*
给定一个数组，有 $$q$$ 次操作：
1. 区间 Affine 变换，$$a_i \gets b \times a_i + c$$
2. 单点查询，输出 $$a_i \bmod 998244353$$

Info 信息可以是 {mul, add}，也可以是 Z
*/

struct Info {
    Z mul, add;
    Z calc(Z x) const {
        return x * mul + add;
    }
};
struct Tag {
    Z mul, add;
};
Tag identity{1, 0};

auto op = [](const Info &x, const Info &y) -> Info {
    return Info{x.mul * y.mul, x.add * y.mul + y.add};
};
auto map = [](const Tag &new_modify, const Info &old_val, uint32_t len) -> Info {
    return Info{old_val.mul * new_modify.mul, old_val.add * new_modify.mul + new_modify.add * len};
};
auto com = [](const Tag &new_modify, const Tag &old_modify) -> Tag {
    return Tag{old_modify.mul * new_modify.mul, old_modify.add * new_modify.mul + new_modify.add};
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    auto mapping = [&](uint32_t i) { return Info(1, 0); };
    auto zkw = OY::make_lazy_ZkwTree<Info, Tag, true>(n, mapping, op, map, com, identity);

    while (q--) {
        int op;
        std::cin >> op;
        if (op == 0) {
            int l, r;
            Z b, c;
            std::cin >> l >> r >> b >> c;
            zkw.add(l, r, Tag{b, c});
        } else {
            int p;
            std::cin >> p;
            std::cout << zkw.query(p).calc(a[p]) << '\n';
        }
    }

    return 0;
}
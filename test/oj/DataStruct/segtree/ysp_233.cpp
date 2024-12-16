#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"
#include "src/DataStruct/Segtree/ZkwTree.h"

using Z = OY::mint998244353;

/*
[Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/233)
[status](https://judge.yosupo.jp/submission/255565)
*/
/**
 * 本题为区间更新区间查询线段树模板题
 * 区间更新：a_i <- b \times a_i + c
 * 区间查询：\sum_{i=l}^{r-1} a_i
 */

using Info = Z;
struct Tag {
    Z mul, add;
};
auto map = [](Tag modify, Info old_val, uint32_t len) { 
    return old_val * modify.mul + modify.add * len; 
};
auto com = [](Tag new_modify, Tag old_modify) { 
    return Tag{
        old_modify.mul * new_modify.mul, 
        old_modify.add * new_modify.mul + new_modify.add
    }; 
};

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
    // auto seg = OY::make_lazy_SegTree<Info, Tag, true>(
    //     n, read, std::plus<>(), map, com, Tag{1, 0}
    // );
    auto seg = OY::make_lazy_ZkwTree<Info, Tag, true>(
        n, read, std::plus<>(), map, com, Tag{1, 0}
    );
    while (q--) {
        int op, l, r, mul, add;
        std::cin >> op;
        
        if (op == 0) {
            std::cin >> l >> r >> mul >> add;
            seg.add(l, r, Tag{mul, add});
        } else {
            std::cin >> l >> r;
            std::cout << seg.query(l, r) << '\n';
        }
    }

    return 0;
}
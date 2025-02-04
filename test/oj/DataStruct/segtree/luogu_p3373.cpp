#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/SegTree.h"
#include "src/DataStruct/Segtree/ZkwTree.h"
#include "src/Math/Modular/StaticModInt32.h"

using ll = long long;

/*
[P3373 【模板】线段树 2](https://www.luogu.com.cn/problem/P3373)
[status](https://www.luogu.com.cn/record/191938060)
*/
/**
 * 本题要进行区间修改和区间查询
 * 为线段树模板题
 */

using Z = OY::StaticModInt32<571373, true>;
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

    int n, m, p;
    std::cin >> n >> m >> p;
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
    for (int i = 0; i < m; i++) {
        int op, l, r, x;
        std::cin >> op;
        
        if (op == 1) {
            std::cin >> l >> r >> x;
            l--;
            seg.add(l, r, Tag{x, 0});
        } else if (op == 2) {
            std::cin >> l >> r >> x;
            l--;
            seg.add(l, r, Tag{1, x});
        } else {
            std::cin >> l >> r;
            l--;
            std::cout << seg.query(l, r) << '\n';
        }
    }

    return 0;
}
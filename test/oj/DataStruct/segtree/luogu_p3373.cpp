#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/SegTree.h"
#include "src/Math/ModInt/StaticModInt32.h"

using ll = long long;

/*
[P3373 【模板】线段树 2](https://www.luogu.com.cn/problem/P3373)
[status](https://www.luogu.com.cn/record/179556118)
*/
/**
 * 本题要进行区间修改和区间查询
 * 为线段树模板题
 */

using Z = OY::StaticModInt32<571373, true>;

struct Tag {
    Z mul, add;
};
auto map = [](Tag modify, Z old_val, uint32_t len) { 
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
    auto seg = OY::make_lazy_SegTree<Z, Tag, true, OY::Seg::Ignore, uint32_t>(
        n, read, std::plus<>(), map, com, Tag{1, 0}
    );
    for (int i = 0; i < m; i++) {
        int op, l, r, x;
        std::cin >> op;
        
        if (op == 1) {
            std::cin >> l >> r >> x;
            l--, r--;
            seg.add(l, r, Tag{x, 0});
        } else if (op == 2) {
            std::cin >> l >> r >> x;
            l--, r--;
            seg.add(l, r, Tag{1, x});
        } else {
            std::cin >> l >> r;
            l--, r--;
            std::cout << seg.query(l, r) << '\n';
        }
    }

    return 0;
}
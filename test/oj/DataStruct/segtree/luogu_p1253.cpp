#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/SegTree.h"

using ll = long long;

/*
[P1253 扶苏的问题](https://www.luogu.com.cn/problem/P1253)
[status](https://www.luogu.com.cn/record/179768473)
*/
/**
 * 本题要进行区间加区间推平和区间最大值查询
 * 使用线段树模板
 */

using Info = ll;
struct Tag {
    ll mul, add;
};
auto op = [](Info a, Info b) { return std::max(a, b); };
auto map = [](Tag t, Info x, uint32_t len) { return x * t.mul + t.add; };
auto com = [](Tag a, Tag b) { return Tag{a.mul * b.mul, b.add * a.mul + a.add}; };

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
    auto seg = OY::make_lazy_SegTree<Info, Tag, true>(
        n, read, op, map, com, Tag{1, 0}
    );

    while (q--) {
        int op, l, r, x;
        std::cin >> op >> l >> r;
        l--;

        if (op == 1) {
            std::cin >> x;
            seg.add(l, r, Tag{0, x});
        } else if (op == 2) {
            std::cin >> x;
            seg.add(l, r, Tag{1, x});
        } else {
            std::cout << seg.query(l, r) << '\n';
        }
    }

    return 0;
}
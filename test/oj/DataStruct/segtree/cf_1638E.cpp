#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ChthollyZkwTree.h"
#include "src/DataStruct/Fenwick/BIT.h"

using ll = long long;

/*
[E. Colorful Operations](https://codeforces.com/contest/1638/problem/E)
[status](https://codeforces.com/contest/1638/submission/301181968)
*/
/**
 * 一个长度为 n 的数组，初始时所有元素均为 0，颜色均为 1
 * 有 q 次操作
 * 1. 将区间 [l, r) 的颜色改为 c
 * 2. 将所有颜色 c 的值增加 x
 * 3. 查询下标为 p 的元素的值
 * 
 * 使用珂朵莉树维护颜色，使用一个 map 维护每个颜色的额外值
 * 颜色改变时，例如原本颜色为 1，现在改为 2，则需要将先加上颜色 1 的额外值，再减掉颜色 2 的额外值，这部分用树状数组维护区间修改单点查询
 * 这样，查询时，只需要在 Fenwick 中查询下标为 p 的元素的值，再加上颜色 p 的额外值即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    OY::VectorBIT<ll> fen(n + 1);
    OY::ChthollyZkwTree<int> cho(n, 1);

    auto add = [&](int l, int r, ll c) {
        fen.add(l, c);
        fen.add(r, -c);
    };
    auto query = [&](int x) {
        return fen.presum(x + 1);
    };

    std::map<int, ll> color_extra;

    while (q--) {
        std::string op;
        std::cin >> op;

        if (op == "Color") {
            int l, r, c;
            std::cin >> l >> r >> c;
            l--;
            cho.enumerate(l, r, [&](int l, int r, int val) {
                add(l, r, color_extra[val] - color_extra[c]);
                cho.modify(l, r, c);
            });
        } else if (op == "Add") {
            int c, x;
            std::cin >> c >> x;
            color_extra[c] += x;
        } else {
            int p;
            std::cin >> p;
            p--;
            std::cout << query(p) + color_extra[cho.query(p)] << '\n';
        }
    }

    return 0;
}
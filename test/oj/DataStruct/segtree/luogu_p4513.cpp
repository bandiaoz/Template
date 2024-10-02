#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/SegTree.h"

using ll = long long;

/*
[P4513 小白逛公园](https://www.luogu.com.cn/problem/P4513)
[status](https://www.luogu.com.cn/record/179571800)
*/
/**
 * 本题要进行单点修改和查询区间最大子段和
 * 为线段树模板题
 */

struct Info {
    int sum, left, right, ans;
    Info() = default;
    Info(int x) : sum(x), left(x), right(x), ans(x) {}
    Info(int sum, int left, int right, int ans) : sum(sum), left(left), right(right), ans(ans) {}
};

auto op = [](Info a, Info b) -> Info {
    return Info{
        a.sum + b.sum, 
        std::max(a.left, a.sum + b.left), 
        std::max(b.right, b.sum + a.right), 
        std::max({a.ans, b.ans, a.right + b.left})
    };
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    auto read = [&](...) {
        int x;
        std::cin >> x;
        return Info(x);
    };
    auto seg = OY::make_SegTree<Info>(n, op, read);

    while (m--) {
        int op, x, y;
        std::cin >> op >> x >> y;
        if (op == 1) {
            if (x > y) std::swap(x, y);
            std::cout << seg.query(x - 1, y - 1).ans << '\n';
        } else {
            seg.modify(x - 1, Info{y, y, y, y});
        }
    }

    return 0;
}
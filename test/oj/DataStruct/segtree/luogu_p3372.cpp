#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/SegTree.h"

using ll = long long;

/*
[P3372 【模板】线段树 1](https://www.luogu.com.cn/problem/P3372)
[status](https://www.luogu.com.cn/record/179754134)
*/
/**
 * 本题要进行区间修改和区间查询
 * 为线段树模板题
 * 也可以使用树状数组解决，而且更快
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    OY::VectorSegLazySumTree<ll, int> seg(n, [](...) {
        ll x;
        std::cin >> x;
        return x;
    });

    while (m--) {
        int op, l, r;
        std::cin >> op;
        if (op == 1) {
            ll k;
            std::cin >> l >> r >> k;
            l--;
            seg.add(l, r, k);
        } else {
            std::cin >> l >> r;
            l--;
            std::cout << seg.query(l, r) << '\n';
        }
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/DataStruct/ClassicProblem/RangeMode.h"

/*
[Static Range Mode Query](https://judge.yosupo.jp/problem/static_range_mode_query)(https://github.com/yosupo06/library-checker-problems/issues/959)
[status](https://judge.yosupo.jp/submission/253561)
*/
/**
 * 本题需要对静态区间进行区间众数查询
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    OY::RangeMode<int> mode(n, [&](int) {
        int x;
        std::cin >> x;
        return x;
    });

    while (q--) {
        int l, r;
        std::cin >> l >> r;
        auto [val, cnt] = mode.query(l, r);
        std::cout << val << ' ' << cnt << '\n';
    }

    return 0;
}
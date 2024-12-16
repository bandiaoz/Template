#include <bits/stdc++.h>
#include "src/DataStruct/ClassicProblem/OfflineKindCounter.h"

/*
[Static Range Count Distinct](https://judge.yosupo.jp/problem/static_range_count_distinct)(https://github.com/yosupo06/library-checker-problems/issues/770)
*/
/**
 * 本题可以作为小波树模板题，用小波树在线回答区间种类数
 * 当然跑得最快的还得离线查询
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    auto read = [](auto...) {
        int x;
        std::cin >> x;
        return x;
    };
    OY::OfflineHashmapKindCounter<false, 700001> kc(n, read, q);
    for (int i = 0; i < q; i++) {
        int l, r;
        std::cin >> l >> r;
        kc.add_query(l, r);
    }
    
    for (auto kind : kc.solve()) {
        std::cout << kind << '\n';
    }

    return 0;
}
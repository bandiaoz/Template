#include <bits/stdc++.h>
#include "src/DataStruct/AdjDiff2D.h"

using ll = long long;

/*
[T271585 【模板】二维前缀和](https://www.luogu.com.cn/problem/T271585)
[status](https://www.luogu.com.cn/record/182390663)
*/
/**
 * 本题为二维差分模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, q;
    std::cin >> n >> m >> q;
    auto read = [](auto...) {
        ll x;
        std::cin >> x;
        return x;
    };
    OY::AdjSumTable2D<ll> table(n, m, read);

    while (q--) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        x1--, y1--;
        std::cout << table.query(x1, x2, y1, y2) << '\n';
    }

    return 0;
}
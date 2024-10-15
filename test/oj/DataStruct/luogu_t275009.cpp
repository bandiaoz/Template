#include <bits/stdc++.h>
#include "src/DataStruct/AdjDiff2D.h"

using ll = long long;

/*
[T275009 【模板】二维差分](https://www.luogu.com.cn/problem/T275009)
[status](https://www.luogu.com.cn/record/182391551)
*/
/**
 * 本题为二维差分模板题
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, q;
    std::cin >> n >> m >> q;
    OY::AdjSumTable2D<int64_t> S(n, m, [](...) {
        int64_t x;
        std::cin >> x;
        return x;
    });

    while (q--) {
        int x1, y1, x2, y2;
        ll c;
        std::cin >> x1 >> y1 >> x2 >> y2 >> c;
        x1--, y1--;
        S.add(x1, x2, y1, y2, c);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << S.query(i, j) << " \n"[j == m - 1];
        }
    }

    return 0;
}
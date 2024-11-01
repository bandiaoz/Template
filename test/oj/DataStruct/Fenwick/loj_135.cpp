#include <bits/stdc++.h>
#include "src/DataStruct/Fenwick/BIT2D_ex.h"

using ll = long long;

/*
[#135. 二维树状数组 3：区间修改，区间查询](https://loj.ac/p/135)
*/
/**
 * 本题为二维树状数组模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::BIT2DEX::Tree<ll> table(n, m);

    int op;
    while (std::cin >> op) {
        if (op == 1) {
            int x1, y1, x2, y2;
            ll v;
            std::cin >> x1 >> y1 >> x2 >> y2 >> v;
            x1--, y1--;
            table.add(x1, x2, y1, y2, v);
        } else {
            int x1, y1, x2, y2;
            std::cin >> x1 >> y1 >> x2 >> y2;
            x1--, y1--;
            std::cout << table.query(x1, x2, y1, y2) << '\n';
        }
    }

    return 0;
}
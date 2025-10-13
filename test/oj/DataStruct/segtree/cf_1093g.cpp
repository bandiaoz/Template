#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ZkwTree.h"

/*
[CF1093G Multidimensional Queries](https://codeforces.com/contest/1093/problem/G)
[status](https://codeforces.com/contest/1093/submission/339039957)
*/
/*
给定 $$n(1 \leq n \leq 2 \times 10^5)$$ 个 $$k(1 \leq k \leq 5)$$ 维点，两点距离为 $$\sum_{i=1}^k |a_i - b_i|$$，即曼哈顿距离，有 $$q(1 \leq q \leq 2 \times 10^5)$$ 次操作：

1. 将第 $$i$$ 个点坐标修改为 $$(b_1, b_2, \dots, b_k)$$
2. 查询区间 $$[l, r]$$ 内，任意两点的最大距离

两点距离为 $$\sum_{i=1}^k |a_i - b_i| = \max\left(\sum_{i = 1}^k (-1)^{c_i} (a_i - b_i)\right)$$，其中 $$c_i$$ 为 $$0$$ 或 $$1$$，一共有 $$2^k$$ 种情况

使用线段树维护即可
*/

const int inf = 1e9;
using Info = std::vector<int>;
auto op = [](Info x, Info y) -> Info {
    if (y.empty()) return x;
    Info res(x.size());
    for (int i = 0; i < res.size(); i++) {
        res[i] = std::max(x[i], y[i]);
    }
    return res;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;
    std::vector points(n, std::vector<int>(k));
    for (auto &point : points) {
        for (auto &i : point) {
            std::cin >> i;
        }
    }

    auto get = [&](const std::vector<int> &point) {
        Info res(1 << k);
        for (int mask = 0; mask < (1 << k); mask++) {
            for (int i = 0; i < k; i++) {
                int sign = (mask >> i & 1) ? 1 : -1;
                res[mask] += sign * point[i];
            }
        }
        return res;
    };

    auto zkw = OY::make_ZkwTree<Info>(n, op, [&](uint32_t i) { return get(points[i]); });

    int q;
    std::cin >> q;
    while (q--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            int p;
            std::cin >> p;
            p--;
            std::vector<int> point(k);
            for (auto &i : point) {
                std::cin >> i;
            }
            zkw.modify(p, get(point));
        } else {
            int l, r;
            std::cin >> l >> r;
            l--;

            int ans = -inf;
            auto res = zkw.query(l, r);
            for (int mask = 0; mask < (1 << k); mask++) {   
                ans = std::max(ans, res[mask] + res[(1 << k) - 1 - mask]);
            }
            std::cout << ans << '\n';
        }
    }

    return 0;
}
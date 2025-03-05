#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ZkwTree.h"

using ll = long long;

/*
[CF992E Nastya and King-Shamans](https://www.luogu.com.cn/problem/CF992E)
[status](https://codeforces.com/contest/992/problem/E)
*/
/**
 * 给定一个数组 a，有 q 次操作：将 a_i 修改为 x，然后询问是否存在 a_i = sum(a[1..i - 1])，特别地，如果 a_1 = 0，则存在 i = 1
 * 如果存在则输出 i，否则输出 -1
 * 
 * 满足 f(i) = a_i - sum(a[1...i - 1]) = 0 的 i 就是所求答案，并且满足 f(i) >= 0 的 i 的数量最多是 log 个
 * 假设有 f(i) >= 0 且 f(j) >= 0，则 a[j] >= 2 \times a[i]
 * 线段树维护 f(i) 的最大值
 * 树上二分找到所有满足 f(i) >= 0 的区间即可
 */

using Info = ll;
using Tag = ll;
auto op = [](Info x, Info y) -> Info {
    return std::max(x, y);
};
auto map = [](Tag modify, Info old_val, uint32_t len) -> Info {
    return old_val + modify;
};
auto com = [](Tag new_modify, Tag old_modify) -> Tag {
    return new_modify + old_modify;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    OY::ZkwSumTree<ll> sum_zkw(n, [&](int i) { return a[i]; });
    auto get = [&](int l, int r) {
        return l == r ? 0 : sum_zkw.query(l, r);
    };
    auto mapping = [&](int i) {
        return a[i] - get(0, i);
    };
    auto zkw = OY::make_lazy_ZkwTree<Info, Tag, true>(n, mapping, op, map, com, Tag{0});

    while (q--) {
        int p, x;
        std::cin >> p >> x;
        p--;
        int add = x - a[p];
        a[p] = x;
        sum_zkw.add(p, add);
        zkw.add(p, add);
        if (p + 1 < n) {
            zkw.add(p + 1, n, -add);
        }

        int ans = -1;
        for (int l = 0, next; l < n; l = next + 1) {
            next = zkw.max_right(l, [&](Info val) {
                return val < 0;
            }) + 1;
            if (next < n && zkw.query(next) == 0) {
                ans = next;
                break;
            }
        }

        std::cout << (ans == -1 ? -1 : ans + 1) << "\n";
    }

    return 0;
}
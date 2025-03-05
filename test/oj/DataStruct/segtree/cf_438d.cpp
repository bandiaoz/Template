#include <bits/stdc++.h>
#include "src/DataStruct/Segtree/ZkwTree.h"

using ll = long long;

/*
[CF438D The Child and Sequence](https://codeforces.com/contest/438/problem/D)
[status](https://codeforces.com/contest/438/submission/308757796)
*/
/**
 * 给定一个数组 a，有 q 次操作
 * 1. 区间求和，输出 a_l + a_{l+1} + ... + a_r
 * 2. 区间取模，将 a_l, a_{l+1}, ..., a_r 都取模 p
 * 3. 单点修改，将 a_x 修改为 y
 * 
 * 维护区间和 sum 和区间最大值 max
 * 对于第二个操作，如果 max < p，则不进行操作
 * 当 p < x 时，x mod p < x / 2，也就是一个数最多可以取模 log 次，那么操作 2 单点取模次数为 n\log x 次
 * 复杂度为两个 log
 */

struct Info {
    ll sum, max;
    Info(ll x = 0) : sum(x), max(x) {}
    Info(ll x, ll y) : sum(x), max(y) {}
};
auto op = [](Info x, Info y) {
    return Info{x.sum + y.sum, std::max(x.max, y.max)};
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    auto read = [&](...) -> Info {
        int x;
        std::cin >> x;
        return Info{x, x};
    };
    auto zkw = OY::make_ZkwTree<Info>(n, op, read);

    while (q--) {
        int op;
        std::cin >> op;
        
        if (op == 1) {
            int l, r;
            std::cin >> l >> r;
            l--;
            std::cout << zkw.query(l, r).sum << '\n';
        } else if (op == 2) {
            int l, r, p;
            std::cin >> l >> r >> p;
            l--;

            auto next = [&](int left) {
                return zkw.max_right(left, [&](Info x) { return x.max < p; }) + 1;
            };
            for (int i = next(l); i < r; i = next(i)) {
                zkw.modify(i, Info(zkw.query(i, i + 1).sum % p));
            }
        } else {
            int p, x;
            std::cin >> p >> x;
            p--;
            zkw.modify(p, Info(x));
        }
    }

    return 0;
}
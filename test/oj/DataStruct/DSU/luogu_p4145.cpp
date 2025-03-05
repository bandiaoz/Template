#include <bits/stdc++.h>
#include "src/DataStruct/DSU/LinearDSU.h"
#include "src/DataStruct/Fenwick/MonoBIT.h"

using ll = long long;

/*
[P4145 上帝造题的七分钟 2 / 花神游历各国](https://www.luogu.com.cn/problem/P4145)
[status](https://www.luogu.com.cn/record/205114043)
*/
/**
 * 给定长度为 n 的数组 a，有 q 次操作
 * 0. 将区间 [l, r] 内的数开平方
 * 1. 查询区间 [l, r] 内的数的和
 * 
 * 由于最多开平方 6 次就变成了 1，所以可以暴力开平方，用树状数组维护单点修改区间查询，用并查集跳过已经是 1 的数
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<ll> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }
    OY::MonoSumBIT<ll> fen(n, [&](int i) { return a[i]; });

    int q;
    std::cin >> q;
    OY::LDSU::Table<true> dsu(n + 1);
    while (q--) {
        int op, l, r;
        std::cin >> op >> l >> r;
        if (l > r) std::swap(l, r);
        l--;
        
        if (op == 0) {
            for (int i = dsu.find_tail(l); i < r; i = dsu.find_tail(i + 1)) {
                fen.add(i, -a[i]);
                a[i] = std::sqrt(a[i]);
                fen.add(i, a[i]);
                if (a[i] == 1) {
                    dsu.unite_after(i);
                }
            }
        } else {
            std::cout << fen.query(l, r) << "\n";
        }
    }

    return 0;
}
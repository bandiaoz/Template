#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::mint1000000007;

/*
[CF959F Mahmoud and Ehab and yet another xor task](https://codeforces.com/contest/959/problem/F)
[status](https://codeforces.com/contest/959/submission/311620210)
*/
/**
 * 给定长度为 n 的数组 a，有 q 次询问，每次询问前 len 个数字中，有多少种不同的子集（可以为空）异或和为 x
 * 
 * 离线下来，按 len 排序，插入线性基，查询
 * 如果 x 非零并且不可表达，那么答案显然为 0
 * 否则答案等于 2^zero，zero 为不在线性基中元素个数
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    std::vector<std::array<int, 3>> qry(q);
    for (int i = 0; i < q; i++) {
        auto &[len, x, id] = qry[i];
        std::cin >> len >> x;
        id = i;
    }

    std::sort(qry.begin(), qry.end(), [&](const auto &lhs, const auto &rhs) {
        return lhs[0] < rhs[0];
    });
    
    OY::StaticHamelXorBase32<20> hxb;
    int cur = 0;
    std::vector<Z> ans(q);
    for (auto [len, x, id] : qry) {
        while (cur < len) {
            hxb.insert(a[cur++]);
        }
        int zero = len - hxb.base_count();
        if (hxb.contains(x)) {
            ans[id] = Z(2).pow(zero);
        }
    }

    for (auto i : ans) {
        std::cout << i << '\n';
    }
    
    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"

using ll = long long;

/*
[P4570 [BJWC2011] 元素](https://www.luogu.com.cn/problem/P4570)
[status](https://www.luogu.com.cn/record/207039112)
*/
/**
 * 给定若干数，每个数有一个权值，求权值之和最大的线性基（所选的数不能组成异或和为 0 的集合）
 * 
 * 按照权值从大到小插入线性基，如果插入成功，则将权值加入答案
 * 无论怎么插入，最终线性基的数量都是相同的
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<std::pair<ll, int>> a(n);
    for (auto &[num, val] : a) {
        std::cin >> num >> val;
    }

    std::sort(a.begin(), a.end(), [&](const auto &lhs, const auto &rhs) {
        return lhs.second > rhs.second;
    });

    int ans = 0;
    OY::StaticHamelXorBase64<60> hxb{};
    for (const auto &[num, val] : a) {
        if (hxb.insert(num) != -1) {
            ans += val;
        }
    }

    std::cout << ans << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/Combination/CantorRank.h"
#include "src/Math/Modular/StaticModInt32.h"
#ifndef LOCAL
#define debug(...)
#endif

using Z = OY::mint998244353;

/*
[P5367 【模板】康托展开](https://www.luogu.com.cn/problem/P5367)
[status](https://www.luogu.com.cn/record/221528659)
*/
/**
 * 本题要求的是一个排列的排名，需要康托展开
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }

    Z ans = OY::CantorRank<Z>::raw_query(a.begin(), a.end());
    std::cout << ans + 1 << '\n';

    return 0;
}
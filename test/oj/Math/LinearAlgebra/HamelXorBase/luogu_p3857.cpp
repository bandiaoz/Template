#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"

using ll = long long;

/*
[P3857 [TJOI2008] 彩灯](https://www.luogu.com.cn/problem/P3857)
[status](https://www.luogu.com.cn/record/207069273)
*/
/**
 * 给定 n 个数，求不同子集 xor 和的种类数。
 * 
 * 答案等于 power(2, 线性基大小)
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::DynamicHamelXorBase64::set_width(n);
    OY::DynamicHamelXorBase64 hxb;
    for (int i = 0; i < m; i++) {
        std::string s;
        std::cin >> s;
        ll val = 0;
        for (int j = 0; j < n; j++) {
            if (s[j] == 'O') {
                val |= 1LL << j;
            }
        }
        hxb.insert(val);
    }

    std::cout << hxb.total_count() % 2008;

    return 0;
}
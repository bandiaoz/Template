#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"

using ll = long long;

/*
[P3812 【模板】线性基](https://www.luogu.com.cn/problem/P3812)
[status](https://www.luogu.com.cn/record/182385706)
*/
/**
 * 本题为线性基模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::StaticHamelXorBase64<50> hxb;
    for (int i = 0; i < n; i++) {
        ll x;
        std::cin >> x;
        hxb.insert(x);
    }

    std::cout << hxb.query_max_bitxor() << "\n";

    return 0;
}
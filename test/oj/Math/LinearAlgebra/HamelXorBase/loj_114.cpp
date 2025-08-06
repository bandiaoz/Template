#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"

using ll = long long;

/*
[#114. k 大异或和](https://loj.ac/p/114)
[status](https://loj.ac/s/2279093)
*/
/**
 * 对于一个可重集，每次给定一个 k，求第 k 小的非空异或和
 * 
 * 如果 n 个向量线性无关，则不可表达 0，k 需要手动加一（题目要求集合非空）
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::StaticHamelXorBase64<51> hxb;
    for (int i = 0; i < n; i++) {
        ll x;
        std::cin >> x;
        hxb.insert(x);
    }

    bool zero = hxb.base_count() < n;

    int q;
    std::cin >> q;
    while (q--) {
        ll k;
        std::cin >> k;
        k--;

        if (!zero) {
            k++;
        }
        if (k >= hxb.total_count()) {
            std::cout << "-1\n";
        } else {
            std::cout << hxb.kth(k) << "\n";
        }
    }

    return 0;
}
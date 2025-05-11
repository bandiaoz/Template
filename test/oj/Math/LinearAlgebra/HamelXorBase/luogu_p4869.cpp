#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/HamelXorBase.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::StaticModInt32<10086, false>;

/*
[P4869 albus就是要第一个出场](https://www.luogu.com.cn/problem/P4869)
[status](https://www.luogu.com.cn/record/207194041)
*/
/**
 * 给定 n 个数组成的线性基，n 个数有 2^n 个子集和 2^n 个子集异或和，将异或和从大到小排序。
 * 给定一个数 num，求 num 在所有子集异或和中的排名。
 * 
 * 结论：n 个数，组成大小为 s 的线性基，能构成 2^s 种不同的数，每个数出现 2^(n-s) 次。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::StaticHamelXorBase32<30> hxb{};
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        hxb.insert(x);
    }

    int num;
    std::cin >> num;
    std::cout << hxb.rank(num) * Z(2).pow(n - hxb.base_count()) + 1 << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/Math/Modular/DynamicModInt32.h"
#include "src/Math/NumberTheory/Cipolla.h"

/*
[P5491 【模板】二次剩余](https://www.luogu.com.cn/problem/P5491)
[status](https://www.luogu.com.cn/record/205879554)
*/
/**
 * 给定一个数 $n$ 和质数 $p$，求 $n$ 的二次剩余。
 * 即 $x^2 \equiv n \pmod p$ 的解 $x$。
 * 
 * 特判 $n = 0$ 的情况，直接输出 $0$。
 * 使用 Cipolla 算法求解二次剩余，如果有解，一定有两个解，且互为相反数 $x$ 和 $p - x$。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    while (t--) {
        int n, p;
        std::cin >> n >> p;

        if (n == 0) {
            std::cout << "0\n";
        } else {
            using Z = OY::DynamicModInt32<0>;
            Z::set_mod(p, true);
            if (auto a = OY::Cipolla<Z>(n); a) {
                std::cout << a << ' ' << p - a << '\n';
            } else {
                std::cout << "Hola!\n";
            }
        }
    }

    return 0;
}
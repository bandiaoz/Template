#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/GaussJordanBitxorElimination.h"

/*
[System of Linear Equations (Mod 2)](https://judge.yosupo.jp/problem/system_of_linear_equations_mod_2)(https://github.com/yosupo06/library-checker-problems/issues/896)
[status](https://judge.yosupo.jp/submission/240217)
*/
/**
 * 本题为异或方程组消元模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::GJBE::GaussJordanBitxorElimination<1 << 12, 1 << 12> GS(m, n);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        for (int j = 0; j < m; j++) {
            GS.coef(i, j) = s[j] - '0';
        }
    }

    std::string result;
    std::cin >> result;
    for (int i = 0; i < n; i++) {
        GS.result(i) = result[i] - '0';
    }

    if (!GS.calc()) {
        std::cout << "-1\n";
    } else {
        std::cout << GS.rank() << '\n';
        for (int i = 0; i < m; i++) {
            std::cout << GS.get_solution(i);
        }
        std::cout << '\n';

        GS.enumerate_base([&](auto &&bitset) {
            for (int i = 0; i < m; i++) {
                std::cout << bitset[i];
            }
            std::cout << '\n';
        });
    }

    return 0;
}
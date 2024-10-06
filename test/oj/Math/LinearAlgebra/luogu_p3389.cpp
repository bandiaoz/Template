#include <bits/stdc++.h>
#include "src/Math/LinearAlgebra/GaussJordanElimination.h"

/*
[P3389 【模板】高斯消元法](https://www.luogu.com.cn/problem/P3389)
[status](https://www.luogu.com.cn/record/180589638)
*/
/**
 * 本题为高斯消元法模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::GJE::GaussJordanElimination<double, 100, 100> GE(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> GE.coef(i, j);
        }
        std::cin >> GE.result(i);
    }

    if (!GE.calc() || GE.has_multi_solution()) {
        std::cout << "No Solution\n";
    } else {
        std::cout << std::fixed << std::setprecision(2);
        for (int i = 0; i < n; i++) {
            std::cout << GE.get_solution(i) << '\n';
        }
    }

    return 0;
}
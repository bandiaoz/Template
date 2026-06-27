#include <bits/stdc++.h>
#include "src/Math/Poly/FastTransform.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::mint998244353;

/*
[P4717 【模板】快速莫比乌斯/沃尔什变换 (FMT/FWT)](https://www.luogu.com.cn/problem/P4717)
[status](https://www.luogu.com.cn/record/231409717)
*/
/*
给定长度为 $$2^n(1 \leq n \leq 17)$$ 两个序列 $$A,B$$，$$C_i=\sum_{j\oplus k = i}A_j \times B_k$$
分别当 $$\oplus$$ 是 or, and, xor 时求出 $$C$$。

FWT 模板题
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<Z> a(1 << n), b(1 << n);
    for (auto &i : a) {
        std::cin >> i;
    }
    for (auto &i : b) {
        std::cin >> i;
    }

    auto work_or = [&](std::vector<Z> a, std::vector<Z> b) {
        OY::FASTTRANS::fast_bitor_transform<true>(a.begin(), a.end());
        OY::FASTTRANS::fast_bitor_transform<true>(b.begin(), b.end());
        for (int i = 0; i < a.size(); i++) {
            a[i] *= b[i];
        }
        OY::FASTTRANS::fast_bitor_transform<false>(a.begin(), a.end());
        for (int i = 0; i < a.size(); i++) {
            std::cout << a[i] << " \n"[i == a.size() - 1];
        }
    };

    auto work_and = [&](std::vector<Z> a, std::vector<Z> b) {
        OY::FASTTRANS::fast_bitand_transform<true>(a.begin(), a.end());
        OY::FASTTRANS::fast_bitand_transform<true>(b.begin(), b.end());
        for (int i = 0; i < a.size(); i++) {
            a[i] *= b[i];
        }
        OY::FASTTRANS::fast_bitand_transform<false>(a.begin(), a.end());
        for (int i = 0; i < a.size(); i++) {
            std::cout << a[i] << " \n"[i == a.size() - 1];
        }
    };

    auto work_xor = [&](std::vector<Z> a, std::vector<Z> b) {
        OY::FASTTRANS::fast_bitxor_transform<true>(a.begin(), a.end());
        OY::FASTTRANS::fast_bitxor_transform<true>(b.begin(), b.end());
        for (int i = 0; i < a.size(); i++) {
            a[i] *= b[i];
        }
        OY::FASTTRANS::fast_bitxor_transform<false>(a.begin(), a.end());
        for (int i = 0; i < a.size(); i++) {
            std::cout << a[i] << " \n"[i == a.size() - 1];
        }
    };

    work_or(a, b);
    work_and(a, b);
    work_xor(a, b);
    
    return 0;
}
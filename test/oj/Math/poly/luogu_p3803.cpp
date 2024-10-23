#include <bits/stdc++.h>
#include "src/Math/Poly/FFTPolynomial.h"
#include "src/Math/Poly/NTTPolynomial.h"
#include "src/Math/Modular/StaticModInt32.h"

/*
[P3803 【模板】多项式乘法（FFT）](https://www.luogu.com.cn/problem/P3803)
[status(FFT)](https://www.luogu.com.cn/record/183697366)
[status(NTT)](https://www.luogu.com.cn/record/183700265)
*/
/**
 * 本题为多项式乘法的模板题，使用 FFT 或 NTT 算法解决
 */

// using poly = OY::FFTPolynomial<int>;
using Z = OY::mint998244353;
using poly = OY::NTTPolynomial<Z>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    poly::s_primitiveRoot = 3;

    int n, m;
    std::cin >> n >> m;
    poly a(n + 1), b(m + 1);
    std::cin >> a >> b;

    poly c = a * b;
    c.resize(n + m + 1);
    for (int i = 0; i < c.size(); i++) {
        std::cout << c[i] << ' ';
    }

    return 0;
}
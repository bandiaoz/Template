#include <bits/stdc++.h>
#include "src/Math/Poly/NTTPolynomial.h"
#include "src/Math/Modular/StaticModInt32.h"

/*
[Convolution](https://judge.yosupo.jp/problem/convolution_mod)
[status](https://judge.yosupo.jp/submission/244430)
*/
/**
 * 本题为多项式乘法的模板题，对 998244353 取模，使用 NTT 算法解决
 */

using Z = OY::mint998244353;
using poly = OY::NTTPolynomial<Z>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    poly::s_primitiveRoot = 3;
    
    int n, m;
    std::cin >> n >> m;
    poly a(n), b(m);
    std::cin >> a >> b;

    poly c = a * b;
    c.resize(n + m - 1);
    std::cout << c << "\n";

    return 0;
}
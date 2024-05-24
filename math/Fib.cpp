#include <bits/stdc++.h>
#include "../Others/Z.hpp"
#include "../Math/Matrix.cpp"

using ll = long long;
using Z = ModInt<int, 1'000'000'007>;

/**
 * @brief 斐波那契数列
 * @link https://www.luogu.com.cn/problem/P1962
 */
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll n;
    std::cin >> n;
    
    auto f = [](auto &&self, ll n) -> Z {
        if (n <= 2) return Z(1);
        
        Matrix<Z> base(2, 2);
        base.a[0][0] = base.a[0][1] = base.a[1][0] = 1;
        Matrix res = power(base, n - 2);
        return res.a[0][0] * self(self, 2) + res.a[0][1] * self(self, 1);
    };
    
    std::cout << f(f, n) << "\n";

    return 0;
}
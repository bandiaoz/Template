#include <bits/stdc++.h>
#include "src/Math/NumberTheory/PollardRho.h"

/*
[P5091 扩展欧拉定理](https://www.luogu.com.cn/problem/P5091)
[status](https://www.luogu.com.cn/record/229391445)
*/
/*
给定三个正整数，$$a,m,b$$，求 $$a^b \bmod m$$
数据范围：$$1\leq a \leq 10^9$$，$$1\leq b \leq 10^{20000000}$$，$$1\leq m \leq 10^8$$。

扩展欧拉定理：
$$a^{b} \equiv \begin{cases}
            a^b & , b < \varphi(m) \\
            a^{b \bmod \varphi(m) + \varphi(m)} & , b \geq \varphi(m)
         \end{cases} \pmod m$$
*/

int power(int a, int b, int m) {
    int ans = 1;
    while (b) {
        if (b & 1) ans = 1LL * ans * a % m;
        a = 1LL * a * a % m;
        b >>= 1;
    }
    return ans;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int a, m;
    std::string b;
    std::cin >> a >> m >> b;

    int phi = OY::PollardRho::get_Euler_Phi(m);
    int small_b = 0;
    for (auto c : b) {
        small_b = small_b * 10 + c - '0';
        if (small_b >= phi) {
            small_b = small_b % phi + phi;
        }
    }

    std::cout << power(a % m, small_b, m) << '\n';

    return 0;
}
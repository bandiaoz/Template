#include <bits/stdc++.h>
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::StaticModInt32<19260817, true>;

/*
[P2613 【模板】有理数取余](https://www.luogu.com.cn/problem/P2613)
[status](https://www.luogu.com.cn/record/229202580)
*/

/*
给出一个有理数 $$c=\frac{a}{b}$$，求 $$c \bmod 19260817$$ 的值（$$19260817$$ 是一个质数）
$$0\leq a \leq 10^{10001}$$，$$1 \leq b \leq 10^{10001}$$，且 $$a, b$$ 不同时是 $$19260817$$ 的倍数。

当 $$b$$ 与 $$19260817$$ 互质时，$$c$$ 的值为 $$a \times b^{-1} \bmod 19260817$$，否则无解。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto get = [&]() {
        std::string s;
        std::cin >> s;
        Z a = 0;
        for (auto c : s) {
            a = a * 10 + (c - '0');
        }
        return a;
    };

    auto a = get(), b = get();
    if (std::gcd(b.val(), Z::mod()) != 1) {
        std::cout << "Angry!\n";
    } else {
        std::cout << a / b << '\n';
    }

    return 0;
}
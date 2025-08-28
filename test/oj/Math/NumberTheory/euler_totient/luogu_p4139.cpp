#include <bits/stdc++.h>
#include "src/Math/NumberTheory/EulerSieve.h"

/*
[P4139 上帝与集合的正确用法](https://www.luogu.com.cn/problem/P4139)
[status](https://www.luogu.com.cn/record/229398528)
*/
/*
定义 $$a_0=1,a_n=2^{a_{n-1}}$$，可以证明 $$b_n=a_n\bmod p$$ 在某一项后都是同一个值，求这个值。
数据范围：$$T\leq 10^3$$，$$p\leq 10^7$$。

由扩展欧拉定理：当 $$b \geq \varphi(m)$$ 时，$$a^b \equiv a^{b \bmod \varphi(p) + \varphi(p)} \pmod p$$

因此：$$a_n \bmod p = 2^{a_{n-1} \bmod \varphi(p) + \varphi(p)} \bmod p$$

递归求解即可
*/

OY::EulerSieve::Sieve<10'000'000, true> sieve;
int power(int a, int b, int m) {
    int ans = 1;
    while (b) {
        if (b & 1) ans = 1LL * ans * a % m;
        a = 1LL * a * a % m;
        b >>= 1;
    }
    return ans;
}

void solve() {
    int p;
    std::cin >> p;

    auto solve = [&](auto &&self, int p) -> int {
        if (p == 1) return 0;
        int phi = sieve.get_Euler_Phi(p);
        return power(2, self(self, phi) + phi, p);
    };

    std::cout << solve(solve, p) << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
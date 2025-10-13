#include <bits/stdc++.h>
#include "src/Math/NumberTheory/PrimeCheck.h"

/*
[Primality Test](https://judge.yosupo.jp/problem/primality_test)(https://github.com/yosupo06/library-checker-problems/issues/880)
[status](https://judge.yosupo.jp/submission/319155)
*/
/*
$$q$$ 次询问，每次询问一个正整数 $$n$$，判断 $$n$$ 是否为质数

miller-rabin 素性检验
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> q;

    while (q--) {
        uint64_t n;
        std::cin >> n;
        std::cout << (OY::is_prime64(n) ? "Yes" : "No") << "\n";
    }

    return 0;
}
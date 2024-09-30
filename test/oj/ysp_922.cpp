#include <bits/stdc++.h>
#include "src/Math/NumberTheory/SqrtDecomposition.h"

using ll = long long;

/*
[Enumerate Quotients](https://judge.yosupo.jp/problem/enumerate_quotients)(https://github.com/yosupo06/library-checker-problems/issues/922)
[status](https://judge.yosupo.jp/submission/238804)
*/
/**
 * 本题为数论分块模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll n;
    std::cin >> n;
    OY::SqrtDecomposition<uint64_t> sd(n);
    std::cout << sd.size() << "\n";
    for (auto [quot, left, right] : sd) {
        std::cout << quot << " ";
    }

    return 0;
}

#include <bits/stdc++.h>
#include "src/Math/Combination/CantorRank.h"
#include "src/Math/Combination/CantorSequence.h"
#include "src/Math/Modular/StaticModInt64.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[P3014 [USACO11FEB] Cow Line S](https://www.luogu.com.cn/problem/P3014)
[status](https://www.luogu.com.cn/record/221529771)
*/
/**
 * 本题要求康托展开和逆康托展开
 */

using Z = OY::mint9223372036854775783; // 比 n! 大的数

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    while (q--) {
        char op;
        std::cin >> op;

        if (op == 'P') {
            ll rank;
            std::cin >> rank;
            rank--;

            auto seq = OY::get_Cantor_sequence(n, rank);
            for (int i = 0; i < n; i++) {
                std::cout << seq[i] + 1 << " \n"[i == n - 1];
            }
        } else {
            std::vector<int> seq(n);
            for (auto &i : seq) {
                std::cin >> i;
                i--;
            }

            std::cout << OY::CantorRank<Z>::raw_query(seq.begin(), seq.end()) + 1 << '\n';
        }
    }

    return 0;
}
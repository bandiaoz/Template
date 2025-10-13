#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"

/*
[Suffix Array](https://judge.yosupo.jp/problem/suffixarray)(https://github.com/yosupo06/library-checker-problems/issues/50)
[status](https://judge.yosupo.jp/submission/319123)
*/
/**
 * 本题为后缀数组模板
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    OY::SA::SuffixArray<false, false, 500'000> sa(s);
    for (int i = 0; i < s.size(); i++) {
        std::cout << sa.query_sa(i) << " \n"[i == s.size() - 1];
    }

    return 0;
}
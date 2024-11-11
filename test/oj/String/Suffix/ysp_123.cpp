#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"

using ll = long long;

/*
[Number of Substrings](https://judge.yosupo.jp/problem/number_of_substrings)(https://github.com/yosupo06/library-checker-problems/issues/123)
[status](https://judge.yosupo.jp/submission/248321)
*/
/**
 * 本题为后缀数组算法模板，计算字符串本质不同的子串数量
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    int n = s.size();

    OY::SA::SuffixArray<true, true, 500000> SA(s.begin(), s.end(), 128);
    ll ans = 0;
    for (int i = 0; i < n; i++) {
        int can = n - SA.query_sa(i);
        int h = SA.query_height(i);
        ans += can - h;
    }

    std::cout << ans << "\n";

    return 0;
}
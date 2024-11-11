#include <bits/stdc++.h>
#include "src/String/Manacher.h"

/*
[P3805 【模板】manacher](https://www.luogu.com.cn/problem/P3805)
[status](https://www.luogu.com.cn/record/179579064)
*/
/**
 * 本题为回文串模板题，求字符串的最长回文子串长度，可以有多种做法
 * 首先它是一道马拉车模板题
 * 也可以用哈希解决
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    OY::Manacher<true, true> manacher(s);
    int ans = 0;
    for (int i = 0; i < s.size(); i++) {
        ans = std::max<int>(ans, manacher.query_max_odd(i) * 2 + 1);
        if (i + 1 < s.size()) {
            ans = std::max<int>(ans, manacher.query_max_even(i, i + 1) * 2);
        }
    }

    std::cout << ans << '\n';

    return 0;
}
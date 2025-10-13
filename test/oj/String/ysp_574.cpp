#include <bits/stdc++.h>
#include "src/String/Manacher.h"

/*
[Enumerate Palindromes](https://judge.yosupo.jp/problem/enumerate_palindromes)(https://github.com/yosupo06/library-checker-problems/issues/574)
[status](https://judge.yosupo.jp/submission/319128)
*/
/*
给定一个字符串，求出以每个位置为中心的回文串的长度

Manacher 算法
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    OY::Manacher<true, true> manacher(s);
    for (int i = 0; i < s.size(); i++) {
        std::cout << manacher.query_max_odd_arm(i) * 2 + 1 << ' ';
        if (i + 1 < s.size()) {
            std::cout << manacher.query_max_even_arm(i, i + 1) * 2 << ' ';
        }
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/String/KMP.h"

/*
[P3375 【模板】KMP](https://www.luogu.com.cn/problem/P3375)
[status](https://www.luogu.com.cn/record/196672242)
*/
/**
 * 给定两个字符串 `t` 和 `s`，求 `s` 在 `t` 中所有出现的位置
 * 
 * 本题作为字符串模板题，有多种解法
 * 首先，肯定可以用 kmp算法 求出前缀函数
 * 其次，可以使用 Z 算法求出 Z 函数，间接求出前缀函数
 * 当然也可以用字符串哈希做
 */

void solve_kmp() {
    std::string t, s;
    std::cin >> t >> s;

    OY::KMP_string kmp(s);
    for (int i = 0, pi = 0; i < t.size(); i++, pi = kmp.jump(pi, t[i])) {
        if (s[pi] == t[i]) pi++;
        if (pi == kmp.size()) {
            std::cout << i - s.size() + 2 << '\n';
        }
    }
    for (int i = 0; i < s.size(); i++) {
        std::cout << kmp.query_Pi(i) << " \n"[i == s.size() - 1];
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_kmp();

    return 0;
}
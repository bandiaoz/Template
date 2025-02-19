#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"

/*
[Longest Common Substring](https://judge.yosupo.jp/problem/longest_common_substring)(https://github.com/yosupo06/library-checker-problems/issues/889)
[status](https://judge.yosupo.jp/submission/255742)
*/
/**
 * 求两个字符串的最长公共子串
 * 将两个字符串拼接起来，只检查「排名相邻」并且「来自两个不同串」的两个后缀的 lcp
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s, t;
    std::cin >> s >> t;
    int n = s.size();
    int m = t.size();

    OY::SA::SuffixArray<true, true, 1'000'001> sa(s + "#" + t);
    int ans = 0;
    int begin_s = 0, begin_t = 0;
    for (int i = 1; i < n + m + 1; i++) {
        if (sa.query_sa(i) < n && sa.query_sa(i - 1) > n && sa.query_height(i) > ans) {
            ans = sa.query_height(i);
            begin_s = sa.query_sa(i);
            begin_t = sa.query_sa(i - 1) - n - 1;
        }
        if (sa.query_sa(i) > n && sa.query_sa(i - 1) < n && sa.query_height(i) > ans) {
            ans = sa.query_height(i);
            begin_s = sa.query_sa(i - 1);
            begin_t = sa.query_sa(i) - n - 1;
        }
    }

    std::cout << begin_s << ' ' << begin_s + ans << ' ' << begin_t << ' ' << begin_t + ans << '\n';

    return 0;
}
#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"

/*
[LCS - Longest Common Substring](https://www.luogu.com.cn/problem/SP1811)
[status](https://www.luogu.com.cn/record/192533943)
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

    OY::SA::SuffixArray<true, true, 500001> sa(s + "#" + t);
    int ans = 0;
    for (int i = 1; i < n + m + 1; i++) {
        if (sa.query_sa(i) < n && sa.query_sa(i - 1) > n || sa.query_sa(i) > n && sa.query_sa(i - 1) < n) {
            ans = std::max<int>(ans, sa.query_height(i));
        }
    }

    std::cout << ans << '\n';

    return 0;
}
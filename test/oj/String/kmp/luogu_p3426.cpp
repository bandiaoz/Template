#include <bits/stdc++.h>
#include "src/String/KMP.h"

using ll = long long;

/*
[P3426 [POI2005] SZA-Template](https://www.luogu.com.cn/problem/P3426)
[status](https://www.luogu.com.cn/record/196849670)
*/
/**
 * 有一个长度为 n 的字符串，求长度最短的印章字符串，使得印章覆盖整个字符串
 * 印章可以重叠，但是要求重叠部分字符相同，例如 aba 印章可以覆盖 ababa
 * 
 * 印章串一定是原串的一个 border，并且印章串在原串的出现位置相邻距离不超过印章串长度
 * 定义 ans[i] 为长度为 i 的前缀的最小印章长度，则 ans[i] = i or ans[m_pi[i]]
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    int n = s.size();

    OY::KMP_string kmp(s);

    std::vector<int> ans(n + 1), f(n + 1);
    ans[1] = 1;
    f[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (f[ans[kmp.query_Pi(i - 1)]] >= i - kmp.query_Pi(i - 1)) {
            ans[i] = ans[kmp.query_Pi(i - 1)];
        } else {
            ans[i] = i;
        }
        f[ans[i]] = i;
    }

    std::cout << ans[n] << "\n";

    return 0;
}
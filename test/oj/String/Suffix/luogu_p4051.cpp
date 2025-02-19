#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"

/*
[P4051 [JSOI2007] 字符加密](https://www.luogu.com.cn/problem/P4051)
[status](https://www.luogu.com.cn/record/192124628)
*/
/**
 * 本题求所有循环同构的字典序顺序
 * 
 * 先将字符串复制一份，进行后缀排序，循环同构的所有 $n$ 个字符串起始位置为 $[0, n)$，只取长度为 $n$ 的子串，排序不变
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    int n = s.size();

    OY::SA::SuffixArray<false, false, 200000> SA(s + s);
    std::string ans;
    for (int i = 0; i < n * 2; i++) {
        int pos = SA.query_sa(i);
        if (pos < n) {
            ans += s[(pos - 1 + n) % n];
        }
    }

    std::cout << ans << "\n";

    return 0;
}
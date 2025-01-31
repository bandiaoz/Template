#include <bits/stdc++.h>
#include "src/String/KMP.h"

using ll = long long;

/*
[P3435 [POI2006] OKR-Periods of Words](https://www.luogu.com.cn/problem/P3435)
[status](https://www.luogu.com.cn/record/196682247)
*/
/**
 * 对于一个字符串 s，如果 Q 是 s 的真前缀，并且 s 是 Q+Q 的前缀，则称 Q 是 s 的周期。
 * 求一个字符串所有前缀的最大周期长度之和。
 * 
 * 最大周期长度 = |s| - |s| 的最小 border 长度
 * 在失配树上，找到离根最近的祖先，就是最小 border 长度，进行验证即可，要求周期长度的两倍大于等于串长
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::string s;
    std::cin >> s;

    OY::KMP_string kmp(s);
    std::vector<int> fa(n + 1);
    for (int i = 1; i <= n; i++) {
        fa[i] = kmp.query_Pi(i - 1);
        if (fa[i] != 0 && fa[fa[i]] != 0) {
            fa[i] = fa[fa[i]];
        }
    }

    ll ans = 0;
    for (int i = 1; i <= n; i++) {
        int border = fa[i];
        int len = i - border;
        if (border && len * 2 >= i) {
            ans += len;
        }
    }

    std::cout << ans << "\n";

    return 0;
}
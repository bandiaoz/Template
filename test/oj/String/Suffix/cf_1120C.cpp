#include <bits/stdc++.h>
#include "src/String/Suffix/SALCP.h"

/*
[C. Compress String](https://codeforces.com/contest/1120/problem/C)
[status](https://codeforces.com/contest/1120/submission/301197538)
*/
/**
 * 每次添加一个字符代价为 x，添加一个已经存在的子串代价为 y
 * 求获得字符串 s 的最小代价
 * 
 * 考虑 dp 转移，dp[i] 表示前 i 个字符的最小代价
 * 这个状态可以通过 x 的代价从 dp[i - 1] 转移过来
 * 并且由于 dp 数组是不降的，所以枚举 j，考虑以 i 结尾的最大 lcs，花费 y 的代价进行转移
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, x, y;
    std::cin >> n >> x >> y;
    std::string s;
    std::cin >> s;

    auto sa = OY::make_SA_LCP<5000>(s);

    const int inf = 1e9;
    std::vector<int> dp(n + 1, inf);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        dp[i + 1] = std::min(dp[i + 1], dp[i] + x);
        for (int j = 0; j < i; j++) {
            if (int max = sa.lcs(j, i, std::min(j + 1, i - j)); max > 0) {
                dp[i + 1] = std::min(dp[i + 1], dp[i + 1 - max] + y);
            }
        }
    }

    std::cout << dp[n] << '\n';

    return 0;
}
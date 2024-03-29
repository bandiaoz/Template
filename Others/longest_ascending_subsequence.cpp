// test problem: https://www.luogu.com.cn/problem/B3637
#include <bits/stdc++.h>

using ll = long long;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (int &i : a) {
        std::cin >> i;
    }

    int ans = 0;
    const int inf = 1e9;
    // 先全部填 inf
    std::vector<int> dp(n, inf);
    for (int i = 0; i < n; i++) {
        // 找到第一个大于等于 a[i] 的位置，然后替换，此时 dp 是递增的，最长递增子序列就是 dp 中不为 inf 的个数
        *std::upper_bound(dp.begin(), dp.end(), a[i]) = a[i];
        ans = std::max<int>(ans, std::lower_bound(dp.begin(), dp.end(), inf) - dp.begin());
    }

    std::cout << ans << "\n";

    return 0;
}
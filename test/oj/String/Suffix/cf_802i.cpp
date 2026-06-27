#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"

using ll = long long;

/*
[CF802I Fake News (hard)](https://codeforces.com/contest/802/problem/I)
[status](https://codeforces.com/contest/802/submission/337521906)
*/
/*
给定字符串 $$s(1 \leq |s| \leq 10^5)$$，求所有子串的出现次数的平方和，即 $$\sum_{p}\mathrm{cnt}(s, p)^2$$
共 $$t(1 \leq t \leq 10)$$ 组询问

如果对于子串 $$p$$ 出现 $$x$$ 次，那么再出现一次贡献为 $$(x + 1)^2 - x^2 = 2x + 1$$

一共有 $$\frac{n(n + 1)}{2}$$ 个子串，所以后面 $$+1$$ 的贡献为 $$\frac{n(n + 1)}{2}$$

前面一项意味着子串 $$p$$ 之前出现了 $$x$$ 次，会造成 $$2x$$ 的贡献。按照后缀 rank 顺序枚举，子串 $$p$$ 之前出现次数等于之前连续的 $$height$$ 数组的值大于等于 $$p$$

那么 $$\sum 2x = \sum \min(height[i...j])$$，总贡献即为 $$\frac{n(n + 1)}{2} + \sum \min(height[i...j])$$
*/

void solve() {
    std::string s;
    std::cin >> s;

    OY::SA::SuffixArray<true, true, 100000> sa(s);
    int n = s.size();

    std::vector<int> stk;
    std::vector<int> left(n), right(n);
    for (int i = 0; i < n; i++) {
        while (stk.size() && sa.query_height(stk.back()) >= sa.query_height(i)) {
            stk.pop_back();
        }
        left[i] = stk.empty() ? 0 : stk.back();
        stk.push_back(i);
    }

    stk.clear();
    for (int i = n - 1; i >= 0; i--) {
        while (stk.size() && sa.query_height(stk.back()) > sa.query_height(i)) {
            stk.pop_back();
        }
        right[i] = stk.empty() ? n : stk.back();
        stk.push_back(i);
    }

    ll ans = 1LL * n * (n + 1) / 2;
    for (int i = 1; i < n; i++) {
        ans += 2LL * sa.query_height(i) * (right[i] - i) * (i - left[i]);
    }

    std::cout << ans << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}
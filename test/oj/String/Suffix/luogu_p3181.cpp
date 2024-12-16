#include <bits/stdc++.h>
#include "src/String/Suffix/SuffixArray.h"
using ll = long long;

/*
[P3181 [HAOI2016] 找相同字符](https://www.luogu.com.cn/problem/P3181)
[status](https://www.luogu.com.cn/record/192433865)
*/
/**
 * 给出两个字符串，各取一个子串，使得两个子串相同，求方案数
 * 将两个字符串拼接起来：st = s + '#' + t，求来自不同字符串的任意两个后缀的 lcp 之和，通过容斥计算即可 work(st) - work(s) - work(t)
 * 而求一个字符串的任意两个后缀的 lcp 之和，可以通过单调栈算贡献得到
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s, t;
    std::cin >> s >> t;
    int n = s.size();
    int m = t.size();
    
    // 求任意两个后缀的 lcp 之和
    auto work = [&](std::string s) {
        int n = s.size();
        OY::SA::SuffixArray<true, true, 500000> sa(s.begin(), s.end());
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

        ll ans = 0;
        for (int i = 1; i < n; i++) {
            ans += 1LL * (i - left[i]) * (right[i] - i) * sa.query_height(i);
        }

        return ans;
    };

    ll ans = work(s + '#' + t) - work(s) - work(t);
    std::cout << ans << "\n";

    return 0;
}
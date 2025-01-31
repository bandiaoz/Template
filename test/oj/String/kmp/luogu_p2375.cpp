#include <bits/stdc++.h>
#include "src/String/KMP.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::mint1000000007;

/*
[P2375 [NOI2014] 动物园](https://www.luogu.com.cn/problem/P2375)
[status](https://www.luogu.com.cn/record/200963909)
*/
/**
 * 给定字符串 s，对于每一个长度为 i 的前缀，求出长度不超过 i / 2 的 border 的数量，记 num[i]
 * 输出 \prod_{i=1}^{n} (num_i + 1) 即可
 * 
 * 倍增向上找深度最小的长度超过 i / 2 的祖先，然后找到这个祖先的父亲，就是最长的满足条件的 border，数量就是深度，复杂度为 O(n \log n)
 * 
 * 或者使用类似 kmp 的跳跃，对于 s[0...i]，找到最长的满足条件的 border，即 s.substr(0, j)，满足 j * 2 <= i + 1，此时 num[i] = dep[j]
 * 复杂度为 O(n)
 */

void solve() {
    std::string s;
    std::cin >> s;
    int n = s.size();

    OY::KMP_string kmp(s);

    std::vector<int> dep(n + 1);
    for (int i = 1; i <= n; i++) {
        dep[i] = dep[kmp.query_Pi(i - 1)] + 1;
    }

    Z ans = 1;
    for (int i = 0, j = 0; i < n; i++) {
        j = kmp.jump(j, s[i]);
        if (s[j] == s[i]) j++;
        while (j * 2 > i + 1) j = kmp.query_Pi(j - 1);
        ans *= dep[j] + 1;
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
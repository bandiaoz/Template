#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P3041 [USACO12JAN] Video Game G](https://www.luogu.com.cn/problem/P3041)
[status](https://www.luogu.com.cn/record/202840714)
*/
/**
 * 有 n 个模式串，构造长度为 k 的文本串，尽可能多的可重叠匹配模式串，问最多可以匹配多少次。字符集为 ABC。
 * 
 * 处理文本串走到每一个状态可以获得多少次匹配，然后 dp 转移即可，注意 fail 树上从上到下传递 cnt 值
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;

    OY::ACAM<3> ac;
    const int N = n * 15; // 300
    ac.reserve(N);
    std::vector<int> cnt(N + 1);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        int p = ac.insert(s.begin(), s.end(), [&](char c) { return c - 'A'; });
        cnt[p]++;
    }

    ac.prepare();

    ac.do_for_extending_nodes([&](int u) {
        cnt[u] += cnt[ac.query_fail(u)];
    });

    std::vector<int> dp(ac.m_data.size(), -1);
    dp[0] = 0;
    for (int i = 0; i < k; i++) {
        std::vector<int> ndp(ac.m_data.size(), -1);
        for (int j = 0; j < ac.m_data.size(); j++) {
            if (dp[j] == -1) continue;
            for (int c = 0; c < 3; c++) {
                int next = ac.next(j, c);
                ndp[next] = std::max(ndp[next], dp[j] + cnt[next]);
            }
        }
        dp = std::move(ndp);
    }

    std::cout << std::ranges::max(dp) << '\n';

    return 0;
}
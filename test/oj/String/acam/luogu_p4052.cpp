#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::StaticModInt32<10'007, true>;

/*
[P4052 [JSOI2007] 文本生成器](https://www.luogu.com.cn/problem/P4052)
[status](https://www.luogu.com.cn/record/202583794)
*/
/**
 * 给定 n 个可识别的单词，求有多少个不同的长度为 m 的文本串，使得文本串中至少包含一个可识别的单词
 * 
 * ac 自动机上 dp，dp[i][j] 表示长度为 i 的文本串，在 ac 自动机上走到 j 节点并且不包含任何可识别的单词的方案数
 * 注意 trie 树上合法状态的子节点也是合法状态，如果 fail 指针指向的点是合法状态，那么这个点也是合法状态
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    const int N = n * 100;
    OY::ACAM<26> ac;
    ac.reserve(N);
    std::vector<int> identify(N + 1);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        int p = ac.insert(s.begin(), s.end(), [](char c) { return c - 'A'; });
        identify[p] = 1;
    }

    ac.prepare([&](int u, int v) {
        identify[u] |= identify[ac.query_fail(u)];
        identify[v] |= identify[u];
    });

    std::vector<Z> dp(N + 1);
    dp[0] = 1;
    for (int i = 0; i < m; i++) {
        std::vector<Z> ndp(N + 1);
        for (int j = 0; j <= N; j++) {
            if (identify[j]) continue;
            for (int c = 0; c < 26; c++) {
                int next = ac.next(j, c);
                if (identify[next]) continue;
                ndp[next] += dp[j];
            }
        }
        dp = std::move(ndp);
    }

    std::cout << Z(26).pow(m) - std::accumulate(dp.begin(), dp.end(), Z(0)) << '\n';

    return 0;
}
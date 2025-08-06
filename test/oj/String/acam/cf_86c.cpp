#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Math/Modular/StaticModInt32.h"

using Z = OY::StaticModInt32<1'000'000'009, true>;

/*
[CF86C Genetic engineering](https://codeforces.com/contest/86/problem/C)
[status](https://codeforces.com/contest/86/submission/329278523)
*/
/*
给出 $$m(1 \leq m \leq 10)$$ 个模式串，长度不超过 10，一个文本串是好的，当且仅当每个位置都至少被一个模式串覆盖（可以重叠）
例如 CATACT 被 CAT 和 TACT 覆盖
求所有长度为 $$n(1 \leq n \leq 1000)$$ 的好文本串个数
字符串仅包含 $$\tt ATGC$$ 四种字符

ac 自动机上 dp，dp[i][j][k] 表示长度为 i 的文本串，在 ac 自动机上走到 k，且后 j 个字符没有匹配的方案数
如果 $$len_k \geq j + 1$$，则可以连带这个字符一起匹配，dp[i + 1][0][k] += dp[i][j][k]
否则，积累一个不能匹配的字符，dp[i + 1][j + 1][k] += dp[i][j][k]

时间复杂度 $$O(|\Sigma| nmN)$$，其中 $$|\Sigma| = 4$$，$$N = \sum |s_i|$$
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::ACAM<4> ac;
    const int N = 100;
    ac.reserve(N);
    auto mapping = [](char c) {
        static const std::string charset = "ATGC";
        return charset.find(c);
    };
    
    std::vector<int> len(N + 1);
    for (int i = 0; i < m; i++) {
        std::string s;
        std::cin >> s;
        int p = ac.insert(s.begin(), s.end(), mapping);
        len[p] = s.size();
    }

    ac.prepare([&](int u, int v) { 
        len[u] = std::max(len[u], len[ac.query_fail(u)]);
    });

    const int LEN = 10;
    std::vector dp(n + 1, std::vector(LEN + 1, std::vector<Z>(ac.m_data.size())));
    dp[0][0][0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= LEN; j++) {
            for (int k = 0; k < ac.m_data.size(); k++) {
                for (int c = 0; c < 4; c++) {
                    if (int next = ac.next(k, c); len[next] > j) {
                        dp[i + 1][0][next] += dp[i][j][k];
                    } else if (j + 1 <= LEN) {
                        dp[i + 1][j + 1][next] += dp[i][j][k];
                    }
                }
            }
        }
    }

    std::cout << std::accumulate(dp[n][0].begin(), dp[n][0].end(), Z(0)) << '\n';

    return 0;
}
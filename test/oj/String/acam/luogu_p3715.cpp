#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Math/Modular/StaticModInt32.h"
#include "src/Math/LinearAlgebra/DynamicMatrix.h"

using Z = OY::mint1000000007;

/*
[P3715 [BJOI2017] 魔法咒语](https://www.luogu.com.cn/problem/P3715)
[status](https://www.luogu.com.cn/record/202718806)
*/
/**
 * 给定 n 个基本词汇 s_i 和 m 个禁咒 t_i，用这些基本词汇拼出长度恰好为 l 的咒语，并且不能包含禁咒
 * 问有多少种不同的咒语
 * 数据范围：l <= 100 或 l <= 10^8 并且 |s_i| <= 2
 * 
 * 先用 m 个禁咒建一个 AC 自动机，标记出所有禁咒节点
 * 然后对于每一个合法节点，枚举 n 个基本词汇，预处理出转移到的下一个节点，next[j] = {p, len} 表示 j 节点可以通过长度为 len 的基本词汇转移到 p 节点
 * 如果 l <= 100，dp[i][j] 表示长度为 i 的咒语以 j 节点结尾的方案数，通过 next 数组转移
 * 如果 l > 100，则需要矩阵快速幂优化，sz 为自动机节点数量，构造大小为 2 * sz 的转移矩阵
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, l;
    std::cin >> n >> m >> l;

    OY::ACAM<26> ac;
    const int N = 100;
    ac.reserve(N);
    std::vector<std::string> basic(n);
    std::vector<int> forbid(N + 1);
    for (auto &s : basic) {
        std::cin >> s;
    }
    for (int i = 0; i < m; i++) {
        std::string t;
        std::cin >> t;
        int p = ac.insert_lower(t);
        forbid[p] = 1;
    }

    ac.prepare([&](int u, int v) {
        forbid[u] |= forbid[ac.query_fail(u)];
        forbid[v] |= forbid[u];
    });

    std::vector<std::vector<std::pair<int, int>>> next(ac.m_data.size()), next2(ac.m_data.size());
    for (int j = 0; j < ac.m_data.size(); j++) {
        if (forbid[j]) continue;
        for (auto s : basic) {
            int p = j;
            for (auto c : s) {
                p = ac.next(p, c - 'a');
                if (forbid[p]) {
                    p = -1;
                    break;
                }
            }
            if (p != -1) {
                next[j].emplace_back(p, s.size());
                next2[p].emplace_back(j, s.size());
            }
        }
    }

    auto solve_dp = [&]() {
        std::vector dp(l + 1, std::vector<Z>(ac.m_data.size()));
        dp[0][0] = 1;
        for (int i = 1; i <= l; i++) {
            for (int j = 0; j < ac.m_data.size(); j++) {
                if (forbid[j]) continue;
                for (auto [p, len] : next2[j]) {
                    if (i >= len) {
                        dp[i][j] += dp[i - len][p];
                    }
                }
            }
        }
        std::cout << std::accumulate(dp[l].begin(), dp[l].end(), Z(0)) << '\n';
    };
    auto solve_matrix = [&]() {
        const uint32_t sz = ac.m_data.size();
        OY::DynamicMatrix<Z> mat(sz * 2, sz * 2);
        
        for (int j = 0; j < sz; j++) {
            for (auto [p, len] : next2[j]) { // p -> j
                if (len == 2) {
                    mat[j + sz][p]++;
                } else {
                    mat[j + sz][p + sz]++;
                }
            }
        }
        for (int i = 0; i < sz; i++) {
            mat[i][i + sz] = 1;
        }
        
        OY::DynamicMatrix<Z> dp(sz * 2, 1);
        dp[0][0] = 1;
        for (auto [p, len] : next[0]) {
            if (len == 1) {
                dp[p + sz][0]++;
            }
        }
        auto res = mat.pow(l) * dp;

        Z ans = 0;
        for (int i = 0; i < sz; i++) {
            ans += res[i][0];
        }
        std::cout << ans << '\n';
    };

    if (l <= 100) {
        solve_dp();
    } else {
        solve_matrix();
    }

    return 0;
}
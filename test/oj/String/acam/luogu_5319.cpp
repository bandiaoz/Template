#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P5319 [BJOI2019] 奥术神杖](https://www.luogu.com.cn/problem/P5319)
[status](https://www.luogu.com.cn/record/202874358)
*/
/**
 * 有长度为 n 的文本串和 m 个模式串，每个模式串有一个权值。文本串每可重叠匹配一次模式串，可以获得一次权值。
 * 最大化文本串权值的几何平均值，即最大化 $\sqrt[k]{\prod_{i=1}^{k} val_i}$，其中 $val_i$ 每次匹配的权值。输出方案。
 * 
 * V = \sqrt[k]{\prod_{i=1}^{k} val_i}
 * 取对数，得到 \log V = \frac{1}{k} \sum_{i=1}^{k} \log val_i
 * 二分答案，\sum (\log val_i - mid) \geq 0
 * 建出 ac 自动机，每个模式串权值为 \log val_i - mid，判断权值总和是否大于等于 0
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::string text;
    std::cin >> text;

    const int N = 1501;
    OY::ACAM<10> ac;
    ac.reserve(N);
    std::vector<double> val(N + 1);
    std::vector<int> cnt(N + 1);
    for (int i = 0; i < m; i++) {
        std::string s;
        int v;
        std::cin >> s >> v;
        int p = ac.insert(s.begin(), s.end(), [&](char c) { return c - '0'; });
        cnt[p]++;
        val[p] = std::log(v);
    }

    ac.prepare();

    ac.do_for_extending_nodes([&](int u) {
        val[u] += val[ac.query_fail(u)];
        cnt[u] += cnt[ac.query_fail(u)];
    });

    auto judge = [&](double mid) -> std::pair<bool, std::string> {
        std::vector dp(n + 1, std::vector(ac.m_data.size(), -1e18));
        std::vector pre(n + 1, std::vector(ac.m_data.size(), std::pair{-1, '#'}));
        dp[0][0] = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < ac.m_data.size(); j++) {
                int min = text[i] == '.' ? 0 : text[i] - '0';
                int max = text[i] == '.' ? 9 : text[i] - '0';
                for (int c = min; c <= max; c++) {
                    int next = ac.next(j, c);
                    if (dp[i + 1][next] < dp[i][j] + val[next] - mid * cnt[next]) {
                        dp[i + 1][next] = dp[i][j] + val[next] - mid * cnt[next];
                        pre[i + 1][next] = {j, c + '0'};
                    }
                }
            }
        }
        for (int i = 0; i < ac.m_data.size(); i++) {
            if (dp[n][i] > 0) {
                std::string ans;
                for (int j = i, len = n; pre[len][j].second != '#'; j = pre[len][j].first, len--) {
                    ans.push_back(pre[len][j].second);
                }
                std::reverse(ans.begin(), ans.end());
                return {true, ans};
            }
        }
        return {false, ""};
    };
    
    double l = 0, r = std::log(1e9);
    while (r - l > 1e-6) {
        double mid = (l + r) / 2;
        if (auto [ok, ans] = judge(mid); ok) {
            l = mid;
        } else {
            r = mid;
        }
    }

    std::cout << judge(l).second << "\n";

    return 0;
}
#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P3796 【模板】AC 自动机（加强版）](https://www.luogu.com.cn/problem/P3796)
[status](https://www.luogu.com.cn/record/197125915)
*/
/**
 * 有 n 个模式串，一个文本串，找出哪些模式串在文本串中出现次数最多
 * 
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，按照 fail 序求前缀和，即为子串出现次数
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    while (std::cin >> n && n) {
        OY::ACAM<26> ac;
        const int N = 70 * n;
        ac.reserve(N);
        std::vector<int> cnt(N + 1), index(N + 1, -1);
        std::vector<std::string> words(n);
        for (int i = 0; i < n; i++) {
            std::cin >> words[i];
            int p = ac.insert_lower(words[i]);
            index[p] = i;
        }

        ac.prepare();

        std::string t;
        std::cin >> t;
        for (int i = 0, j = 0; i < t.size(); i++) {
            j = ac.next(j, t[i] - 'a');
            cnt[j]++;
        }

        int max = 0;
        std::vector<int> ans;
        ac.do_for_failing_nodes([&](int u) {
            // 在 t 中出现过并且是模式串
            if (cnt[u] && index[u] != -1) {
                if (cnt[u] > max) {
                    max = cnt[u];
                    ans = {index[u]};
                } else if (cnt[u] == max) {
                    ans.push_back(index[u]);
                }
            }
            if (cnt[u]) {
                cnt[ac.query_fail(u)] += cnt[u];
            }
        });

        std::sort(ans.begin(), ans.end());
        std::cout << max << "\n";
        for (auto i : ans) {
            std::cout << words[i] << "\n";
        }

    }

    return 0;
}
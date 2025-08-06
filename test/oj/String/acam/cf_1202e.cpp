#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

using ll = long long;

/*
[CF1202E You Are Given Some Strings...](https://codeforces.com/contest/1202/problem/E)
[status](https://codeforces.com/contest/1202/submission/329014691)
*/
/*
给定字符串 text 和 n 个字符串 $$s_i$$
令 $$f(t, s)$$ 表示 s 在 t 中的出现次数，求 $$\sum_{i = 1}^{n} \sum_{j = 1}^{n} f(text, s_i + s_j)$$，其中 $$s_i + s_j$$ 表示字符串拼接

枚举 text 的每个位置，计算以该位置为结尾的子串的贡献
对 text 和每个 s_i 翻转，然后求出以该位置为开头的子串的贡献
两个贡献做乘积即可
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;

    int n;
    std::cin >> n;
    std::vector<std::string> a(n);
    for (auto &s : a) {
        std::cin >> s;
    }

    auto work = [&]() {
        OY::ACAM<26> ac;
        const int N = 200000;
        ac.reserve(N);
        std::vector<int> cnt(N + 1);
        for (auto &s : a) {
            int p = ac.insert_lower(s);
            cnt[p]++;
        }
        ac.prepare();
        ac.do_for_extending_nodes([&](int u) {
            cnt[u] += cnt[ac.query_fail(u)];
        });

        std::vector<int> res(text.size() + 1);
        for (int p = 0, i = 0; i < text.size(); i++) {
            p = ac.next(p, text[i] - 'a');
            res[i + 1] = cnt[p];
        }
        return res;
    };
    auto left = work();
    std::reverse(text.begin(), text.end());
    for (auto &s : a) {
        std::reverse(s.begin(), s.end());
    }
    auto right = work();

    ll ans = 0;
    for (int i = 1; i < text.size(); i++) {
        ans += 1LL * left[i] * right[text.size() - i];
    }

    std::cout << ans << '\n';

    return 0;
}
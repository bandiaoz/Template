#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P3966 [TJOI2013] 单词](https://www.luogu.com.cn/problem/P3966)
[status](https://www.luogu.com.cn/record/201435354)
*/
/**
 * 有 n 个单词，求每个单词在所有单词中的出现次数（可以认为文本串是 word[1] + '#' + word[2] + '#' + ... + word[n]）
 * 
 * 对于 words[i]，统计所有单词在 words[i] 中出现的次数
 * 注意需要在 fail 树上做前缀和
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::ACAM<26> ac;
    const int N = 1'000'000;
    ac.reserve(N);

    std::vector<int> cnt(N + 1), pos(n);
    std::vector<std::string> words(n);
    for (int i = 0; i < n; i++) {
        std::cin >> words[i];
        int p = ac.insert_lower(words[i]);
        pos[i] = p;
    }

    ac.prepare();

    for (auto &word : words) {
        for (int i = 0, j = 0; i < word.size(); i++) {
            j = ac.next(j, word[i] - 'a');
            cnt[j]++;
        }
    }

    ac.do_for_failing_nodes([&](int u) {
        cnt[ac.query_fail(u)] += cnt[u];
    });

    for (int i = 0; i < n; i++) {
        std::cout << cnt[pos[i]] << "\n";
    }

    return 0;
}
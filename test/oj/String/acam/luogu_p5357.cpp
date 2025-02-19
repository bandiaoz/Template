#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P5357 【模板】AC 自动机](https://www.luogu.com.cn/problem/P5357)
[status](https://www.luogu.com.cn/record/197125921)
*/
/**
 * 给定 n 个模式串，一个文本串，分别求每个模式串在文本串中的出现次数
 * 
 * 本题为 ac 自动机模板题
 * 建好 ac 自动机之后，在文本串上走一遍，求出每个状态的匹配次数
 * 如果某个状态匹配上了，那么这个状态的所有 fail 状态也匹配上了，因此需要按照 fail 序求前缀和，即为所有子串出现次数
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    OY::ACAM<26> ac;
    const int N = 200000;
    ac.reserve(N);
    std::vector<int> cnt(N + 1), pos(n);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        pos[i] = ac.insert_lower(s);
    }
    ac.prepare();

    std::string text;
    std::cin >> text;

    for (int i = 0, j = 0; i < text.size(); i++) {
        j = ac.next(j, text[i] - 'a');
        cnt[j]++;
    }

    ac.do_for_failing_nodes([&](int u) {
        cnt[ac.query_fail(u)] += cnt[u];
    });

    for (int i = 0; i < n; i++) {
        std::cout << cnt[pos[i]] << "\n";
    }

    return 0;
}
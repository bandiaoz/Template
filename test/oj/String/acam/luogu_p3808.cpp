#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P3808 AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
[status](https://www.luogu.com.cn/record/197125869)
[status](https://www.luogu.com.cn/record/224493268)
*/
/**
 * 给定 n 个模式串，一个文本串，求有多少个不同的模式串在文本串中出现
 * 两个模式串不同当且仅当他们编号不同
 * 
 * 本题为 ac 自动机模板题
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::ACAM<26> ac;
    const int N = 1000000;
    ac.reserve(N);
    std::vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        pos[i] = ac.insert_lower(s);
    }

    ac.prepare();

    std::string text;
    std::cin >> text;

    std::vector<int> cnt(N + 1);
    for (int i = 0, j = 0; i < text.size(); i++) {
        j = ac.next(j, text[i] - 'a');
        cnt[j]++;
    }

    ac.do_for_failing_nodes([&](int u) {
        cnt[ac.query_fail(u)] += cnt[u];
    });

    std::cout << std::count_if(pos.begin(), pos.end(), [&](int p) { return cnt[p] != 0; }) << '\n';

    return 0;
}
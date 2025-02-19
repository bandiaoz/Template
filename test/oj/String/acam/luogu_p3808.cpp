#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P3808 AC 自动机（简单版）](https://www.luogu.com.cn/problem/P3808)
[status](https://www.luogu.com.cn/record/197125869)
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
    std::vector<int> cnt(N + 1);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        int p = ac.insert_lower(s);
        cnt[p]++;
    }

    ac.prepare();

    std::string text;
    std::cin >> text;

    int ans = 0;
    for (int i = 0, j = 0; i < text.size(); i++) {
        j = ac.next(j, text[i] - 'a');
        // 可以边搜边改，搜过的就清空，以避免重复统计；同时也保证了时间复杂度
        for (int x = j; x && cnt[x]; x = ac.query_fail(x)) {
            ans += cnt[x];
            cnt[x] = 0;
        }
    }

    std::cout << ans << '\n';

    return 0;
}
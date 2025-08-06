#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"

/*
[P2292 [HNOI2004] L 语言](https://www.luogu.com.cn/problem/P2292)
[status](https://www.luogu.com.cn/record/202399625)
*/
/**
 * 有一个字典 D，里面有 $$n (1 \leq n \leq 20)$$ 个单词，每个单词 $$s_i (1 \leq |s_i| \leq 20)$$
 * 有 $$m (1 \leq m \leq 50)$$ 次询问，每次给出一个文本串 $$t (1 \leq |t| \leq 2 \times 10^6)$$，问在字典 D 下，可以理解文本串的最长前缀长度，也就是这个前缀由给定的单词拼接而成
 * 
 * 本题为多模式字符串匹配，使用 ac 自动机
 * 记录每个节点可以匹配的单词长度，在 trie 树上从上到下继承 fail 指针的匹配长度，然后状压做 dp 即可
 */ 

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    OY::ACAM<26> ac;
    const int N = 20 * n;
    ac.reserve(N);
    std::vector<int> len(N + 1), masks(N + 1);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        auto p = ac.insert_lower(s);
        len[p] = s.size();
        masks[p] = 1 << s.size();
    }

    ac.prepare();
    ac.do_for_extending_nodes([&](int u) {
        masks[u] |= masks[ac.query_fail(u)];
    });

    while (m--) {
        std::string text;
        std::cin >> text;
        
        int ans = 0;
        // mask 的第 i 位表示 text 串向左 i 位的 dp 值
        for (int j = 0, p = 0, mask = 2; j < text.size(); j++) {
            p = ac.next(p, text[j] - 'a');
            if (mask & masks[p]) {
                mask |= 1;
                ans = j + 1;
            }
            mask <<= 1;
        }

        std::cout << ans << "\n";
    }

    return 0;
}
#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"
#ifndef LOCAL
#define debug(...)
#endif

using ll = long long;

/*
[P2922 [USACO08DEC]Secret Message G](https://www.luogu.com.cn/problem/P2922)
[status](https://www.luogu.com.cn/record/237510028)
*/
/*
有 $$m(1 \leq m \leq 5 \times 10^4)$$ 个二进制信息串，有 $$n(1 \leq n \leq 5 \times 10^4)$$ 个询问，每次一个字符串，询问有多少个信息串能匹配这个字符串，也就是一个串是另一个串的前缀。
字符串的总长度不超过 $$5 \times 10^5$$。

答案等于 trace 过程中经过的 end 加上 find 结尾的 cnt 减去重复统计的 end 节点。
*/

template <typename Node>
struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
    int cnt = 0;
    int end = 0;
};

using Node = OY::StaticTrie<NodeWrap, 2>::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int m, n;
    std::cin >> m >> n;
    OY::StaticTrie<NodeWrap, 2> trie;
    auto get = [&](int sz) {
        std::vector<int> v(sz);
        for (auto &i : v) {
            std::cin >> i;
        }
        return v;
    };
    for (int i = 0; i < m; i++) {
        int sz;
        std::cin >> sz;
        auto v = get(sz);
        trie.insert(sz, 
            [&](int i) { return v[i]; }, 
            [&](Node *node) { node->cnt++; }
        )->end++;
    }

    for (int i = 0; i < n; i++) {
        int sz;
        std::cin >> sz;
        auto v = get(sz);
        
        int ans = 0;
        trie.trace(sz, [&](int i) { return v[i]; }, 
            [&](Node *node) { 
                ans += node->end;
            });
        auto node = trie.find(sz, [&](int i) { return v[i]; });
        if (node) {
            ans += node->cnt;
            ans -= node->end;
        }
        std::cout << ans << "\n";
    }

    return 0;
}
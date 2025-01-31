#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"

using ll = long long;

/*
[P3879 [TJOI2010] 阅读理解](https://www.luogu.com.cn/problem/P3879)
[status](https://www.luogu.com.cn/record/196232387)
*/
/**
 * 有 n 篇文章，每篇文章有若干个单词
 * 给出 m 个单词，问每个单词在哪些文章中出现过，从小到大不重复地输出文章编号
 * 可以使用 Trie 树，每个单词结尾节点维护一个 vector<int>，表示该单词在哪些文章中出现过，不重复添加
 */

template <typename Node>
struct NodeWrap {
    std::vector<int> v;
};

void solve_trie() {
    int n;
    std::cin >> n;
    OY::StaticTrie<NodeWrap, 26> trie;
    for (int i = 0; i < n; i++) {
        int k;
        std::cin >> k;
        while (k--) {
            std::string s;
            std::cin >> s;
            auto it = trie.insert_lower(s);
            if (it->v.empty() || it->v.back() != i) {
                it->v.push_back(i);
            }
        }
    }

    int q;
    std::cin >> q;
    while (q--) {
        std::string s;
        std::cin >> s;
        if (auto it = trie.find_lower(s); it) {
            for (auto i : it->v) {
                std::cout << i + 1 << ' ';
            }
        }
        std::cout << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_trie();

    return 0;
}
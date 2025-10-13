#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"
#include "src/Graph/simple/TopologicalSort.h"

/*
[P3065 [USACO12DEC] First! G](https://www.luogu.com.cn/problem/P3065)
[status](https://www.luogu.com.cn/record/239723748)
*/
/*
给定 $$n$$ 个字符串，询问能否改变字母表的顺序，来使得一些字符串字典序最小
字符串长度之和不超过 $$M = 3 \times 10^5$$

建出字典树，对于每一个字符串，假设字典序最小，那么这个字符串的每个节点向兄弟节点连有向边，判断是否成环
*/

template <typename Node>
struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
};
using Trie = OY::StaticTrie<NodeWrap, 26>;
using Node = Trie::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    Trie trie;
    std::vector<std::string> words(n);
    for (int i = 0; i < n; i++) {
        auto &word = words[i];
        std::cin >> word;
        trie.insert_lower(word)->m_is_end = true;
    }

    std::vector<std::string> ans;
    for (const auto &word : words) {
        OY::TOPO::Graph topo(26);
        uint32_t prev = 0;
        bool prefix = false;
        for (int i = 0; i < word.size(); i++) {
            if (trie.get_node(prev)->m_is_end) {
                prefix = true;
                break;
            }
            int cur_char = word[i] - 'a';
            for (int i = 0; i < 26; i++) {
                if (i != cur_char && trie.get_node(prev)->get_child(i) != 0) {
                    topo.add_edge(cur_char, i);
                }
            }
            prev = trie.get_node(prev)->get_child(word[i] - 'a');
        }
        if (auto [_, ok] = topo.calc(); ok && !prefix) {
            ans.push_back(word);
        }
    }

    std::cout << ans.size() << '\n';
    for (const auto &word : ans) {
        std::cout << word << '\n';
    }

    return 0;
}
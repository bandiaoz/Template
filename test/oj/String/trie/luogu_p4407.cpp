#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"

/*
[P4407 [JSOI2009] 电子字典](https://www.luogu.com.cn/problem/P4407)
[status](https://www.luogu.com.cn/record/239728603)
*/
/*
给定 $$n(1 \leq n \leq 10^4)$$ 个单词，组成一个词典。有 $$q(1 \leq q \leq 10^4)$$ 次询问，每次询问一个单词：
1. 如果该单词在词典中出现过，输出 -1
2. 否则输出词典中有多少个单词和这个字符串编辑距离为 1
所有单词互不相同，查询的字符串可能重复，字符串长度不超过 $$20$$

在字典树上搜索即可
*/

template <typename Node>
struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
};
using Trie = OY::StaticTrie<NodeWrap, 26>;
using Node = Trie::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<std::string> words(n);
    Trie trie;
    for (auto &word : words) {
        std::cin >> word;
        trie.insert_lower(word)->m_is_end = true;
    }

    while (q--) {
        std::string s;
        std::cin >> s;

        if (auto it = trie.find_lower(s); it && it->m_is_end) {
            std::cout << "-1\n";
        } else {
            std::vector<int> ans;
            auto dfs = [&](auto &&self, uint32_t node, int i, bool modify) -> void {
                if (i == s.size() && trie.get_node(node)->m_is_end) {
                    ans.push_back(node);
                    return;
                }
                for (int j = 0; j < 26; j++) {
                    if (int next = trie.get_node(node)->get_child(j); next) {
                        if (j == s[i] - 'a') {
                            self(self, next, i + 1, modify);
                        } else if (!modify) {
                            self(self, next, i + 1, true);
                        }
                        if (!modify) {
                            self(self, next, i, true);
                        }
                    }
                }
                if (!modify) {
                    self(self, node, i + 1, true);
                }
            };
            dfs(dfs, 0, 0, false);

            std::sort(ans.begin(), ans.end());
            ans.erase(std::unique(ans.begin(), ans.end()), ans.end());
            std::cout << ans.size() << "\n";
        }
    }

    return 0;
}
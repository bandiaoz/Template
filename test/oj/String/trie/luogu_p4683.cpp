#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"

/*
[P4683 [IOI 2008] Type Printer](https://www.luogu.com.cn/problem/P4683)
[status](https://www.luogu.com.cn/record/240318073)
*/
/*
给定一个打字机，需要打印 $$n(1 \leq n \leq 2.5 \times 10^4)$$ 个单词（长度不超过 20，所有单词各不相同），有以下 3 种操作：
1. 添加一个字符到打字机末尾
2. 删除打字机末尾的字符
3. 打印单词

构造字典树，最后打印那个最长的单词
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
    std::vector<std::string> words(n);
    Trie trie;
    std::string max;
    for (auto &word : words) {
        std::cin >> word;
        trie.insert_lower(word)->m_is_end = true;
        max = std::max(max, word, [&](const std::string &lhs, const std::string &rhs) {
            return lhs.size() < rhs.size();
        });
    }

    std::string word, ans;
    auto dfs = [&](auto &&self, int node, int deep) -> void {
        if (trie.get_node(node)->m_is_end) {
            ans += "P";
            if (word == max) {
                std::cout << ans.size() << "\n";
                for (auto c : ans) {
                    std::cout << c << "\n";
                }
            }
        }
        auto work = [&](int i) {
            char ch = char(i + 'a');
            if (int next = trie.get_node(node)->get_child(i); next) {
                ans += ch;
                word.push_back(ch);
                self(self, next, deep + 1);
                word.pop_back();
                ans += "-";
            }
        };
        
        std::vector<int> order(26);
        std::iota(order.begin(), order.end(), 0);
        if (deep < max.size()) {
            int j = max[deep] - 'a';
            order.erase(std::find(order.begin(), order.end(), j));
            order.insert(order.end(), j);
        }
        for (int i : order) {
            work(i);
        }
    };
    dfs(dfs, 0, 0);

    return 0;
}
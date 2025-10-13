#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"

/*
[P2536 [AHOI2005]病毒检测](https://www.luogu.com.cn/problem/P2536)
[status](https://www.luogu.com.cn/record/237592978)
*/
/*
给定长度不超过 1000 的模板串，包含 ACTG 和 *? 字符，给出 $$n(1 \leq n \leq 500)$$ 个长度不超过 500 的字符串，问模板串不能匹配多少个字符串

记忆化搜索即可
*/

template <typename Node>
struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
    int cnt = 0;
};

using Node = OY::StaticTrie<NodeWrap, 4>::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;
    int n;
    std::cin >> n;
    OY::StaticTrie<NodeWrap, 4> trie;
    auto get = [&](char ch) {
        if (ch == 'A') return 0;
        if (ch == 'C') return 1;
        if (ch == 'T') return 2;
        if (ch == 'G') return 3;
        return -1;
    };
    for (int i = 0; i < n; i++) {
        std::string word;
        std::cin >> word;
        trie.insert(word.size(), [&](int i) { return get(word[i]); })->cnt++;
    }
    
    int ans = 0;
    auto mapping = [&](int i) { return get(text[i]); };
    const int N = 1000;
    std::vector<std::array<std::bitset<N + 1>, 2>> vis(trie.m_data.size());
    auto dfs = [&](auto &&self, int node, int p, bool any_matched) -> void {
        int idx = any_matched ? 1 : 0;
        if (vis[node][idx].test(p)) {
            return;
        }
        vis[node][idx].set(p);
        if (p == text.size() && !any_matched) {
            ans += trie.get_node(node)->cnt;
            return;
        }
        if (any_matched) {
            self(self, node, p, false);
            for (int i = 0; i < 4; i++) {
                if (int next = trie.get_node(node)->get_child(i); next) {
                    self(self, next, p, false);
                    self(self, next, p, true);
                }
            }
        } else if (mapping(p) != -1) {
            if (int next = trie.get_node(node)->get_child(mapping(p)); next) {
                self(self, next, p + 1, false);
            }
        } else if (text[p] == '?') {
            for (int i = 0; i < 4; i++) {
                if (int next = trie.get_node(node)->get_child(i); next) {
                    self(self, next, p + 1, false);
                }
            }
        } else {
            self(self, node, p + 1, true);
        }
    };
    dfs(dfs, 0, 0, false);

    std::cout << n - ans << "\n";

    return 0;
}
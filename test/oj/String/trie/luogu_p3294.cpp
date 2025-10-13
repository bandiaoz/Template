#include <bits/stdc++.h>
#include "src/String/trie/Trie.h"
#include "src/Tree/VectorTree.h"

using ll = long long;

/*
[P3294 [SCOI2016]背单词](https://www.luogu.com.cn/problem/P3294)
[status](https://www.luogu.com.cn/record/237505676)
*/
/*
给定 $$n(1 \leq n \leq 10^5)$$ 个单词，每个单词由小写字母组成，长度不超过 $$5.1 \times 10^5$$。
排列单词，如果第 $$i$$ 个单词的后缀在位置 $$j(j < i)$$ 出现过，花费 $$j - i$$，否则花费 $$i$$。

将 Trie 重构，只保留单词结尾的节点，然后按照子树大小，从小到大遍历树。
*/

template <typename Node>
struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
    int cnt = 0;
};

using Node = OY::StaticTrie<NodeWrap>::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::StaticTrie<NodeWrap> trie;  
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        std::reverse(s.begin(), s.end());
        trie.insert_lower(s, [](Node *node) { 
            node->cnt++;
        }) -> m_is_end = true;
    }

    OY::VectorTree::Tree<int> tree(n + 1);
    int cur = 1;
    auto dfs = [&](auto &&self, Node *node, int last) -> void {
        if (node->m_is_end) {
            tree.add_edge(last, cur);
            last = cur;
            cur++;
        }
        for (int i = 0; i < 26; i++) {
            if (node->get_child(i)) {
                self(self, trie.get_node(node->get_child(i)), last);
            }
        }
    };
    dfs(dfs, trie.get_node(0), 0);

    std::vector<int> sz(n + 1, 1);
    tree.tree_dp_vertex(0, {}, [&](int u, int v) { sz[u] += sz[v]; }, {});

    ll ans = 0;
    cur = 1;
    auto work = [&](auto &&self, int u, int f, int last) -> void {
        if (u != 0) {
            ans += cur - last;
            last = cur;
            cur++;
        }
        std::vector<int> children;
        tree.do_for_each_adj_vertex(u, [&](int v) {
            if (v == f) return;
            children.push_back(v);
        });
        std::sort(children.begin(), children.end(), [&](int a, int b) {
            return sz[a] < sz[b];
        });
        for (int v : children) {
            self(self, v, u, last);
        }
    };

    work(work, 0, -1, 0);
    std::cout << ans << '\n';

    return 0;
}
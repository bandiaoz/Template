#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/Tree/HeavyLightDecomposition.h"
#include "src/String/trie/Trie.h"

/*
[P6088 [JSOI2015]字符串树](https://www.luogu.com.cn/problem/P6088)
[status](https://www.luogu.com.cn/record/240311070)
*/
/*
给定一棵 $$n(1 \leq n \leq 10^5)$$ 个点的树，树的每条边上有字符串，有 $$q(1 \leq q \leq 10^5)$$ 次询问：
每次输入一个字符串 $$s$$，询问一条路径上有多少字符串以 $$s$$ 为前缀。
所有字符串长度不超过 10。

将询问离线下来，将路径答案转换为树上差分，dfs 的时候使用 Trie 维护链上字符串，计算每条从根开始的链的贡献
*/

template <typename Node>
struct NodeWrap : OY::Trie::BaseNodeWrap<Node> {
    int cnt = 0;
};
using Trie = OY::StaticTrie<NodeWrap, 26>;
using Node = Trie::node;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::VectorTree::Tree<std::string> tree(n);
    Trie trie;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::string s;
        std::cin >> u >> v >> s;
        u--, v--;
        tree.add_edge(u, v, s);
        trie.insert_lower(s);
    }

    tree.set_root(0);
    tree.prepare();
    OY::HLD::Table<decltype(tree)> hld(&tree);

    int q;
    std::cin >> q;
    std::vector<std::vector<std::tuple<int, int, std::string>>> con(n);
    for (int i = 0; i < q; i++) {
        int u, v;
        std::string s;
        std::cin >> u >> v >> s;
        u--, v--;

        int lca = hld.lca(u, v);
        con[u].emplace_back(i, 1, s);
        con[v].emplace_back(i, 1, s);
        con[lca].emplace_back(i, -2, s);
    }

    std::vector<int> ans(q);
    auto dfs = [&](auto &&self, int u, int f, std::string s) -> void {
        trie.trace_lower(s, [&](Node *node) { node->cnt++; });
        for (auto [i, op, s] : con[u]) {
            if (auto node = trie.find_lower(s); node != nullptr) {
                ans[i] += op * node->cnt;
            }
        }
        tree.do_for_each_adj_edge(u, [&](int v, std::string s) {
            if (v == f) return;
            self(self, v, u, s);
        });
        trie.trace_lower(s, [&](Node *node) { node->cnt--; });
    };
    dfs(dfs, 0, -1, "");

    for (auto i : ans) {
        std::cout << i << "\n";
    }

    return 0;
}
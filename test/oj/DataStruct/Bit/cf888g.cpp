#include <bits/stdc++.h>
#include "src/DataStruct/Bit/BiTrie.h"

using ll = long long;

/*
[G. Xor-MST](https://codeforces.com/contest/888/problem/G)
[status](https://codeforces.com/contest/888/submission/300302842)
*/
/**
 * 给定无向图，边 (u, v) 的权值为 a[u] ^ a[v]，求 MST 权值和。
 * 
 * Boruvka 的思想：每一轮，每个连通块都选择一个最小的出边，然后合并
 * 
 * 先将所有权值插入到 trie 中，可以看成 trie 的叶子节点为一个连通块，向上合并
 * 如果 trie 的节点有两个分支，那么合并的代价就是两个子树各取一个值，异或最小
 */

struct Info {
    int val;
    int l, r;
    Info() : val(0), l(-1), r(-1) {}
};

using Trie = OY::BiTrie::Tree<uint32_t, 30, Info>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &i : a) {
        std::cin >> i;
    }    

    std::ranges::sort(a);

    Trie trie;
    for (int i = 0; i < n; i++) {
        int v = 1 << 29;
        trie.insert(a[i], [&](Trie::node *node) { 
            node->val = v; 
            v >>= 1;
            if (node->l == -1) {
                node->l = i;
            }
            node->r = i;
        });
    }

    auto dfs = [&](auto &&self, Trie::node *node) -> ll {
        if (node->is_null()) return 0;
        if (!node->child0()->is_null() && !node->child1()->is_null()) {
            ll ans = 1 << 30;
            int l = node->child0()->l;
            int r = node->child0()->r;
            int dif_val = node->child0()->val;
            // 枚举左子树的值，在右子树中找到最小的异或值
            // 每个点只会在祖先合并时被枚举到，所以枚举次数最多是 trie 的高度
            for (int i = l; i <= r; i++) {
                auto [_, min] = trie.min_bitxor(a[i], [&](Trie::node *node) {
                    if (node->is_null()) return false;
                    if (node->val == dif_val) {
                        return false;
                    }
                    return true;
                });
                ans = std::min<ll>(ans, min);
            }
            return ans + self(self, node->child0()) + self(self, node->child1());
        } else if (!node->child0()->is_null()) {
            return self(self, node->child0());
        } else {
            return self(self, node->child1());
        }
    };

    std::cout << dfs(dfs, trie.root()) << "\n";

    return 0;
}
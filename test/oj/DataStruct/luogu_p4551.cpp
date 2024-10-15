#include <bits/stdc++.h>
#include "src/DataStruct/BiTrie.h"

using ll = long long;

/*
[P4551 最长异或路径](https://www.luogu.com.cn/problem/P4551)
[status](https://www.luogu.com.cn/record/180827144)
*/
/**
 * 简单路径的 xor sum 可以在 lca 处结算
 * lca 处将各个子树的路径都合并在一起，且在合并的时候计算最大异或
 *
 * 也可以直接求出每个点到根的异或，然后求全局最大异或对
 *
 * 当然，不使用数据结构的分治搜索是最快的
 */

void solve_bitrie() {
    int n;
    std::cin >> n;
    std::vector<std::vector<std::pair<int, int>>> g(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        u--, v--;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }

    OY::BiTrie::Tree<uint32_t, 31> trie;
    uint32_t ans = 0;
    auto dfs = [&](auto &&self, int u, int f, int cur) -> void {
        trie.insert(cur);
        ans = std::max(ans, trie.max_bitxor(cur).second);
        for (auto [v, w] : g[u]) {
            if (v == f) continue;
            self(self, v, u, cur ^ w);
        }
    };
    dfs(dfs, 0, -1, 0);

    std::cout << ans;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve_bitrie();

    return 0;
}
#include <bits/stdc++.h>
#include "src/Tree/VectorTree.h"
#include "src/DataStruct/DSU/DSU.hpp"
#include "src/Tree/HLDZkw.h"

/*
[CF160D Edges in MST](https://codeforces.com/contest/160/problem/D)
[status](https://codeforces.com/contest/160/submission/338258850)
*/
/*
给定带权无向图，确定每一边：
1. 一定在所有 MST 上
2. 可能在某个 MST 上
3. 一定不在任何 MST 上

先随机生成一棵 MST
对于所有不在这棵 MST 上的边 $$(u, v, w)$$，如果路径 $$u \to v$$ 上的最大边权等于 $$w$$，这条边可以在某个 MST 上，并且边权等于 $$w$$ 的边可以被替换，否则（路径边权全部严格小于 $$w$$）一定不在任何 MST 上
对于在 MST 上的边，如果对于不在 MST 上的边 $$(u, v, w)$$，路径 $$u \to v$$ 上的最大边权严格小于 $$w$$，则这条边一定在所有 MST 上，否则只是可能在某个 MST 上
*/

struct Info { int min, max; };
struct Tag { int min, max; };
auto op = [](Info a, Info b) -> Info { 
    return {std::min(a.min, b.min), std::max(a.max, b.max)}; 
};
auto map = [](Tag modify, Info old_val, uint32_t len) -> Info { 
    return {std::min(old_val.min, modify.min), std::max(old_val.max, modify.max)}; 
};
auto com = [](Tag new_modify, Tag old_modify) -> Tag { 
    return {std::min(new_modify.min, old_modify.min), std::max(new_modify.max, old_modify.max)}; 
};

const int inf = 2e9;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<std::array<int, 5>> edges(m);
    std::map<std::pair<int, int>, int> mp;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        std::cin >> u >> v >> w;
        u--, v--;
        edges[i] = {u, v, w, i, false};
        mp[{u, v}] = mp[{v, u}] = i;
    }

    std::sort(edges.begin(), edges.end(), [&](const auto &a, const auto &b) {
        return a[2] < b[2];
    });

    OY::DSUTable<true> dsu(n);
    OY::VectorTree::Tree<int> tree(n);
    for (auto &[u, v, w, i, used] : edges) {
        if (dsu.unite_by_ID(u, v)) {
            used = true;
            tree.add_edge(u, v, w);
        }
    }
    
    tree.set_root(0);
    tree.prepare();
    
    std::vector<int> fa(n, -1), val(n);
    tree.tree_dp_edge(0, [&](int u, int f, int w) { val[u] = w, fa[u] = f; }, {}, {});
    
    const int inf = 2e9;
    auto mapping = [&](int u) { return Info{inf, val[u]}; };
    auto hld = OY::make_lazy_HLDZkw<Info, Tag, true>(&tree, mapping, op, map, com, Tag{inf, 0});
    auto zkw = hld.m_zkw;
    std::vector<int> ans(m, 1), diff(n + 1);
    for (auto &[u, v, w, i, used] : edges) {
        if (!used) {
            auto [_, max] = hld.query_path<false>(u, v);
            if (max == w) {
                ans[i] = 2;
                hld.add_path<false>(u, v, Tag{w, 0});
            } else {
                ans[i] = 3;
            }
        }
    }

    for (int u = 1; u < n; u++) {
        auto [min, _] = hld.query(u);
        int f = fa[u], id = mp[{u, f}];
        if (val[u] < min) {
            ans[id] = 1;
        } else {
            ans[id] = 2;
        }
    }

    for (int i = 0; i < m; i++) {
        if (ans[i] == 1) {
            std::cout << "any\n";
        } else if (ans[i] == 2) {
            std::cout << "at least one\n";
        } else {
            std::cout << "none\n";
        }
    }

    return 0;
}
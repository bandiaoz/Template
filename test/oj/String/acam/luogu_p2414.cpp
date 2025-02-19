#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Tree/VectorTree.h"
#include "src/DataStruct/Fenwick/MonoBIT.h"

/*
[P2414 [NOI2011] 阿狸的打字机](https://www.luogu.com.cn/problem/P2414)
[status](https://www.luogu.com.cn/record/202442298)
*/
/**
 * 给定若干模式串，有 m 次询问，每次询问第 x 个模式串在第 y 个模式串中出现了多少次
 * 
 * x 在 y 中出现次数等价于所有 y 的前缀中，有多少个状态可以通过 fail 跳到 x
 * 进一步转换为，在 fail 树上，x 子树中，有多少状态是 y 的前缀状态
 * fail 子树转换为 dfs 序，dfs 搜索 trie 树，当前节点为 y，用 Fenwick 维护 y 前缀节点，查询 x 子树区间中有多少个点是 y 的前缀状态即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    OY::ACAM<26> ac;
    const int N = 100000;
    ac.reserve(N);

    std::vector<int> que{0}, pos;
    for (auto c : s) {
        if (c == 'B') {
            que.pop_back();
        } else if (c == 'P') {
            pos.push_back(que.back());
        } else {
            int x = que.back();
            x = ac._get_or_new(x, c - 'a');
            que.push_back(x);
        }
    }

    OY::VectorTree::Tree<bool> trie_tree(ac.m_data.size()), fail_tree(ac.m_data.size());
    ac.prepare([&](int u, int v) {
        trie_tree.add_edge(u, v);
    });
    ac.do_for_failing_nodes([&](int u) {
        fail_tree.add_edge(ac.query_fail(u), u);
    });

    std::vector<int> dfn(ac.m_data.size()), size(ac.m_data.size(), 1);
    int cur = 0;
    auto dfs = [&](auto &&self, int u, int f) -> void {
        dfn[u] = cur++;
        fail_tree.do_for_each_adj_vertex(u, [&](int v) {
            if (v == f) return;
            self(self, v, u);
            size[u] += size[v];
        });
    };
    dfs(dfs, 0, -1);

    std::vector<std::vector<std::pair<int, int>>> query(ac.m_data.size());

    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        x--, y--;
        query[pos[y]].emplace_back(pos[x], i);
    }

    std::vector<int> ans(m);
    OY::MonoSumBIT<int> fen(ac.m_data.size());
    auto dfs2 = [&](auto &&self, int u, int f) -> void {
        fen.add(dfn[u], 1);
        for (auto [x, id] : query[u]) {
            ans[id] = fen.query(dfn[x], dfn[x] + size[x]);
        }
        trie_tree.do_for_each_adj_vertex(u, [&](int v) {
            if (v == f) return;
            self(self, v, u);
        });
        fen.add(dfn[u], -1);
    };
    dfs2(dfs2, 0, -1);

    for (int i = 0; i < m; i++) {
        std::cout << ans[i] << '\n';
    }

    return 0;
}
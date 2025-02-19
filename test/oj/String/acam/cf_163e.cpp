#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/DataStruct/Fenwick/BIT_ex.h"
#include "src/Tree/VectorTree.h"

using ll = long long;

/*
[E. e-Government](https://codeforces.com/contest/163/problem/E)
[status](https://codeforces.com/contest/163/submission/305781923)
*/
/**
 * 有 n 个模式串和 q 次操作，有三种类型操作：
 * 1. 询问所有模式串在字符串 t 中出现次数的总和
 * 2. 添加编号为 i 的模式串，如果已经存在则不添加
 * 3. 删除编号为 i 的模式串，如果已经不存在则不删除
 * 
 * 建出 ac 自动机，每个模式串在 t 中出现次数转换为 fail 树上，模式串的结尾对子树做贡献
 * 询问操作等价于 t 对应的状态到 fail 树的根的总和，每个状态单点求值相加
 * 添加和删除操作，使用树状数组维护对 fail 子树进行修改即可
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q, n;
    std::cin >> q >> n;
    
    OY::ACAM<26> ac;
    const int N = 1e6;
    ac.reserve(N);
    std::vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        pos[i] = ac.insert_lower(s);
    }

    ac.prepare();

    OY::VectorTree::Tree<bool> tree(ac.m_data.size());
    ac.do_for_failing_nodes([&](int u) {
        tree.add_edge(ac.query_fail(u), u);
    });
    tree.set_root(0);
    tree.prepare();

    std::vector<int> dfn(ac.m_data.size()), sz(ac.m_data.size(), 1);
    int cur = 0;
    auto dfs = [&](auto &&dfs, int u, int f) -> void {
        dfn[u] = cur++;
        tree.do_for_each_adj_vertex(u, [&](int v) {
            if (v == f) return;
            dfs(dfs, v, u);
            sz[u] += sz[v];
        });
    };
    dfs(dfs, 0, -1);

    OY::VectorBIT_ex<int> fen(ac.m_data.size());
    std::vector<int> active(n, 1);
    for (auto p : pos) {
        fen.add(dfn[p], dfn[p] + sz[p], 1);
    }
    
    while (q--) {
        std::string op;
        std::cin >> op;

        if (op.front() == '?') {
            int p = 0;
            ll ans = 0;
            for (int i = 1; i < op.size(); i++) {
                p = ac.next(p, op[i] - 'a');
                ans += fen.query(dfn[p]);
            }
            std::cout << ans << "\n";
        } else {
            int i = std::stoi(op.substr(1)) - 1;
            int u = pos[i], l = dfn[u], r = dfn[u] + sz[u];
            if (op.front() == '+' && !active[i]) {
                active[i] = 1;
                fen.add(l, r, 1);
            } else if (op.front() == '-' && active[i]) {
                active[i] = 0;
                fen.add(l, r, -1);
            }
        }
    }

    return 0;
}
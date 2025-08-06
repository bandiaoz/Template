#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Tree/VectorTree.h"
#include "src/Tree/HLDZkw.h"

/*
[CF1437G Death DBMS](https://codeforces.com/contest/1437/problem/G)
[status](https://codeforces.com/contest/1437/submission/329022762)
*/
/*
给定 n 个字符串和 m 次操作，每个字符串初始权值为 0
操作有两种：
1. 将第 i 个字符串的权值修改为 $$x (1 \leq x \leq 10^9)$$
2. 给出字符串 q，求 q 的子串的最大权值
保证 $$\sum |s_i| \leq 3 \times 10^5, \sum |q| \leq 3 \times 10^5$$

建立 ac 自动机，q 的子串的最大权值，转换为 fail 树上，q 的每个前缀对应结点为 p，求从根到 p 的路径上的最大权值，用树剖线段树维护
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    OY::ACAM<26> ac;
    const int N = 300000;
    ac.reserve(N);
    std::vector<std::string> a(n);
    std::vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
        pos[i] = ac.insert_lower(a[i]);
    }
    ac.prepare();

    OY::VectorTree::Tree<bool> fail_tree(ac.m_data.size());
    ac.do_for_failing_nodes([&](int u) {
        fail_tree.add_edge(u, ac.query_fail(u));
    });
    fail_tree.set_root(0);
    fail_tree.prepare();

    std::vector<std::multiset<int>> val_st(ac.m_data.size());
    std::vector<int> val(n);
    for (int i = 0; i < n; i++) {
        val_st[pos[i]].insert(val[i]);
    }

    auto mapping = [&](int u) { return val_st[u].empty() ? -1 : *val_st[u].rbegin(); };
    auto zkw = OY::make_HLDZkw<int>(&fail_tree, std::max<int>, mapping);

    while (m--) {
        int op;
        std::cin >> op;

        if (op == 1) {
            int i, x;
            std::cin >> i >> x;
            i--;

            int p = pos[i];
            val_st[p].extract(val[i]);
            val[i] = x;
            val_st[p].insert(val[i]);
            zkw.modify(p, mapping(p));
        } else {
            std::string text;
            std::cin >> text;
            
            int ans = -1;
            for (int i = 0, p = 0; i < text.size(); i++) {
                p = ac.next(p, text[i] - 'a');
                ans = std::max(ans, zkw.query_path<true>(0, p));
            }

            std::cout << ans << "\n";
        }
    }

    return 0;
}
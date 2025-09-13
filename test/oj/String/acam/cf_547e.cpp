#include <bits/stdc++.h>
#include "src/String/acam/ACAutomaton.h"
#include "src/Tree/VectorTree.h"
#include "src/DataStruct/Fenwick/MonoBIT.h"

/*
[CF547E Mike and Friends](https://codeforces.com/contest/547/problem/E)
[status](https://codeforces.com/contest/547/submission/329000133)
*/
/*
给定 n 个字符串 a[1...n]，q 次询问，每次询问字符串 a[k] 在 a[l...r] 中出现的次数（单个字符串可以多次贡献）。
$$1 \leq n \leq 2 \times 10^5, 1 \leq q \leq 5 \times 10^5, \sum |a_i| \leq 2 \times 10^5$$

用 n 个字符串建立 ac 自动机，建 fail 树
a[k] 在 a[l...r] 中的出现次数等价于在 a[1...r] 中的出现次数减去在 a[1...l-1] 中的出现次数

套路：a[k] 在模式串 s 中的出现次数，等价于 s 的所有前缀，有多少个状态可以通过 fail 跳到 a[k]。进一步转换为，在 fail 树上，a[k] 子树中，有多少状态是 s 的前缀状态。
将一个询问拆成两个前缀询问，按照右端点排序，逐个字符串的每个前缀添加到树状数组中，查询 fail 树的子树和。
*/

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    OY::ACAM<26> ac;
    const int N = 200000;
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
        fail_tree.add_edge(ac.query_fail(u), u);
    });

    std::vector<int> dfn(ac.m_data.size()), size(ac.m_data.size(), 1);
    int cur = 0;
    fail_tree.tree_dp_vertex(0, [&](int u, int f) { dfn[u] = cur++; }, [&](int u, int v) { size[u] += size[v]; }, {});

    std::vector<std::array<int, 3>> query(q * 2);
    for (int i = 0; i < q; i++) {
        int l, r, k;
        std::cin >> l >> r >> k;
        l--, k--;
        query[i * 2] = {l, k, i * 2};
        query[i * 2 + 1] = {r, k, i * 2 + 1};
    }

    std::sort(query.begin(), query.end());

    OY::MonoSumBIT<int> fen(ac.m_data.size());
    std::vector<int> ans(q);
    for (int i = 0, j = 0; i <= n; i++) {
        while (j < query.size() && query[j][0] <= i) {
            auto [x, k, idx] = query[j];
            int f = idx % 2 == 0 ? -1 : 1;
            ans[idx / 2] += f * fen.query(dfn[pos[k]], dfn[pos[k]] + size[pos[k]]);
            j++;
        }

        if (i < n) {
            int p = 0;
            for (auto c : a[i]) {
                p = ac.next(p, c - 'a');
                fen.add(dfn[p], 1);
            }
        }
    }

    for (int i = 0; i < q; i++) {
        std::cout << ans[i] << '\n';
    }

    return 0;
}